INPUT=$1

	rm -f ./life_release* ./life_debug*
	
	# compile RELEASE
	clang -Wall -o ./life_release -DNDEBUG ./life/life/life.c
	rm -f ./life_release.txt
	./life_release < $INPUT &> life_release.txt
	
	# compile DEBUG
	# clang -Wall -o ./life_debug ./life/life/life.c
	# rm -f ./life_debug.txt
	# ./life_debug < $INPUT &> life_debug.txt
	
	
	
	# compile sample solution RELEASE
	clang -Wall -o ./life_sample_release -DNDEBUG ./sample_solution/life.c
	rm -f ./life_sample_release.txt
	./life_sample_release < $INPUT &> life_sample_release.txt
	
	# compile sample solution DEBUG
	clang -Wall -o ./life_sample_debug ./sample_solution/life.c
	rm -f ./life_sample_debug.txt
	./life_sample_debug < $INPUT &> life_sample_debug.txt
	
	#cat ./life_release.txt
	#diff ./life_release.txt ./lifeOut.txt
	