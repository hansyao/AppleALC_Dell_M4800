#!/bin/bash

# zlib_pack.command
# Usage: ./zlib_pack.command
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources -name '*.xml' | while read file
do
	echo "Compressing" $file
	perl Tools/zlib.pl deflate "$file" > "$file.zlib.tmp" || exit 1
	mv "$file.zlib.tmp" "$file.zlib" || exit 1

	if [ "$1" == "-d" ]; then
		rm -f "$file"
	fi
done

popd &>/dev/null
