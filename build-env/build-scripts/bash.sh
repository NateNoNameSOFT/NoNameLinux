. $DIST_ROOT/build-env/build-scripts/inc-start.sh $1 $(basename $0)

./configure --prefix=/usr                   \
            --build=$(support/config.guess) \
            --host=$LFS_TGT                 \
            --without-bash-malloc

make

make DESTDIR=$LFS install
mv $LFS/usr/bin/bash $LFS/bin/bash
ln -sv bash $LFS/bin/sh

. $DIST_ROOT/build-env/build-scripts/inc-end.sh $1 $(basename $0)