#include <stdio.h>
#include <stdlib.h>
#include<time.h>

struct matrix {
    int original_vals;
    int new_vals;
};
typedef struct matrix Matrix;


int main(int argc, char *argv[]) {
    FILE *data_file, *filter_file, *output_file;
    int index = 0;
    int filter[5][5];
    Matrix** data;
    int index2;
    int values;
    int iterations;

    if (argc < 5) {
    	printf("Usage: ./convolution1 data1.txt filter1.txt temp111 1\n");
    	return 1;
    }
    data_file = fopen(argv[1], "r");
    filter_file = fopen(argv[2], "r");
    if ((data_file == NULL) || (filter_file == NULL)) {
    	printf("Error: Files cannot open\n");
    	return 1;
    }
    iterations = atoi(argv[4]);
    if (iterations == 0) {
    	printf("Error: Invalid iteration\n");
    	return 1;
    }
    output_file = fopen(argv[3], "w");
    data = (Matrix**) malloc(sizeof(Matrix*)*1024);
    for (index = 0; index < 1024; index++) {
        data[index] = (Matrix*) malloc(sizeof(Matrix) * 1024);
    }

    index = 0;
    do {
        index2 = 0;
        do {
            if (index2 == 1023) {
                fscanf(data_file, "%d\n", &values);
                data[index][index2].original_vals = values;
            }
            else {
                fscanf(data_file, "%d,", &values);
                data[index][index2].original_vals = values;
            }
            index2 = index2 + 1;
        } while (index2 < 1024);
        index = index + 1;
    } while (index < 1024);

    index = 0;
    do {
        index2 = 0;
        do {
            if (index2 == 4) {
                fscanf(filter_file, "%d\n", &values);
                filter[index][index2] = values;
            }
            else {
                fscanf(filter_file, "%d,", &values);
                filter[index][index2] = values;
            }
            index2 = index2 + 1;
        } while (index2 < 5);
        index = index + 1;
    } while (index < 5);

    for (int iteration = 0; iteration < iterations; ++iteration) {
        for (int row = 0; row < 1024; ++row) {
            for (int column = 0; column < 1024; ++column) {
                int sum = 0;
                for (int filter_row = 0; filter_row < 5; ++filter_row) {
                    for (int filter_column = 0; filter_column < 5;
                    ++filter_column)
                    {
                        int row_index = row - filter_row + 2;
                        int column_index = column - filter_column + 2;
                        if (0 <= row_index && row_index < 1024 && 0
                        <= column_index && column_index < 1024) {
                            sum += data[row_index][column_index].original_vals
                                   * filter[filter_row][filter_column];
                        }
                    }
                }
                sum = sum / 16; // Scaling
                // Saturation
                if (sum > 16) {
                    data[row][column].new_vals = 16;
                }
                else if (sum >= -16) {
                    data[row][column].new_vals = sum;
                }
                else {
                    data[row][column].new_vals = -16;
                }
            }
        }
        for (int row = 0; row < 1024; ++row) {
            for (int column = 0; column < 1024; ++column) {
                data[row][column].original_vals = data[row][column].new_vals;
            }
        }
    }

    for (index = 0; index < 1024; index++) {
        for (index2 = 0; index2 < 1024; index2++) {
            fprintf(output_file, "%d ",
                    data[index][index2].new_vals);
        }
        fprintf(output_file, "\n");
    }

    for (index = 0; index < 1024; index++) {
        free(data[index]);
    }
    free(data);
    fclose(data_file);
    fclose(filter_file);
    fclose(output_file);
    return 0;
}
