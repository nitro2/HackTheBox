#!/bin/bash

# Check if an argument was provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 hexstring"
    exit 1
fi

# Convert the hex string to ASCII
echo -e $(echo $1 | sed 's/.\{2\}/\\x&/g')