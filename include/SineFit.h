/*
 * SineFit.h
 *
 *      Date: Sep 16, 2020
 *      Author: Arash Roshanineshat
 */
#ifndef __SINEFIT__
#define __SINEFIT__

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define F_Resolution 10000 // Hz
#define GridDS 100
#define PI M_PI

typedef struct Matrix_st{
    int row;
    int col;
    float **Data;
} Mat;

void cleanup(Mat **);

Mat* GenerateFrequencyGrid(float, int);
Mat* Transpose(Mat*);
Mat* MultiplyMatrices (Mat*, Mat*);
Mat* InverseMatrix(Mat * matrix);
Mat* DotMultiMatrices (Mat*, Mat*);

void printMatrix(Mat * matrix);
float MatrixDet(Mat*);

// Functions to be called by user
/*
   datacount: Number of Samples
   CWFreq: 
       ThreeFitData: Known Frequency
       FourParamFir: Initial Guessed Frequency
   data_arr: Pointer to the data array
   FS: Sampling Frequency
*/
Mat* ThreeFitData (int datacount, float CWFreq, float* data_arr, float Fs);
Mat* FourParamFit(int datacount, float CWFreq, float* data_arr, float Fs);

#endif /* __SINEFIT__ */
