#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"
#include "hc_am.hpp"
#include "cblas.h"
TEST(hcblas_dcopy, return_correct_dcopy_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 23;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    long xOffset = 0;
    hcblasStatus status; 
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    double *X = (double*)calloc(lenx, sizeof(double));
    double *Y = (double*)calloc(leny, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].get_default_view());
/* Implementation type I - Inputs and Outputs are HCC device pointers*/
   double* devX = hc::am_alloc(sizeof(double) * lenx, acc[1], 0);
   double* devY = hc::am_alloc(sizeof(double) * leny, acc[1], 0);
   double* devX1 = NULL;
   double* devY1 = NULL;
   for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
   }
   for(int i = 0; i < leny; i++){
            Y[i] =  rand() % 15;
   }
   hc::am_copy(devX, X, lenx * sizeof(double));
   hc::am_copy(devY, Y, leny * sizeof(double));
   /* Proper call */
   status = hc.hcblas_dcopy(accl_view, N, devX, incX, xOffset, devY, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* X and Y are null */
   status = hc.hcblas_dcopy(accl_view, N, devX1, incX, xOffset, devY1, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_dcopy(accl_view, N, devX, incX, xOffset, devY, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_dcopy(accl_view, N, devX, incX, xOffset, devY, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID); 
   /* incY is 0 */
   incX = 1; incY = 0;
   status = hc.hcblas_dcopy(accl_view, N, devX, incX, xOffset, devY, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   free(X);
   free(Y);
   hc::am_free(devX);
   hc::am_free(devY);
}

TEST(hcblas_dcopy, func_correct_dcopy_Implementation_type_1) {
   Hcblaslibrary hc;
   int N = 23;
   int incX = 1;
   int incY = 1;
   long yOffset = 0;
   long xOffset = 0;
   hcblasStatus status;
   long lenx = 1 + (N-1) * abs(incX);
   long leny = 1 + (N-1) * abs(incY);
   double *X = (double*)calloc(lenx, sizeof(double));
   double *Y = (double*)calloc(leny, sizeof(double));
   double *Ycblas = (double*)calloc(leny, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
   /* Implementation type I - Inputs and Outputs are HCC device pointers*/
   double* devX = hc::am_alloc(sizeof(double) * lenx, acc[1], 0);
   double* devY = hc::am_alloc(sizeof(double) * leny, acc[1], 0);
   for(int i = 0; i < lenx; i++){
             X[i] = rand() % 10;
   }
   for(int i = 0; i < leny; i++){
             Y[i] =  rand() % 15;
	     Ycblas[i] = Y[i];
   }
   hc::am_copy(devX, X, lenx * sizeof(double));
   hc::am_copy(devY, Y, leny * sizeof(double));
   status = hc.hcblas_dcopy(accl_view, N, devX, incX, xOffset, devY, incY, yOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   hc::am_copy(Y, devY, leny * sizeof(double));
   cblas_dcopy( N, X, incX, Ycblas, incY );
   for(int i = 0; i < leny; i++){
           EXPECT_EQ(Y[i], Ycblas[i]);
   }
   free(X);
   free(Y);
   free(Ycblas);
   hc::am_free(devX);
   hc::am_free(devY);
}

TEST(hcblas_dcopy, return_correct_dcopy_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 23;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    int batchSize = 32;
    long xOffset = 0;
    hcblasStatus status;
    long X_batchOffset = N; 
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
    double *Ybatch = (double*)calloc(leny * batchSize, sizeof(double));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].get_default_view());
    double* devXbatch = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[1], 0);
    double* devYbatch = hc::am_alloc(sizeof(double) * leny * batchSize, acc[1], 0); 
    double* devX1batch = NULL;
    double* devY1batch = NULL;
/* Implementation type II - Inputs and Outputs are HCC device pointers with batch processing */
   for(int i = 0;i < lenx * batchSize;i++){
            Xbatch[i] = rand() % 10;
   }
   for(int i = 0;i < leny * batchSize;i++){
            Ybatch[i] =  rand() % 15;
   }
   hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(double));
   hc::am_copy(devYbatch, Ybatch, leny * batchSize * sizeof(double));
   /* Proper call */
   status= hc.hcblas_dcopy(accl_view, N, devXbatch, incX, xOffset, devYbatch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* x and y are null */
   status= hc.hcblas_dcopy(accl_view, N, devX1batch, incX, xOffset, devY1batch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_dcopy(accl_view, N, devXbatch, incX, xOffset, devYbatch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_dcopy(accl_view, N, devXbatch, incX, xOffset, devYbatch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incY is 0 */
   incX = 1; incY = 0;
   status= hc.hcblas_dcopy(accl_view, N, devXbatch, incX, xOffset, devYbatch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   free(Xbatch);
   free(Ybatch);
   hc::am_free(devXbatch);
   hc::am_free(devYbatch);
}

TEST(hcblas_dcopy, func_correct_dcopy_Implementation_type_2) {
   Hcblaslibrary hc;
   int N = 23;
   int incX = 1;
   int incY = 1;
   long yOffset = 0;
   int batchSize = 32;
   long xOffset = 0;
   hcblasStatus status;
   long X_batchOffset = N;
   long Y_batchOffset = N;
   long lenx = 1 + (N-1) * abs(incX);
   long leny = 1 + (N-1) * abs(incY);
   double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));
   double *Ybatch = (double*)calloc(leny * batchSize, sizeof(double));
   double *Ycblasbatch = (double*)calloc(leny * batchSize, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
   double* devXbatch = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[1], 0);
   double* devYbatch = hc::am_alloc(sizeof(double) * leny * batchSize, acc[1], 0);
/* Implementation type II - Inputs and Outputs are HCC device pointers with batch processing */
   for(int i = 0;i < lenx * batchSize;i++){
             Xbatch[i] = rand() % 10;
   }
   for(int i = 0;i < leny * batchSize;i++){
             Ybatch[i] =  rand() % 15;
	     Ycblasbatch[i] = Ybatch[i];
   }
   hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(double));
   hc::am_copy(devYbatch, Ybatch, leny * batchSize * sizeof(double));
   status= hc.hcblas_dcopy(accl_view, N, devXbatch, incX, xOffset, devYbatch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   hc::am_copy(Ybatch, devYbatch, leny * batchSize * sizeof(double));
   for(int i = 0; i < batchSize; i++)
             cblas_dcopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
   for(int i =0; i < leny * batchSize; i++){
             EXPECT_EQ(Ybatch[i], Ycblasbatch[i]);
   }	     
   free(Xbatch);
   free(Ybatch);
   free(Ycblasbatch);
   hc::am_free(devXbatch);
   hc::am_free(devYbatch);

}
