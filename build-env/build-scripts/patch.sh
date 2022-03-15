. $DIST_ROOT/build-env/build-scripts/inc-start.sh $1 $(basename $0)

./configure --prefix=/usr   \
            --host=$LFS_TGT \
            --build=$(build-aux/config.guess)

make
make DESTDIR=$LFS install

. $DIST_ROOT/build-env/build-scripts/inc-end.sh $1 $(basename $0)