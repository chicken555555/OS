 #include<pthread.h>
 #include<sys/types.h>
 #include<stdlib.h>
 #include<unistd.h>
 #include<math.h>
 #include<stdio.h>
 
int a[4][4], b[4][4];

void *matrixeval(void *val) {
    int row = *((int *)val); // Extract the row number from the passed value

    // Copy the elements from the original matrix 'a' to matrix 'b'
    for (int i = 0; i < 4; i++) {
        b[row][i] = a[row][i];
    }

    // Perform element-wise square operation
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < row; j++) {
            b[row][i] *= b[row][i];
        }
    }
    
    pthread_exit(NULL); // Exit the thread
}

int main() {
    pthread_t tid[4];
    int i, j;

    // Input matrix 'a'
    for (i = 0; i < 4; i++) {
        printf("Enter the elements of row %d: ", i + 1);
        for (j = 0; j < 4; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    // Print matrix 'a' before processing
    printf("Before processing:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    // Create threads
    for (i = 0; i < 4; i++) {
        pthread_create(&tid[i], NULL, matrixeval, (void *)&i);
        // There was a sleep(1) here, but I removed it because it could cause timing issues
    }

    // Join threads
    for (i = 0; i < 4; i++) {
        pthread_join(tid[i], NULL);
    }

    // Print matrix 'b' after processing
    printf("After processing:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

    return 0;
}