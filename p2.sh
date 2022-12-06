#!/bin/bash
make clean
make

printf "\nBUDDY ALLOCATE RUN:\n\n"
./proj2 0 sample_input/input_1
./proj2 0 sample_input/input_2
./proj2 0 sample_input/input_3
./proj2 0 sample_input/input_4
./proj2 0 sample_input/input_5
./proj2 0 sample_input/input_6
./proj2 0 sample_input/input_7
./proj2 0 sample_input/input_8
./proj2 0 sample_input/input_9
./proj2 0 sample_input/input_10
./proj2 0 sample_input/input_11
./proj2 0 sample_input/input_12

printf "\nBUDDY ALLOCATE OUTPUTS:\n\n"
diff -qs output/result-0-input_1 sample_output/result-0-input_1
diff -qs output/result-0-input_2 sample_output/result-0-input_2
diff -qs output/result-0-input_3 sample_output/result-0-input_3
diff -qs output/result-0-input_4 sample_output/result-0-input_4
diff -qs output/result-0-input_5 sample_output/result-0-input_5
diff -qs output/result-0-input_6 sample_output/result-0-input_6
diff -qs output/result-0-input_7 sample_output/result-0-input_7
diff -qs output/result-0-input_8 sample_output/result-0-input_8
diff -qs output/result-0-input_9 sample_output/result-0-input_9
diff -qs output/result-0-input_10 sample_output/result-0-input_10
diff -qs output/result-0-input_11 sample_output/result-0-input_11
diff -qs output/result-0-input_12 sample_output/result-0-input_12

printf "\nSLAB ALLOCATE RUN:\n\n"
./proj2 1 sample_input/input_1
./proj2 1 sample_input/input_2
./proj2 1 sample_input/input_3
./proj2 1 sample_input/input_4
./proj2 1 sample_input/input_5
./proj2 1 sample_input/input_6
./proj2 1 sample_input/input_7
./proj2 1 sample_input/input_8
./proj2 1 sample_input/input_9
./proj2 1 sample_input/input_10
./proj2 1 sample_input/input_11
./proj2 1 sample_input/input_12

printf "\nSLAB ALLOCATE OUTPUTS:\n\n"
diff -qs output/result-1-input_1 sample_output/result-1-input_1
diff -qs output/result-1-input_2 sample_output/result-1-input_2
diff -qs output/result-1-input_3 sample_output/result-1-input_3
diff -qs output/result-1-input_4 sample_output/result-1-input_4
diff -qs output/result-1-input_5 sample_output/result-1-input_5
diff -qs output/result-1-input_6 sample_output/result-1-input_6
diff -qs output/result-1-input_7 sample_output/result-1-input_7
diff -qs output/result-1-input_8 sample_output/result-1-input_8
diff -qs output/result-1-input_9 sample_output/result-1-input_9
diff -qs output/result-1-input_10 sample_output/result-1-input_10
diff -qs output/result-1-input_11 sample_output/result-1-input_11
diff -qs output/result-1-input_12 sample_output/result-1-input_12