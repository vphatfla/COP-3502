#!/bin/bash

# Compile the C program
gcc -o inventory inventory.c

# Check if compilation was successful
if [ $? -ne 0 ]; then
  echo "Compilation failed. Exiting."
  exit 1
fi

# Input and output file names
input_prefix="inventory_"
output_prefix="inventory_"
extension=".in"

# Number of test cases
num_tests=3

# Loop through test cases
for ((i=1; i<=$num_tests; i++)); do
  input_file="${input_prefix}$(printf "%02d" $i)${extension}"
  expected_output_file="${output_prefix}$(printf "%02d" $i).out"
  generated_output_file="${output_prefix}$(printf "%02d" $i)_generated.out"

  # Run the C program and redirect output to a file
  ./inventory < "$input_file" > "$generated_output_file"

  # Compare generated output with expected output
  if diff -w -q "$generated_output_file" "$expected_output_file" &> /dev/null; then
    echo "Test case $i: Passed"
  else
    echo "Test case $i: Failed"
  fi
done

# Cleanup
rm -f inventory
