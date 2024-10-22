#!/bin/bash

# Check if a file argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Initialize variables
filename=$1
line_count=0
loops_list=()

# Check if file exists
if [ ! -f "$filename" ]; then
    echo "File not found!"
    exit 1
fi

# Process each line in the file
while IFS= read -r line; do
    # Extract the loops number using grep and cut
    loops=$(echo "$line" | grep -oP '(?<=run loops: )\d+')

    # If loops number is found, add to the list
    if [ ! -z "$loops" ]; then
        line_count=$((line_count + 1))
        loops_list+=("$loops")
    fi
done < "$filename"

# Output the result: number of lines followed by loops, each on a new line
echo "$line_count"
for loop in "${loops_list[@]}"; do
    echo "$loop"
done
