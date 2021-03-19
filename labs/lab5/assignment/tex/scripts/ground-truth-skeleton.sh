for file in test_progs/*.s; do
	file=$(echo $file | cut -d'.' -f1)
	echo "Assembling $file"
	# How do you assemble a testcase?
	echo "Running $file"
	# How do you run a testcase?
	echo "Saving $file output"
	# How do you want to save the output?
	# What files do you want to save?
done
