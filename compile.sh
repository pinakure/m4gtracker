export GCC_VERSION=3.0.4
export BINUTILS_VERSION=2.11
export NEWLIB_VERSION=1.11.0
echo "------------------------------------------------------------------------------"
echo "Configure Binutils"
cd build-binutils
chmod 777 * -R
echo "------------------------------------------------------------------------------" > configure.log
echo "CONFIGURING BINUTILS" >> configure.log
echo "--------------------------------------<----------------------------------------" >> configure.log
./binutils-$BINUTILS_VERSION/configure --target=arm-thumb-elf >> configure.log
make
make install
echo "------------------------------------------------------------------------------"
echo " "
