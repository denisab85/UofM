cp -f ./life/life/life.c ./
clang -Wall -o ./life_bin ./life.c
./life_bin ./life.txt > life_out.txt
diff ./life_out.txt ./lifeOut.txt