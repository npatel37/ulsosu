# Quick Start

## Licensing


 The full software license for DMRG++ version 3 
 can be found in file LICENSE in the root directory of the code.
 DMRG++ is a free and open source implementation of the
 DMRG algorithm. You are welcomed to use it and publish data
 obtained with DMRG++. If you do,
<b>please cite this work</b> (see next subsection).

## DISCLAIMER

<pre>
 THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
 CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT OWNER, CONTRIBUTORS, UNITED STATES GOVERNMENT,
 OR THE UNITED STATES DEPARTMENT OF ENERGY BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 DAMAGE.

 NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED
 STATES DEPARTMENT OF ENERGY, NOR THE COPYRIGHT OWNER, NOR
 ANY OF THEIR EMPLOYEES, REPRESENTS THAT THE USE OF ANY
 INFORMATION, DATA, APPARATUS, PRODUCT, OR PROCESS
 DISCLOSED WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.
</pre>

## How To Cite This Work

<pre>
\@article{re:alvarez0209,
author="G. Alvarez",
title="The Density Matrix Renormalization Group for
Strongly Correlated Electron Systems: A Generic Implementation",
journal="Computer Physics Communications",
volume="180",
pages="1572-1578",
year="2009"}

\@article{re:alvarez0310,
author="G. Alvarez",
title="Implementation of the SU(2) Hamiltonian
Symmetry for the DMRG Algorithm",
journal="Computer Physics Communications",
volume="183",
pages="2226-2232",
year="2012"}


\@article{re:alvarez0311,
author="G. Alvarez and  L. G. G. V. Dias da Silva and
E. Ponce and  E. Dagotto",
title="Time Evolution with the DMRG Algorithm:
A Generic Implementation
for Strongly Correlated Electronic Systems",
journal="Phys. Rev. E",
volume="84",
pages="056706",
year="2011"}

\@article{re:alvarez0713,
author="G. Alvarez",
title="Production of minimally entangled typical thermal states
with the Krylov-space approach",
journal="Phys. Rev. B",
volume="87",
pages="245130",
year="2013"}

And also:
\@article{re:alvarez08,
 re:webDmrgPlusPlus,
 Author = {G. Alvarez},
 Title = {DMRG++ Website},
 Publisher = {\\url{https://g1257.github.com/dmrgPlusPlus}} }
</pre>

## Code Integrity

Hash of the latest commit is also posted at

https://g1257.github.com/hashes.html

Latest commit should always be signed.
Keys at https://g1257.github.com/keys.html

## Building and Running DMRG++

### Required Software

\begin{enumerate}

\item GNU C++ or LLVM CLANG++

\item (required) The BLAS and LAPACK library.

\item (required) HDF5

\item (required) PsimagLite. See below for details.

\item (optional) boost-devel (boost-spirit) for Ainur

\item (optional) GSL (GNU Scientific library)

\item (optional) make or gmake
(only needed to use the Makefile)

\item (optional) perl
(only needed to run the configure.pl script)

\end{enumerate}

### Downloading DMRG++
Create a directory somewhere and cd to it.

<pre>
git clone https://code.ornl.gov/gonzalo_3/PsimagLite
cd PsimagLite/
git checkout features
git pull origin features 

cd ../
git clone https://code.ornl.gov/gonzalo_3/dmrgpp
cd dmrgpp/
git checkout features
git pull origin features
</pre>
Please also read https://g1257.github.io/availabilityOfSoftware.html

### Preparing your Makefile

Create a file anywhere
called /path/to/somewhere/myconfig.psiTag

Now have a look at dmrgpp/TestSuite/inputs/ConfigBase.psiTag
DO NOT Edit this file. You may override items in that file using
your myconfig.psiTag.
For example, to use openblas instead of regular blas, you may
write in your myconfig.psiTag
<pre>
	dependency BLAS = LDFLAGS += -lopenblas
	dependency LAPACK= ()
</pre>
For the psiTag syntax see the beginning of PsimagLite/scripts/PsiTag.pm

### Compiling and Linking DMRG++
<pre>
 cd PsimagLite/lib
 ./configure.pl -c /path/to/somewhere/myconfig.psiTag
 make -j something
 cd ../../
 cd dmrgpp/src
 ./configure.pl -c /path/to/somewhere/myconfig.psiTag
 make -j something
</pre>

Above <code>something</code> refers to the number of cores you want
to use for parallelizing the make run.

### Running DMRG++

Assuming you are in dmrgpp/src,
copy input2.inp to dmrgpp/src with

<code>cp ../TestSuite/inputs/input2.inp .</code>

and then you may run with

<code>./dmrg -f ../TestSuite/inputs/input2.inp</code>

You will now have two files a data2.hdf5 and an ASCII file runForinput2.cout.
The name data2 is obtained from the corresponding label in the input file,
in this case input2.inp. Normally the code writes stdout to
runForinput2.cout for an input called input2.inp, and stderr to the
terminal. If you would like to override the default inferred
name runForinput2.cout you may use 
<code>./dmrg -f ../TestSuite/inputs/input2.inp -l myoutputfile</code>
If you would like stdout be written to the terminal say -l -
 
