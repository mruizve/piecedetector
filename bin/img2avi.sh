#!/usr/bin/env bash

if [ "$#" -ne 2 ]; then
	echo "usage: $0 images output_video"
	exit 0
fi

ffmpeg -i $1 -c:v huffyuv $2

exit 0

