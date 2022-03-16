set -e

echo "Dist Root: ${DIST_ROOT:?}"
echo "LFS: ${LFS:?}"

if ! test $(whoami) == "distbuild" ; then
	echo "Must run as distbuild!"
	exit -1
fi

echo "Creating build enviorment..."
cd $DIST_ROOT/build-env

#bash -e build-scripts/binutils-pass-1.sh binutils-2.36.1.tar.xz
#bash -e build-scripts/gcc-pass-1.sh gcc-10.2.0.tar.xz
#bash -e build-scripts/linux-headers.sh linux-5.10.17.tar.xz
#bash -e build-scripts/glibc.sh glibc-2.33.tar.xz
#bash -e build-scripts/libstdcpp-pass-1.sh gcc-10.2.0.tar.xz
#bash -e build-scripts/m4.sh m4-1.4.18.tar.xz
#bash -e build-scripts/ncurses.sh ncurses-6.2.tar.gz
#bash -e build-scripts/bash.sh bash-5.1.tar.gz
#bash -e build-scripts/coreutils.sh coreutils-8.32.tar.xz
#bash -e build-scripts/diffutils.sh diffutils-3.7.tar.xz
#bash -e build-scripts/file.sh file-5.39.tar.gz
#bash -e build-scripts/findutils.sh findutils-4.8.0.tar.xz
#bash -e build-scripts/gawk.sh gawk-5.1.0.tar.xz
#bash -e build-scripts/grep.sh grep-3.6.tar.xz
#bash -e build-scripts/gzip.sh gzip-1.10.tar.xz
#bash -e build-scripts/make.sh make-4.3.tar.gz
#bash -e build-scripts/patch.sh patch-2.7.6.tar.xz
#bash -e build-scripts/sed.sh sed-4.8.tar.xz
#bash -e build-scripts/tar.sh tar-1.34.tar.xz
#bash -e build-scripts/xz.sh xz-5.2.5.tar.xz
#bash -e build-scripts/binutils-pass-2.sh binutils-2.36.1.tar.xz
#bash -e build-scripts/gcc-pass-2.sh gcc-10.2.0.tar.xz

sudo -E build-scripts/build-chroot.sh

echo "DONE!"
