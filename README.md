# NoNameLinux

Set /bin/sh to point to bash
```bash
sudo rm /bin/sh
sudo ln -s /bin/bash /bin/sh
```

To compile set the paths for DIST_ROOT and LFS
```bash
export DIST_ROOT=/home/name/repos/NoNameLinux # Path to the root
export LFS=$DIST_ROOT/build-env/build-root
```

Now Download the Packages and Compile
```bash
sudo -E ./prepare.sh
su distbuild -
./create.sh
```
