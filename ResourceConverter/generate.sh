#!/bin/bash

#  generate.sh
#  AppleALC
#
#  Copyright © 2016 vit9696. All rights reserved.

ret=0

rm -f "${PROJECT_DIR}/AppleALC/kern_resources.cpp"

"${PROJECT_DIR}/DerivedData/${PROJECT}/Build/Products/${CONFIGURATION}/ResourceConverter" \
	"${PROJECT_DIR}/Resources" \
	"${PROJECT_DIR}/AppleALC/kern_resources.cpp" || ret=1

if (( $ret )); then
	echo "Failed to build kern_resources.cpp"
	exit 1
fi