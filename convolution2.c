#include <stdio.h>
#include <stdlib.h>
#include<time.h>

int main(int argc, char *argv[]) {
    FILE *file1, *file2, *file3;
    int index1;
    int filter[5][5];
    int** data;
    int** result;
    int index2;
    int value;
    int iterations;

    if (argc < 5) {
    	printf("Usage: ./convolution2 data1.txt filter1.txt temp111 1\n");
    	return 1;
    }
    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");
    if ((file1 == NULL) || (file2 == NULL)) {
    	printf("Error: Files cannot open\n");
    	return 1;
    }
    iterations = atoi(argv[4]);
    if (iterations == 0) {
    	printf("Error: Invalid iteration\n");
    	return 1;
    } 
    file3 = fopen(argv[3], "w");
    data = (int**) malloc(sizeof(int*)*1024);
    result = (int**) malloc(sizeof(int*) * 1024);
    for (index1 = 0; index1 < 1024; index1++) {
        data[index1] = (int*) malloc(sizeof(int) * 1024);
        result[index1] = (int*) malloc(sizeof(int) * 1024);
    }

    index1 = 0;
    do {
        index2 = 0;
        do {
            if (index2 == 1023) {
                fscanf(file1, "%d\n", &value);
                data[index1][index2] = value;
//                printf("%d\n", data[index1][index2]);
            }
            else {
                fscanf(file1, "%d,", &value);
                data[index1][index2] = value;
//                printf("%d ", data[index1][index2]);
            }
            index2 = index2 + 1;
        } while (index2 < 1024);
        index1 = index1 + 1;
    } while (index1 < 1024);

    index1 = 0;
    do {
        int j = 0;
        do {
            if (j == 4) {
                fscanf(file2, "%d\n", &value);
                filter[index1][j] = value;
//                printf("%d\n", filter[index1][index2]);
            }
            else {
                fscanf(file2, "%d,", &value);
                filter[index1][j] = value;
//               printf("%d ", filter[index1][index2]);
            }
            j = j + 1;
        } while (j < 5);
        index1 = index1 + 1;
    } while (index1 < 5);

    for (int iteration = 0; iteration < iterations; ++iteration) {
        for (int row = 0; row < 1024; ++row) {
            for (int column = 0; column < 1024; ++column) {
                int sum = 0;
                for (int filter_row = 0; filter_row < 5; ++filter_row) {
                    for (int filter_column = 0;
                         filter_column < 5; ++filter_column) {
                        int row_index = row - filter_row + 2;
                        int column_index = column - filter_column + 2;
                        if (0 <= row_index && row_index < 1024
                            && 0 <= column_index && column_index < 1024) {
                            sum += data[row_index][column_index]
                                   * filter[filter_row][filter_column];
                        }
                    }
                }
                sum = sum / 16;
                if (sum > 16) {
                    result[row][column] = 16;
                }
                else if (sum >= -16) {
                    result[row][column] = sum;
                }
                else {
                    result[row][column] = -16;
                }
            }
        }
        for (int row = 0; row < 1024; ++row) {
            for (int column = 0; column < 1024; ++column) {
                data[row][column] = result[row][column];
            }
        }
    }

    for (index1 = 0; index1 < 1024; index1++) {
        for (index2 = 0; index2 < 1024; index2++) {
            fprintf(file3, "%d ", data[index1][index2]);
        }
        fprintf(file3, "\n");
    }

    for (index1 = 0; index1 < 1024; index1++) {
        free(data[index1]);
        free(result[index1]);
    }
    free(data);
	free(result);
    fclose(file1);
    fclose(file2);
    fclose(file3);
    return 0;
}
