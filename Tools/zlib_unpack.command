#!/bin/bash

# zlib_unpack.command
# Usage: ./zlib_unpack.command [-d]
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources/ -name '*.xml.zlib' | while read file
do
	echo "Decompressing" $file
	perl Tools/zlib.pl inflate "$file" > "${file%.*}"
	# Enforce human readable form
	plutil -convert xml1 "${file%.*}"
done

popd &>/dev/null
