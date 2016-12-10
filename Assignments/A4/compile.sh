function run()
{
	printf "\nTo compile and run with %s press ENTER\n" $1
	read

	cp -f $1 ./main.c

	# compile
	make clear
	make

	#run test
	./gc.bin
}

function handin
{
	mkdir ./handin
	cp -f ./GC/GC/* ./handin
	cp ./Makefile ./handin
	cp ./Readme.txt ./handin
}

handin

# update source files from Xcode project
cp -f ./GC/GC/* ./

run ./GC/GC/main.c

run ./A4Tests/main1.c

run ./A4Tests/main2.c

run ./A4Tests/main3.c

run ./A4Tests/main4.c

run ./A4Tests/main5.c

make clear

rm -f ./*.c ./*.h