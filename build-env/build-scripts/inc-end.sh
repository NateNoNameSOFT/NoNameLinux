FILE=$1

echo 
echo "Cleaning Up..."
echo

cd $LFS/sources
DIR=$(echo $FILE | awk -F"\\\\.t" '{print $1'})
rm -rf $DIR

echo 
echo "Done Processing $2"
echo 