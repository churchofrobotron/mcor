#!/bin/bash

while [ "1" == "1" ] ; do
	echo converting
	mogrify -format png -colorspace gray -colors 16 -normalize work/*.jpeg
done

