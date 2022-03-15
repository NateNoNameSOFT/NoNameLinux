. $DIST_ROOT/build-env/build-scripts/inc-start.sh $1 $(basename $0)

mkdir -v build
cd build

../configure --prefix=$LFS/tools \
	--with-sysroot=$LFS \
	--target=$LFS_TGT \
	--disable-nls \
	--disable-werror

make && make install

. $DIST_ROOT/build-env/build-scripts/inc-end.sh $1 $(basename $0)