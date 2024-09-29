#include <stdio.h>

int main() {
    // set image size 
    int Y_LEN = 1000;
    int X_LEN = 1000;

    // initialize image file header
    printf("P3\n");
    printf("1000 1000\n");
    printf("255\n");

    // write to image
    for (int y = 0; y < Y_LEN; y++) {
        for (int x = 0; x < X_LEN; x++) {
            printf("%d %d %d ", 255, 0, 0);
        }
    }
    printf("\n");

}