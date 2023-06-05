#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct matrix {
    int original_vals;
    int new_vals;
};
typedef struct matrix Matrix; // Type alias for struct matrix type.


void read_data_from_file(FILE* file, Matrix** data);
void read_filter_from_file(FILE* file, int filter[5][5]);
void convolve(Matrix** data, int filter[5][5]);
void update_data_matrix(Matrix** data);
void output_data(FILE* file, Matrix** data);


int main(int argc, char* argv[]) {
    FILE* data_file, *filter_file, *output_file;
    int filter[5][5];
    Matrix** data;
    int iterations;

    if (argc < 5) { // Confirm that correct number of arguments passed.
        printf("Usage: ./convolution data1.txt filter1.txt temp111 1\n");
        return 1;
    }

    // Open the data, filter, and output files
    data_file = fopen(argv[1], "r");
    filter_file = fopen(argv[2], "r");
    output_file = fopen(argv[3], "w");
    if (data_file == NULL || filter_file == NULL) {
        printf("Error: Files cannot be opened\n");
        return 1;
    }

    // Confirm that inputted iterations is valid.
    iterations = atoi(argv[4]);
    if (iterations <= 0) {
        printf("Error: Invalid number of iterations\n");
        return 1;
    }

    // Allocate memory for data matrix.
    data = (Matrix**)malloc(sizeof(Matrix*) * 1024);
    for (int i = 0; i < 1024; i++) {
        data[i] = (Matrix*)malloc(sizeof(Matrix) * 1024);
    }

    // Read data from data and filter file
    read_data_from_file(data_file, data);
    read_filter_from_file(filter_file, filter);

    // Perform 2D convolution operation, and scale and saturate values.
    for (int iteration = 0; iteration < iterations; ++iteration) {
        convolve(data, filter);
        update_data_matrix(data);
    }

    // Output data to output file
    output_data(output_file, data);

    // Free up used memory
    for (int i = 0; i < 1024; i++) {
        free(data[i]);
    }
    free(data);
    // Close files
    fclose(data_file);
    fclose(filter_file);
    fclose(output_file);
    return 0;
}


/* This function reads the values from the data file and inserts them
 * into their corresponding data array positions. */
void read_data_from_file(FILE* file, Matrix** data) {
    int index = 0;
    int index2;
    int values;

    do {
        index2 = 0;
        do {
            if (index2 == 1023) {
                fscanf(file, "%d\n", &values);
                data[index][index2].original_vals = values;
            } else {
                fscanf(file, "%d,", &values);
                data[index][index2].original_vals = values;
            }
            index2 = index2 + 1;
        } while (index2 < 1024);
        index = index + 1;
    } while (index < 1024);
}


/* This function reads the values from the filter file and insets them
 * into their corresponding data array positions. */
void read_filter_from_file(FILE* file, int filter[5][5]) {
    int index = 0;
    int index2;
    int values;

    do {
        index2 = 0;
        do {
            if (index2 == 4) {
                fscanf(file, "%d\n", &values);
                filter[index][index2] = values;
            } else {
                fscanf(file, "%d,", &values);
                filter[index][index2] = values;
            }
            index2 = index2 + 1;
        } while (index2 < 5);
        index = index + 1;
    } while (index < 5);
}


// This function applies the two-dimensional convolve2 operation
void convolve(Matrix** data, int filter[5][5]) {
    int row, column, filter_row, filter_column;
    int row_index, column_index;
    int sum;

    for (row = 0; row < 1024; ++row) {
        for (column = 0; column < 1024; ++column) {
            sum = 0;
            for (filter_row = 0; filter_row < 5; ++filter_row) {
                for (filter_column = 0; filter_column < 5; ++filter_column) {
                    row_index = row - filter_row + 2;
                    column_index = column - filter_column + 2;
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
            } else if (sum >= -16) {
                data[row][column].new_vals = sum;
            } else {
                data[row][column].new_vals = -16;
            }
        }
    }
}


/* This function copies the values from the temporary result array
 * to the data array */
void update_data_matrix(Matrix** data) {
    int row, column;

    for (row = 0; row < 1024; ++row) {
        for (column = 0; column < 1024; ++column) {
            data[row][column].original_vals = data[row][column].new_vals;
        }
    }
}


/* This function outputs the data array to a file
 * as a space separated list of values */
void output_data(FILE* file, Matrix** data) {
    int index, index2;

    for (index = 0; index < 1024; index++) {
        for (index2 = 0; index2 < 1024; index2++) {
            fprintf(file, "%d ", data[index][index2].new_vals);
        }
        fprintf(file, "\n");
    }
}
