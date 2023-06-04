#include <stdio.h>
#include <stdlib.h>
#include<time.h>

struct matrix {
    int o_val;
    int n_val;
};
typedef struct matrix Matrix;


int main(int argc, char *argv[]) {
    FILE *file1, *file2, *file3;
    int i = 0;
    int filter[5][5];
    Matrix** data;
    int j, k, l, m;
    int val;
    int iter;

    if (argc < 5) {
    	printf("Usage: ./convolution1 data1.txt filter1.txt temp111 1\n");
    	return 1;
    }
    file1 = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");
    if ((file1 == NULL) || (file2 == NULL)) {
    	printf("Error: Files cannot open\n");
    	return 1;
    }
    iter = atoi(argv[4]);
    if (iter == 0) {
    	printf("Error: Invalid iteration\n");
    	return 1;
    }  
    file3 = fopen(argv[3], "w");
    data = (Matrix**) malloc(sizeof(Matrix*)*1024);
    for (i = 0; i < 1024; i++) {
        data[i] = (Matrix*) malloc(sizeof(Matrix)*1024);
    }


    i = 0;
    do {
        j = 0;
        do {
            if (j == 1023) {
                fscanf(file1, "%d\n", &val);
                data[i][j].o_val = val;
//                printf("%d\n", data[i][j]);
            }
            else {
                fscanf(file1, "%d,", &val);
                data[i][j].o_val = val;
//                printf("%d ", data[i][j]);
            }
            j = j + 1;
        } while (j < 1024);
        i = i + 1;
    } while (i < 1024);

    i = 0;
    do {
        int j = 0;
        do {
            if (j == 4) {
                fscanf(file2, "%d\n", &val);
                filter[i][j] = val;
//                printf("%d\n", filter[i][j]);
            }
            else {
                fscanf(file2, "%d,", &val);
                filter[i][j] = val;
//               printf("%d ", filter[i][j]);
            }
            j = j + 1;
        } while (j < 5);
        i = i + 1;
    } while (i < 5);




// Your code is here


    for (i = 0; i < 1024; i++) {
        for (j = 0; j < 1024; j++) {
            fprintf(file3, "%d ", data[i][j].n_val);
        }
        fprintf(file3, "\n");
    }

    for (i = 0; i < 1024; i++) {
        free(data[i]);
    }
    free(data);
    fclose(file1);
    fclose(file2);
    fclose(file3);
    return 0;
}
