#!/bin/bash
#09-09-24  Michele Tavella <michele.tavella@epfl.ch>

if [ "$2" = "" ]; then
	echo "Error: edit.sh EDITOR_LIB EDITOR_EXAMPLES"
	exit;
fi

$1 src/*/*.cpp src/*/*.hpp 
$2 examples/*.cpp
