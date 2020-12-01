To compile an exe file use the following command:
gcc --std=gnu99 -pthread main.c -o line_processor

Then to run the comand run either of these:
./line_processor
./line_processor < input1.txt
./line_processor < input1.txt > output1.txt