rm -r -f build
mkdir build
cd build
cmake ../src
make all
./convolution_matrix00
