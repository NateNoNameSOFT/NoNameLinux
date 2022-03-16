. $DIST_ROOT/build-env/build-scripts/inc-start.sh $1 $(basename $0)

./configure --prefix=/usr   \
            --enable-shared \
            --without-ensurepip

make
make install

. $DIST_ROOT/build-env/build-scripts/inc-end.sh $1 $(basename $0)