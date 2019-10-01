#!/usr/bin/sh

ASHES_DIR=$PWD

for file in $ASHES_DIR/Test-*
do
	echo "Running $file"
	$file -vk > $file-vk.log
done

for file in $ASHES_DIR/Sample-*
do
	echo "Running $file"
	$file -vk > $file-vk.log
done
