export SRC_DIR=/mnt/z/Projets/C++/Ashes

rm -r ./build-clang
rm -r ./build-gcc
rm -r ./vcpkg-clang
rm -r ./vcpkg-gcc

mkdir build-clang
mkdir build-gcc
mkdir vcpkg-clang
mkdir vcpkg-gcc

cd ./build-clang
CC=clang-15 CXX=clang++-15 cmake $SRC_DIR --preset dev-ninja-debug -DPROJECTS_OUTPUT_DIR=$PWD
ninja -j30

cd ../build-gcc
CC=gcc-12 CXX=g++-12 cmake $SRC_DIR --preset dev-ninja-release -DPROJECTS_OUTPUT_DIR=$PWD
ninja -j30

cd ../vcpkg-clang
CC=clang-15 CXX=clang++-15 cmake $SRC_DIR --preset dev-vcpkg-ninja-release -DPROJECTS_OUTPUT_DIR=$PWD -DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/external/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake .
ninja -j30

cd ../vcpkg-gcc
CC=gcc-12 CXX=g++-12 cmake $SRC_DIR --preset dev-vcpkg-ninja-debug -DPROJECTS_OUTPUT_DIR=$PWD -DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/external/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake .
ninja -j30

cd ..
