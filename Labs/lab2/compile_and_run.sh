echo "
//-----------------------------------------
// NAME: Denis Abakumov
// STUDENT NUMBER: 7823110
// COURSE: COMP 2160, SECTION: A02
// INSTRUCTOR: Michael Zapp
// ASSIGNMENT: Lab 2
//
// REMARKS: mail merge
//
//-----------------------------------------
"
read -n1 -r -p "Press any key to compile merge-start.c"
clang ./merge-start.c -o merge-start

echo

read -n1 -r -p "Press any key to run merge-start w/o passing parameters"
./merge-start

echo "
Press any key to run merge-start with parameters:
./merge-start template.txt data.txt"
read -n1 -r
./merge-start template.txt data.txt

read -n1 -r -p "Press any key to list merge-start.c"
nano merge-start.c
