#!/bin/sh

#  zlib_unpack.command
#  
#
#  Created by Rodion Shingarev on 17/05/15.
#
MyPath=`dirname "$BASH_SOURCE"`
cd "$MyPath"

find ./Resources/   -name '*.xml.zlib' | while read file
do
echo "Decompressing" $file
perl zlib.pl inflate "$file" > "${file%.*}"


done

