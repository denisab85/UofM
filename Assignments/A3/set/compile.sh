function run()
{
	# compile
	clang -c main.c -I.
	clang -o main main.o set.o

	#run test
	./main ./sequences.txt
}

# update source files from Xcode project
cp -f /Volumes/Data/UofM/COMP-2160/Assignments/A3/set_project/set_project/* ./


unzip -o set-trial1.zip
run
unzip -o set-trial2.zip
run
unzip -o set-trial3.zip
run
unzip -o set-trial4.zip
run
unzip -o set-trial6.zip
run
unzip -o set-trial7.zip
run
unzip -o set-trial5.zip
run
