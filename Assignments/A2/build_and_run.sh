rm ./life_out.txt
rm ./life_bin

cp -f ./life/life/life.c ./
clang -Wall -o ./life_bin -DNDEBUG ./life.c
rm ./life_out.txt
./life_bin < ./life.txt &> life_out.txt
cat ./life_out.txt
#diff ./life_out.txt ./lifeOut.txt