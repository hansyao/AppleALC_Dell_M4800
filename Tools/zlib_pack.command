#!/bin/bash

# zlib_pack.command
# Usage: ./zlib_pack.command
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources/ -name '*.xml' | while read file
do
	echo "Compressing" $file
	perl Tools/zlib.pl deflate "$file" > "$file.zlib"

	if [ "$1" == "-d" ]; then
		rm "$file"
	fi
done

popd &>/dev/null
