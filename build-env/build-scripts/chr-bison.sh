. $DIST_ROOT/build-env/build-scripts/inc-start.sh $1 $(basename $0)

./configure --prefix=/usr \
            --docdir=/usr/share/doc/bison-3.7.5

make
make install

. $DIST_ROOT/build-env/build-scripts/inc-end.sh $1 $(basename $0)