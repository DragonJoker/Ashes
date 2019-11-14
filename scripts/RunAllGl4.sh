#!/usr/bin/sh

ASHES_DIR=$PWD

for file in $ASHES_DIR/Test-*
do
	echo "Running $file"
	$file -gl4 > $file-gl4.log
done

for file in $ASHES_DIR/Sample-*
do
	echo "Running $file"
	$file -gl4 > $file-gl4.log
done
