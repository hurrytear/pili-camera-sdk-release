#/bin/sh

if test -d build.linux ;
then rm -rf build.linux
fi
mkdir build.linux

cd build.linux
cmake .. && make

cp ../fengtimo.flv .
cp ../start.sh .

