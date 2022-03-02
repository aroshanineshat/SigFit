#include <SineFit.h>

void cleanup(Mat ** var){
    for (int row = 0; row < (*var)->row; row++){
        free ((*var)->Data[row]);
    }
    free ((*var)->Data);
    free (*var);
    *var = NULL;
}


Mat * DotMultiMatrices (Mat* Mat1, Mat* Mat2){

    Mat *result;
    result = (Mat*)malloc(sizeof(Mat));

    result->row = Mat1->row;
    result->col = Mat1->col;

    result->Data = (float **)malloc (result->row * sizeof(float*));
    for (int row = 0; row < result->row; row++){
        result->Data[row] = (float *)calloc(result->col, sizeof(float));
    }

    for (int c_row = 0 ; c_row < result->row; c_row++){
        for (int c_col = 0; c_col < result->col ; c_col ++){
            result->Data[c_row][c_col] = Mat1->Data[c_row][c_col] * Mat2->Data[c_row][c_col];
        }
    }

    return result;

}


float MatrixDet(Mat* matrix){
    float result = 0;
    if (matrix->row == 2 && matrix->col == 2){
        float mSlope = 1;
        float sSlope = 1;
        for (int c_row = 0; c_row < matrix->row; c_row++){
            for (int c_col = 0 ; c_col < matrix->col; c_col++){
                if (c_row == c_col){
                    mSlope = mSlope * matrix->Data[c_row][c_col];
                }
                else {
                    sSlope = sSlope * matrix->Data[c_row][c_col];
                }
            }
        }
        result = mSlope - sSlope;
    }
    else if (matrix->row == 3 && matrix->col == 3){

        Mat DeterminentCoeffs;
        DeterminentCoeffs.row = 1;
        DeterminentCoeffs.col = 3;

        DeterminentCoeffs.Data = (float **)malloc (DeterminentCoeffs.row * sizeof(float*));
        for (int row = 0; row < DeterminentCoeffs.row; row++){
            DeterminentCoeffs.Data[row] = (float *)calloc(DeterminentCoeffs.col, sizeof(float));
        }
        DeterminentCoeffs.Data[0][0] = 1;
        DeterminentCoeffs.Data[0][1] = -1;
        DeterminentCoeffs.Data[0][2] = 1;

        Mat SubMat;
        SubMat.row = 2;
        SubMat.col = 2;

        SubMat.Data = (float **)malloc (SubMat.row * sizeof(float*));
        for (int row = 0; row < SubMat.row; row++){
            SubMat.Data[row] = (float *)calloc(SubMat.col, sizeof(float));
        }


        for (int c_col = 0; c_col < 3; c_col++){
            for (int c_row = 0; c_row < 3; c_row ++){
                if (c_row == 0){
                    continue;
                }
                int c_col_index = 0;
                for (int sub_mat_col =0; sub_mat_col < 3; sub_mat_col++){
                    if (sub_mat_col == c_col) continue;
                    SubMat.Data[c_row - 1][c_col_index] = matrix->Data[c_row][sub_mat_col];
                    c_col_index ++;
                }
            }
            result = result + DeterminentCoeffs.Data[0][c_col] * matrix->Data[0][c_col] * MatrixDet(&SubMat);

        }

        //colleting memory garbage
        for (int row = 0; row < SubMat.row; row++){
            free (SubMat.Data[row]);
        }
        free (SubMat.Data);

        for (int row = 0; row < DeterminentCoeffs.row; row++){
            free (DeterminentCoeffs.Data[row]);
        }
        free (DeterminentCoeffs.Data);
    }
    else{
        result = 0; // There is no Determinent
    }
    return result;
}

Mat * InverseMatrix(Mat * matrix){

    Mat *result;
    result = (Mat*)malloc(sizeof(Mat));

    result->row = matrix->row;
    result->col = matrix->col;

    result->Data = (float **)malloc (result->row * sizeof(float*));
    for (int row = 0; row < result->row; row++){
        result->Data[row] = (float *)calloc(result->col, sizeof(float));
    }

    Mat *CofactorMatrixCoeffs;
    CofactorMatrixCoeffs = (Mat*)malloc(sizeof(Mat));

    CofactorMatrixCoeffs->row = 3;
    CofactorMatrixCoeffs->col = 3;
    CofactorMatrixCoeffs->Data = (float **)malloc (CofactorMatrixCoeffs->row * sizeof(float*));
    for (int row = 0; row < CofactorMatrixCoeffs->row; row++){
        CofactorMatrixCoeffs->Data[row] = (float *)calloc(CofactorMatrixCoeffs->col, sizeof(float));
    }
    CofactorMatrixCoeffs->Data[0][0] = +1;
    CofactorMatrixCoeffs->Data[0][1] = -1;
    CofactorMatrixCoeffs->Data[0][2] = +1;

    CofactorMatrixCoeffs->Data[1][0] = -1;
    CofactorMatrixCoeffs->Data[1][1] = +1;
    CofactorMatrixCoeffs->Data[1][2] = -1;

    CofactorMatrixCoeffs->Data[2][0] = +1;
    CofactorMatrixCoeffs->Data[2][1] = -1;
    CofactorMatrixCoeffs->Data[2][2] = +1;

    Mat *SubMat;
    SubMat = (Mat*)malloc(sizeof(Mat));
    SubMat->row = 2;
    SubMat->col = 2;

    SubMat->Data = (float **)malloc (SubMat->row * sizeof(float*));
    for (int row = 0; row < SubMat->row; row++){
        SubMat->Data[row] = (float *)calloc(SubMat->col, sizeof(float));
    }


    for (int c_row = 0; c_row < result->row; c_row ++){
        for (int c_col = 0; c_col < result->col; c_col++){
            int c_row_index = 0;
            for (int sub_row = 0; sub_row < matrix->row; sub_row++){
                if (sub_row == c_row) continue;
                int c_col_index = 0;
                for (int sub_col = 0; sub_col < matrix->col; sub_col++){
                    if (sub_col == c_col) continue;
                    SubMat->Data[c_row_index][c_col_index] = matrix->Data[sub_row][sub_col];
                    c_col_index ++;
                }
                c_row_index ++;
            }
            result->Data[c_row][c_col] = MatrixDet(SubMat);
        }
    }

    Mat* f_result = DotMultiMatrices(result, CofactorMatrixCoeffs);

    Mat* t_result = Transpose(f_result);

    float deter = MatrixDet(matrix);

    for (int c_row = 0 ; c_row < f_result->row; c_row++){
        for (int c_col = 0; c_col < f_result->col ; c_col ++){
            t_result->Data[c_row][c_col] = t_result->Data[c_row][c_col] / deter;
        }
    }

    //Collecting memory garbage
    cleanup(&SubMat);

    cleanup(&CofactorMatrixCoeffs);

    cleanup(&result);
    cleanup(&f_result);

    return t_result;
}


Mat* MultiplyMatrices (Mat* Mat1, Mat* Mat2){

    Mat *result;

    result = (Mat*)malloc(sizeof(Mat));

    result->row = Mat1->row;
    result->col = Mat2->col;
    result->Data = (float **)malloc (result->row * sizeof(float*));
    for (int row = 0; row < result->row; row++){
        result->Data[row] = (float *)calloc(result->col, sizeof(float));
    }

    for (int row = 0; row < result->row; row++){
        for (int col = 0; col < result->col; col++){
            float summation = 0;
            for (int m = 0; m < Mat1->col; m++){
                summation = summation + (Mat1->Data[row][m] * Mat2->Data[m][col]);
            }
            result->Data[row][col] = summation;
        }
    }

    return result;

}

Mat * Transpose(Mat* matrix){
    Mat *D;
    D = (Mat*) malloc (sizeof(struct Matrix_st));

    D->row = matrix->col;
    D->col = matrix->row;

    D->Data = (float **)malloc (D->row * sizeof(float*));
    for (int row = 0; row < D->row; row ++){
        D->Data[row] = (float *)calloc(D->col, sizeof(float));
    }

    for (int i = 0; i< matrix->row; i++){
        for (int j = 0; j < matrix->col; j++){
            D->Data[j][i] = matrix->Data[i][j];
        }
    }
    return D;
}

Mat* GenerateFrequencyGrid(float CenterFreq, int GridSize){

    Mat *result;

    if (GridSize % 2 == 0) {
        //Convert it to an odd number
        GridSize = GridSize + 1;
    }

    result = (Mat*)malloc(sizeof(Mat));

    result->row = GridSize;
    result->col = 1;

    result->Data = (float **)malloc (result->row * sizeof(float*));
    for (int row = 0; row < result->row; row ++){
        result->Data[row] = (float *)calloc(result->col, sizeof(float));
    }

    for (int i = 0; i<GridSize; i++) {
        float freq = CenterFreq + (i - (int)((GridSize-1) / 2)) * F_Resolution;
        result->Data[i][0] = freq;
    }

    return result;

}

Mat *FourParamFit(int datacount, float CWFreq, float* data_arr , int Fs ){

    Mat * FrequencyGrid;
    Mat * g_omega;
    Mat * D;
    Mat * y;
    Mat * yT;

    Mat *DT;
    Mat *DTD;
    Mat *DTDInv;
    Mat *DTDInvDT;
    Mat *DDTDInvDT;
    Mat *DDTDInvDTy;
    Mat *yTDDTDInvDTy;

    FrequencyGrid = GenerateFrequencyGrid(CWFreq, GridDS);

    g_omega = (Mat*)malloc(sizeof(Mat));
    g_omega->row = GridDS;
    g_omega->col = 2;
    g_omega->Data = (float **)malloc (g_omega->row * sizeof(float*));
    for (int row = 0; row < g_omega->row; row ++){
        g_omega->Data[row] = (float *)calloc(g_omega->col, sizeof(float));
    }

    D = (Mat*)malloc(sizeof(Mat));
    D->row = datacount;
    D->col = 3;
    D->Data = (float**)malloc(D->row * sizeof(float*));
    for (int i=0; i<D->row;i++){
        D->Data[i] = (float*)calloc(D->col,sizeof(float));
    }

    y = (Mat*)malloc(sizeof(Mat));
    y->row = datacount;
    y->col = 1;
    y->Data = (float**)malloc(y->row * sizeof(float*));
    for (int i=0; i < y->row;i++){
        y->Data[i] = (float*)calloc(y->col,sizeof(float));
    }

    for (int i=0; i < y->row;i++){
        y->Data[i][0] = data_arr[i];
    }

    yT = Transpose(y);

    for (int freq_index = 0; freq_index<GridDS; freq_index++){

        float freq = FrequencyGrid->Data[freq_index][0];

        for (int r=0; r<D->row; r++){
            float omega = 2*PI*freq/((float)Fs);
            D->Data[r][0] = cos(omega*r);
            D->Data[r][1] = sin(omega*r);
            D->Data[r][2] = 1;
        }

        DT = Transpose (D);
        DTD = MultiplyMatrices(DT, D);
        DTDInv = InverseMatrix(DTD);
        DTDInvDT = MultiplyMatrices(DTDInv, DT);
        DDTDInvDT = MultiplyMatrices(D, DTDInvDT);
        DDTDInvDTy= MultiplyMatrices(DDTDInvDT, y);
        yTDDTDInvDTy = MultiplyMatrices(yT, DDTDInvDTy);


        g_omega->Data[freq_index][0] = yTDDTDInvDTy->Data[0][0];
        g_omega->Data[freq_index][1] = freq;

        //Collect Garbage
        cleanup(&yTDDTDInvDTy);
        cleanup(&DDTDInvDTy);
        cleanup(&DDTDInvDT);
        cleanup(&DTDInvDT);
        cleanup(&DTDInv);
        cleanup(&DTD);
        cleanup(&DT);
    }


    //Finding the maximum g_omega
    float maxFreq;
    float maxValue;

    for (int i = 0; i < GridDS; i++){
        if (i == 0) {
            maxFreq = g_omega->Data[i][1];
            maxValue = g_omega->Data[i][0];
            continue;
        } else if (g_omega->Data[i][0] > maxValue){
            maxFreq = g_omega->Data[i][1];
            maxValue = g_omega->Data[i][0];
        }
    }
    Mat* max_gomegaFitted;
    max_gomegaFitted = ThreeFitData(datacount, maxFreq, data_arr, Fs);

    Mat* result;
    result = (Mat*)malloc(sizeof(Mat));
    result->row = 4;
    result->col = 1;

    result->Data = (float **)malloc (result->row * sizeof(float*));
    for (int row = 0; row < result->row; row++){
        result->Data[row] = (float *)calloc(result->col, sizeof(float));
    }

    result->Data[0][0] = max_gomegaFitted->Data[0][0];
    result->Data[1][0] = max_gomegaFitted->Data[1][0];
    result->Data[2][0] = max_gomegaFitted->Data[2][0];
    result->Data[3][0] = maxFreq;

    cleanup(&FrequencyGrid);
    cleanup(&max_gomegaFitted);
    cleanup(&g_omega);
    cleanup(&yT);
    cleanup(&y);
    cleanup(&D);
    return result;

}


Mat* ThreeFitData (int datacount, float freq, float* data_arr, int Fs){
    Mat *D;
    D = (Mat*)malloc(sizeof(Mat));
    D->row = datacount;
    D->col = 3;
    D->Data = (float**)malloc(D->row * sizeof(float*));
    for (int i=0; i<D->row;i++){
        D->Data[i] = (float*)calloc(D->col,sizeof(float));
    }

    for (int r=0; r<D->row; r++){
            float omega = 2*PI*(freq)/((float)Fs);
            D->Data[r][0] = cos(omega*r);
            D->Data[r][1] = sin(omega*r);
            D->Data[r][2] = 1;
    }

    Mat *y;
    y = (Mat*)malloc(sizeof(Mat));
    y->row = datacount;
    y->col = 1;
    y->Data = (float**)malloc(y->row * sizeof(float*));
    for (int i=0; i < y->row;i++){
        y->Data[i] = (float*)calloc(y->col,sizeof(float));
    }

    for (int i=0; i < y->row;i++){
        y->Data[i][0] = data_arr[i];
    }

   
    Mat *DT = Transpose(D);
    Mat *DTD = MultiplyMatrices(DT, D);
    Mat *DTDInv = InverseMatrix(DTD);
    Mat *DTDInvDT = MultiplyMatrices(DTDInv, DT);
    Mat *result = MultiplyMatrices(DTDInvDT, y);

    //Collect Memory Garbage
    cleanup(&D);
    cleanup(&DT);
    cleanup(&DTD);
    cleanup(&DTDInv);
    cleanup(&DTDInvDT);
    cleanup(&y);

    return result;

}

void printMatrix(Mat * matrix){
    for (int i=0; i < matrix->row; i++){
        for (int j=0; j< matrix->col; j++){
            printf("%f\t\t", matrix->Data[i][j]);
        }
        printf("\n");
    }
}

