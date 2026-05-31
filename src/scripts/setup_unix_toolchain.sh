export GCC_VERSION=4.0.4
export BINUTILS_VERSION=2.12
export NEWLIB_VERSION=1.11.0
clear
echo "------------------------------------------------------------------------------"
echo "Checking data presence"
echo " "
if [ ! -f "binutils-$BINUTILS_VERSION.tar.gz" ]; then
    echo "Getting Binutils..."
    wget "https://mirrors.kernel.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz" >> setup.log
fi
if [ ! -f "gcc-$GCC_VERSION.tar.gz" ]; then
    echo "Getting GCC..."
    wget "https://mirrors.kernel.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz"  >> setup.log
fi
if [ ! -f "gcc-g++-$GCC_VERSION.tar.gz" ]; then
    echo "Getting GPP..."
    wget "https://mirrors.kernel.org/gnu/gcc/gcc-$GCC_VERSION/gcc-g++-$GCC_VERSION.tar.gz"  >> setup.log
fi
if [ ! -f "newlib-$NEWLIB_VERSION.tar.gz" ]; then
    echo "Getting NewLib..."
    wget "http://gsdview.appspot.com/nativeclient-mirror/toolchain/newlib/newlib-$NEWLIB_VERSION.tar.gz"  >> setup.log
fi
if [ ! -f "crtls.zip" ]; then
    echo "Getting CRT Link Script..."
    wget "http://www.devrs.com/gba/files/crtls.zip" >> setup.log
fi
echo " "
echo "Needed packages are present. Proceeding..."
echo " "
echo "------------------------------------------------------------------------------"
echo "Creating Directories"
mkdir build-binutils 1>> setup.log 2>> setup.log
mkdir build-gcc 1>> setup.log 2>> setup.log
mkdir build-newlib 1>> setup.log 2>> setup.log
echo " "
echo "------------------------------------------------------------------------------"
echo "Decompressing data"
echo " "
if [ ! "$(ls -A ./build-binutils)" ]; then
    echo "--------------------------------------"
    echo "Decompressing BinUtils..."
    cd build-binutils
    tar xvf ../binutils-$BINUTILS_VERSION.tar.gz  >> setup.log
    cd ..
fi
if [ ! "$(ls -A ./build-gcc)" ]; then
    echo "--------------------------------------"
    echo "Decompressing GCC..."
    cd build-gcc
    tar xvf ../gcc-$GCC_VERSION.tar.gz  >> setup.log
    #echo "--------------------------------------"
    #echo "Decompressing C++..."
    #tar xvf ../gcc-g++-$GCC_VERSION.tar.gz
    cd ..
fi
if [ ! "$(ls -A ./build-newlib)" ]; then
    echo "--------------------------------------"
    echo "Decompressing NewLib..."
    cd build-newlib
    tar xvf ../newlib-$NEWLIB_VERSION.tar.gz  >> setup.log
    cd ..
fi
echo "--------------------------------------"
echo "Decompressing CRT Link Script..."
unzip -o crtls.zip >> setup.log
echo " "
#echo "------------------------------------------------------------------------------"
#echo "Cleaning Up"
#rm binutils-$BINUTILS_VERSION.tar.gz 1>> setup.log 2>> setup.log
#rm gcc-$GCC_VERSION.tar.gz 1>> setup.log 2>> setup.log
#rm gcc-g++-$GCC_VERSION.tar.gz1>> setup.log 2>> setup.log
#rm newlib-$NEWLIB_VERSION.tar.gz 1>> setup.log 2>> setup.log
#rm crtls.zip 1>> setup.log 2>> setup.log
#echo " "
#echo "------------------------------------------------------------------------------"
#echo "Generating Configure scripts"
echo "#!/bin/bash" > build-binutils/confgure.sh
echo ./binutils-$BINUTILS_VERSION/configure --target=arm-thumb-elf >> build-binutils/configure.sh 
echo 'echo "You can type make now"' >> build-binutils/configure.sh 
echo "#!/bin/bash" > build-gcc/configure.sh
echo ./gcc-$GCC_VERSION/configure --target=arm-thumb-elf --with-cpu=arm7tdmi --with-newlib --enable-multilib --enable-interwork --disable-threads --enable-targets=arm-elf --with-headers=../build-newlib/newlib-$NEWLIB_VERSION/newlib/libc/include --enable-languages="c" >> build-gcc/configure.sh 
echo 'echo "You can type make now"' >> build-gcc/configure.sh 
echo "#!/bin/bash" > build-newlib/configure.sh
echo ./newlib-$NEWLIB_VERSION/configure --target=arm-thumb-elf >> build-newlib/configure.sh 
echo 'echo "You can type make now"' >> build-newlib/configure.sh 
chmod 777 build-newlib/configure.sh
chmod 777 build-binutils/configure.sh
chmod 777 build-gcc/configure.sh
echo " "
echo "Finished Preparing Packages. You can call configure.sh in each build-xxxx folder now. "
