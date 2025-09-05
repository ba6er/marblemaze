#!/bin/sh

if [ $# -gt 1 ]
then
	echo "Too many arguments, expecting one"
	echo "Valid arguments are: build-release, build-debug, clean"
	exit 1
fi

case $1 in
	"build-release")
		cmake -S . -B build -DCMAKE_BUILD_TYPE="Release"
		cd build && make
		exit 0
		;;
	"build-debug")
		cmake -S . -B build -DCMAKE_BUILD_TYPE="Debug"
		cd build && make
		exit 0
		;;
	"")
		cmake -S . -B build -DCMAKE_BUILD_TYPE="Debug"
		cd build && make && ./marblemaze && cd ..
		exit 0
		;;
	"clean")
		echo "Cleaning the build directory"
		rm -rf ./build/
		exit 0
		;;
	*)
		echo "Valid arguments are: build-release, build-debug, clean"
		exit 1
		;;
esac
