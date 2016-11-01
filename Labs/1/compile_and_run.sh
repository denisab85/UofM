clang ./exercise1.c -o exercise1
clang ./sign.c -o sign
clang ./squash.c -o squash

echo
read -n1 -r -p "Press any key to test against words.txt (a shorter dictionary)"
./sign < ./words.txt | sort | ./squash

echo
read -n1 -r -p "Press any key to test against /usr/share/dict/words (a longer dictionary)"
./sign < /usr/share/dict/words | sort | ./squash
