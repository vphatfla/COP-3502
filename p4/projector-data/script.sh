ompile the C program
gcc projector.c -o projector -lm

# Check if compilation was successful
if [ $? -ne 0 ]; then
	    echo "Compilation failed. Exiting."
	        exit 1
fi

# Loop through input files
for i in {01..10}; do
	    # Run the program with input file
	        ./projector < $i.in > tmp_output.txt
		    
		    # Compare the output with the provided output, ignoring white space
		        if diff -w tmp_output.txt $i.out >/dev/null; then
				        echo "Test case $i: Passed"
					    else
						            echo "Test case $i: Failed"
							        fi

								    # Clean up temporary output file
								        rm tmp_output.txt
								done

								# Clean up compiled program
								rm projector


