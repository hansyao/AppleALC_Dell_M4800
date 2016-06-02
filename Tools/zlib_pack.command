#!/bin/sh

#  zlib_pack.command
#  
#
#  Created by Rodion Shingarev on 17/05/15.
#
MyPath=`dirname "$BASH_SOURCE"`
cd "$MyPath"

find ./Resources/   -name '*.xml' | while read file
do
echo "Compressing" $file
perl zlib.pl deflate "$file" > "$file.zlib"

done

