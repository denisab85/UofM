#!/bin/bash

read -p "Enter CACHE_SIZE: " CACHE_SIZE
read -p "Enter MAX_NUMBER: " MAX_NUMBER

echo "---------------------------------"
printf "Compiling regular version with CACHE_SIZE = $CACHE_SIZE\n"

cd ./fib_project/fib
clang -c -D CACHE_SIZE=$CACHE_SIZE ./*.c
clang -Wall -o ../../fib ./*.o
cd ../../

echo "---------------------------------"
printf "Running fib 1..$MAX_NUMBER in a loop:\n\n"

for ((n=1; n<=MAX_NUMBER; n++)); do
   ./fib $n
done

printf "\nTiming the largest number [fib $MAX_NUMBER]:\n"
time ./fib $MAX_NUMBER


printf "\n\n---------------------------------\nCompiling 'even' version with CACHE_SIZE = $CACHE_SIZE\n"
cd ./fib_project/fib
clang -c -D CACHE_SIZE=$CACHE_SIZE -D EVEN ./*.c
clang -Wall -o ../../fib ./*.o
cd ../../

echo "---------------------------------"
printf "Running fib 1..$MAX_NUMBER in a loop:\n\n"

for ((n=1; n<=MAX_NUMBER; n++)); do
   ./fib $n
done

printf "\nTiming the largest number [fib $MAX_NUMBER]:\n"
time ./fib $MAX_NUMBER
