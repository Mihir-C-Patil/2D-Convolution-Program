#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void read_data_from_file(const char* filename, int** data);
void read_filter_from_file(const char* filename, int filter[5][5]);
void convolve(int** data, int** result, const int filter[5][5]);
void update_data_array(int** data, int** result);
void output_data(const char* filename, const int** data);

int main(int argc, char *argv[]) {
    FILE* data_file, *filter_file, *output_file;
    int index1, index2, value, iterations;
    int** data;
    int** result;
    int filter[5][5];

    if (argc < 5) { // Confirm that correct number of arguments passed.
        printf("Usage: ./convolution2 data1.txt filter1.txt temp111 1\n");
        return 1;
    }

    // Open data, filter and output files.
    data_file = fopen(argv[1], "r");
    filter_file = fopen(argv[2], "r");
    output_file = fopen(argv[3], "w");
    if (data_file == NULL || filter_file == NULL) {
        printf("Error: Files cannot open\n");
        return 1;
    }

    // Confirm that inputted iterations is valid.
    iterations = atoi(argv[4]);
    if (iterations <= 0) {
        printf("Error: Invalid iteration\n");
        return 1;
    }

    // Allocate memory for data and result arrays.
    data = (int**) malloc(sizeof(int*) * 1024);
    result = (int**) malloc(sizeof(int*) * 1024);
    for (index1 = 0; index1 < 1024; index1++) {
        data[index1] = (int*) malloc(sizeof(int) * 1024);
        result[index1] = (int*) malloc(sizeof(int) * 1024);
    }

    // Read data from data and input file
    read_data_from_file(argv[1], data);
    read_filter_from_file(argv[2], filter);

    // Perform 2D convolution operation, and scale and saturate values.
    for (int iteration = 0; iteration < iterations; ++iteration) {
        convolve(data, result, filter);
        update_data_array(data, result);
    }

    // Output the data to the output file
    output_data(argv[3], data);

    // Free up used memory
    for (index1 = 0; index1 < 1024; index1++) {
        free(data[index1]);
        free(result[index1]);
    }
    free(data);
    free(result);
    fclose(data_file);
    fclose(filter_file);
    fclose(output_file);
    return 0;
}


/* This function reads the values from the data file and inserts them
 * into their corresponding data array positions. */
void read_data_from_file(const char* filename, int** data) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }
    for (int row = 0; row < 1024; ++row) {
        for (int column = 0; column < 1024; ++column) {
            if (column == 1023)
                fscanf(file, "%d\n", &data[row][column]);
            else
                fscanf(file, "%d,", &data[row][column]);
        }
    }
    fclose(file);
}


/* This function reads the values from the filter file and insets them
 * into their corresponding data array positions. */
void read_filter_from_file(const char* filename, int filter[5][5]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }
    for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++column) {
            if (column == 4)
                fscanf(file, "%d\n", &filter[row][column]);
            else
                fscanf(file, "%d,", &filter[row][column]);
        }
    }
    fclose(file);
}


// This function applies the two-dimensional convolve operation
void convolve(int** data, int** result, const int filter[5][5]) {
    for (int row = 0; row < 1024; ++row) {
        for (int column = 0; column < 1024; ++column) {
            int sum = 0;
            for (int filter_row = 0; filter_row < 5; ++filter_row) {
                for (int filter_column = 0; filter_column < 5; ++filter_column)
                {
                    int row_index = row - filter_row + 2;
                    int column_index = column - filter_column + 2;
                    if (0 <= row_index && row_index < 1024 && 0
                    <= column_index && column_index < 1024) {
                        sum += data[row_index][column_index]
                                * filter[filter_row][filter_column];
                    }
                }
            }
            sum = sum / 16; // Scaling
            // Saturation
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
}


/* This function copies the values from the temporary result array
 * to the data array */
void update_data_array(int** data, int** result) {
    for (int row = 0; row < 1024; ++row) {
        for (int column = 0; column < 1024; ++column) {
            data[row][column] = result[row][column];
        }
    }
}


/* This function outputs the data array to a file
 * as a space separated list of values */
void output_data(const char* filename, const int** data) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        return;
    }
    for (int row = 0; row < 1024; ++row) {
        for (int column = 0; column < 1024; ++column) {
            fprintf(file, "%d ", data[row][column]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}