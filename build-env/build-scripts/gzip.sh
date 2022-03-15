. $DIST_ROOT/build-env/build-scripts/inc-start.sh $1 $(basename $0)

./configure --prefix=/usr --host=$LFS_TGT

make
make DESTDIR=$LFS install
mv -v $LFS/usr/bin/gzip $LFS/bin

. $DIST_ROOT/build-env/build-scripts/inc-end.sh $1 $(basename $0)