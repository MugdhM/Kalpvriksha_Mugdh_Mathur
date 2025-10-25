#include<stdio.h>
#include<stdlib.h>
#include<time.h>



void swap(int *a, int *b) {
    *a += *b;
    *b = *a - *b;
    *a -= *b;
}

void reverseArray(int *array,int size) {
    for(int index = 0; index < size/2; index++) {
        swap(array + index, array + size - 1 - index);
    }
}

void transposeMatrix(int *matrix, int size) { 
    for(int row = 0; row < size; row++) {
        for(int column = row + 1; column < size; column++) {
            swap(matrix + row * size + column , matrix + column * size + row);          //replacing matrix[i][j] with matrix[j][i];
        }
    }
}
//            current Issue
//Issue 1: though it was mentioned in the task that the rotation shold only be done with swapping addresses and not by swapping values but that is not possible since we got only one single pointer
// if I were to create the matrix as array of pointer to arrays of int it would cause it to addition memory to store N pointers which is quite not withing the constraints and also swapping addresses in pointer array would result in swapping rows and not the swaaping of rows with column which is required in the 90 degree clockwise rotation of matrix
// therefore I have used pointer arithmatics to address the task of rotating the array


void rotateMatrix(int *matrix,int size) {

    transposeMatrix(matrix,size);                              //to rotate matrix by 90 degree Step 1: transpose

    for(int row = 0; row < size; row++) {                       //Step 2:  reverse every array 
        reverseArray(matrix + row * size,size);                 //passing base address by pointer arithematics
    }   

}


void printMatrix(int *matrix,int size) {
    printf("\n");
    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            printf(" %5d ", *(matrix + row * size + column));     // printing matrix with pointer Arithematics
        }
        printf("\n");
    }
}


//unfinished function 


//Issue 2: 
//by using a singl block of memory for matrix the and only able to use pointer arithematics to access the values in matrix manner below code becomes much more complex from which it already is
// if it is not acceptable, I'll have to move to the other implementation which uses matrix as array of pointer to arrays
//

void applySmootheningFilterOnMatrix(int *matrix,int size) {       //using pointer arithematics instead of indexing

    int previousRow[size];
    int previousValue = 0;
    int currentValue = 0;
    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            currentValue = *(matrix + row * size + column);
            if(row == 0)                                              //top row
            {

                //update in previous row at top row
                previousRow[column] = *(matrix + row * size + column);  //matrix[row][column];

                if(column == 0)       //Top-Left Corner
                {
                    *(matrix + row * size + column) += *(matrix + row * size + column + 1) + *(matrix + (row + 1)*size + column) + *(matrix + (row + 1)*size + column+1);
                    *(matrix + row * size + column) /= 4;

                    // this can be understood as arr[i][j] = (arr[i][j] + arr[i][j+1] + arr[i+1][j] + arr[i+1][j+1])/4

                    
                }
                else if(column == size - 1)  
                {
                    *(matrix + row * size + column) += previousValue + *(matrix + (row + 1)*size + column) + *(matrix + (row + 1)*size + column-1);
                    *(matrix + row * size + column) /= 4;
                }
                else {
                    //ie  matrix[i][j]              =     matrix[i][j-1] +       matrix[i+1][j-1]                +    matrix[i+1][j]                    +       matrix[i][j+1]               +         matrix[i+1][j+1]
                    *(matrix + row * size + column) +=  previousValue + *(matrix + (row + 1)*size + column-1) +  *(matrix + (row + 1)*size + column) + *(matrix + row * size + column + 1) + *(matrix + (row + 1)*size + column+1);
                    *(matrix + row * size + column) /= 6;
                }
            } 
            else if(row == size - 1)                                  //bottom row
            {
                if(column == 0)       
                {
                    *(matrix + row * size + column) += previousRow[column] + previousRow[column + 1] + *(matrix + row * size + column + 1) ;
                    *(matrix + row * size + column) /= 4;
                }
                else if(column == size - 1)  
                {
                    *(matrix + row * size + column) +=previousValue +  previousRow[column - 1] + previousRow[column];
                    *(matrix + row * size + column) /= 4;
                }
                else {
                    *(matrix + row * size + column) += previousValue +  previousRow[column - 1] + previousRow[column] + previousRow[column + 1] +  *(matrix + row * size + column + 1);
                    *(matrix + row * size + column) /= 6;
                }
            }
            else                                                      //middle rows
            {
                if(column == 0)       
                {
                    *(matrix + row * size + column) += previousRow[column] + previousRow[column + 1] + *(matrix + row * size + column + 1) + *(matrix + (row + 1) * size + column) + *(matrix + (row + 1) * size + column+1);
                    *(matrix + row * size + column) /= 6;

                }   
                else if(column == size - 1)  
                {
                    *(matrix + row * size + column) += previousValue + previousRow[column - 1] + previousRow[column] + *(matrix + (row + 1) * size + column - 1) + *(matrix + (row + 1) * size + column);
                    *(matrix + row * size + column) /=6;
                    

                    //updation in previous row
                    previousRow[column - 1] = previousValue;
                    previousRow[column] = currentValue;

                }
                else {
                    // this lines boils down to  arr[i][j] = (arr[i][j] + arr[i][j-1] + arr[i][j+1] + arr[i-1][j-1] + arr[i-1][j] + arr[i-1][j+1] + arr[i+1][j-1] + arr[i+1][j] + arr[i+1][j+1] ) /9 ;

                    *(matrix + row * size + column) += previousValue +  previousRow[column - 1] + previousRow[column] + previousRow[column + 1] +  *(matrix + row * size + column+1) +  *(matrix + (row + 1) * size + column - 1) + *(matrix + (row + 1) * size + column) + *(matrix + (row + 1) * size + column+1);
                    *(matrix + row * size + column) /= 9;

                    //updation in previous Row 
                    previousRow[column - 1] = previousValue;
                }
            }

            previousValue = currentValue;
        }
    }
}

int* generateMatrixWithRandomValues(int size) {       // this function allocates continuous block of memory of size of N x N for integers

    int *matrix = (int *)malloc(size * size * sizeof(int));       //dynamic memory allocation for matrix N x N

    for(int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++) {
            *(matrix + row * size + column) = rand() % 256;         // filling each cell with random value between 0 to 255 inclusive
        }
    }

    return matrix;

}

int main()  {
    
    int size = 0;                            // size of Square Matrix N x N
    int *matrix = NULL;
    srand(time(NULL));                        //passing current time as seed for random numbers

    printf("Enter size of the image: ");
    scanf("%d",&size);


    matrix = generateMatrixWithRandomValues(size);

    printf("\n\n Original Matrix :- \n");
    printMatrix(matrix,size);

    rotateMatrix(matrix,size);

    printf("\n\n Rotated Matrix :- \n");
    printMatrix(matrix,size);

    applySmootheningFilterOnMatrix(matrix,size);

    printf("\n\n Matrix after applying 3x3 Smoothening Filter :- \n");
    printMatrix(matrix,size);

    return 0;
}
