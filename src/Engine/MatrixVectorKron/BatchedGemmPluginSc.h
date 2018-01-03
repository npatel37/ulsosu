#ifndef BATCHEDGEMM_H
#define BATCHEDGEMM_H
#include <cassert>
#include <complex>
#include "Vector.h"
#include "../../../../dmrgppPluginSc/src/BatchedGemm.h"

typedef PsimagLite::Vector<IntegerType>::Type VectorIntegerType;
typedef PsimagLite::Vector<int>::Type VectorIntType;

namespace Dmrg {

template<typename InitKronType>
class BatchedGemm2 {

	typedef typename InitKronType::ArrayOfMatStructType ArrayOfMatStructType;
	typedef typename ArrayOfMatStructType::MatrixDenseOrSparseType MatrixDenseOrSparseType;
	typedef typename MatrixDenseOrSparseType::VectorType VectorType;
	typedef typename InitKronType::SparseMatrixType SparseMatrixType;
	typedef typename SparseMatrixType::value_type ComplexOrRealType;
	typedef typename MatrixDenseOrSparseType::MatrixType MatrixType;
	typedef typename InitKronType::GenIjPatchType GenIjPatchType;
	typedef typename GenIjPatchType::BasisType BasisType;
	typedef BatchedGemm<ComplexOrRealType> BatchedGemmPluginScType;

	static const typename InitKronType::WhatBasisEnum DUMMY = InitKronType::OLD;

public:

	BatchedGemm2(const InitKronType& initKron)
	    :  initKron_(initKron), batchedGemm_(0)
	{
		if (!enabled()) return;
		std::cout<<"PLUGIN_SC: is in use\n";
		convertOffsets(offsets_);
		SizeType npatches = initKron_.numberOfPatches(DUMMY);
		SizeType nC = initKron_.connections();
		ComplexOrRealType** aptr = new ComplexOrRealType*[npatches*npatches*nC];
		ComplexOrRealType** bptr = new ComplexOrRealType*[npatches*npatches*nC];
		VectorIntType ldAptr(npatches*npatches*nC);
		VectorIntType ldBptr(npatches*npatches*nC);

		convertToVector(pLeft_, initKron_.lrs(DUMMY).left(), GenIjPatchType::LEFT);
		convertToVector(pRight_, initKron_.lrs(DUMMY).right(), GenIjPatchType::RIGHT);
		assert(pLeft_.size() == npatches);
		assert(pRight_.size() == npatches);

		for (SizeType outPatch = 0; outPatch < npatches; ++outPatch) {
			for (SizeType inPatch = 0; inPatch < npatches; ++inPatch) {
				for (SizeType ic=0;ic<nC;++ic) {
					const ArrayOfMatStructType& xiStruct = initKron_.xc(ic);
					const ArrayOfMatStructType& yiStruct = initKron_.yc(ic);

					const MatrixDenseOrSparseType& Amat = xiStruct(outPatch,inPatch);
					const MatrixDenseOrSparseType& Bmat = yiStruct(outPatch,inPatch);

					const MatrixType& AmatDense = Amat.dense();
					const MatrixType& BmatDense = Bmat.dense();

					ComplexOrRealType* a = const_cast<ComplexOrRealType*>(&(AmatDense(0,0)));
					ComplexOrRealType* b = const_cast<ComplexOrRealType*>(&(BmatDense(0,0)));
					aptr[outPatch + inPatch*npatches + ic*npatches*npatches] = a;
					bptr[outPatch + inPatch*npatches + ic*npatches*npatches] = b;

					initKron_.checks(Amat, Bmat, outPatch, inPatch);

					ldAptr[outPatch + inPatch*npatches + ic*npatches*npatches] = AmatDense.rows();
					ldBptr[outPatch + inPatch*npatches + ic*npatches*npatches] = BmatDense.rows();
				}
			}
		}

		batchedGemm_ = new BatchedGemmPluginScType(nC,
		                                           npatches,
		                                           &(pLeft_[0]),
		        &(pRight_[0]),
		        aptr,
		        &(ldAptr[0]),
		        bptr,
		        &(ldBptr[0]));
		delete[] aptr;
		aptr = 0;
		delete[] bptr;
		bptr = 0;
	}

	BatchedGemm2()
	{
		delete batchedGemm_;
		batchedGemm_ = 0;

	}

	bool enabled() const { return initKron_.batchedGemm(); }

	void matrixVector(VectorType& vout, const VectorType& vin) const
	{
		assert(enabled());
		ComplexOrRealType* vinptr = const_cast<ComplexOrRealType*>(&(vin[0]));
		ComplexOrRealType* voutptr = const_cast<ComplexOrRealType*>(&(vout[0]));
		batchedGemm_->apply_Htarget(vinptr, voutptr);
	}

private:

	void convertToVector(VectorIntType& v,
	                     const BasisType& b,
	                     typename GenIjPatchType::LeftOrRightEnumType lOrR) const
	{
		SizeType npatches = initKron_.numberOfPatches(DUMMY);
		v.clear();
		v.resize(npatches, 0);
		for (SizeType ipatch = 0; ipatch < npatches; ++ipatch) {
			SizeType igroup = initKron_.patch(DUMMY,
			                                  lOrR)[ipatch];
			v[ipatch] = b.partition(igroup + 1) - b.partition(igroup);
		}
	}

	void convertOffsets(VectorIntegerType& v) const
	{
		SizeType npatches = initKron_.numberOfPatches(DUMMY);
		v.clear();
		v.resize(npatches, 0);
		for (SizeType i = 0; i < npatches; ++i)
			v[i] =initKron_.offsetForPatches(DUMMY, i);
	}

	BatchedGemm2(const BatchedGemm2&);

	BatchedGemm2& operator=(const BatchedGemm2&);

	const InitKronType& initKron_;
	VectorIntegerType offsets_;
	VectorIntType pLeft_;
	VectorIntType pRight_;
	BatchedGemm<ComplexOrRealType>* batchedGemm_;
};
}
#endif // BATCHEDGEMM_H
