#include "hcblaslib.h"
#include <cstdlib> 
#include "gtest/gtest.h"
#include "hc_am.hpp"
#include "cblas.h"

TEST(hcblas_dscal, return_correct_dscal_Implementation_type_1) {
   Hcblaslibrary hc;
   int N = 23;
   int incX = 1;
   long xOffset = 0;
   double alpha = 1;
   hcblasStatus status; 
   long lenx = 1 + (N-1) * abs(incX);
   double *X = (double*)calloc(lenx, sizeof(double));//host input
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
/* Implementation type I - Inputs and Outputs are HCC double array containers */
   double* devX = hc::am_alloc(sizeof(double) * lenx, acc[1], 0);
   for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
   }
   double* devX1 = NULL;
   hc::am_copy(devX, X, lenx * sizeof(double));
   /* devX1 is NULL */
   status = hc.hcblas_dscal(accl_view, N, alpha, devX1, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* alpha is some scalar */
   status = hc.hcblas_dscal(accl_view, N, alpha, devX, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* alpha is 0 */
   alpha = 0;
   status = hc.hcblas_dscal(accl_view, N, alpha, devX, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_dscal(accl_view, N, alpha, devX, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_dscal(accl_view, N, alpha, devX, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_INVALID); 
}

TEST(hcblas_dscal, function_correct_dscal_Implementation_type_1) {
   Hcblaslibrary hc;
   int N = 23;
   int incX = 1;
   long xOffset = 0;
   double alpha = 1;
   hcblasStatus status;
   long lenx = 1 + (N-1) * abs(incX);
   double *X = (double*)calloc(lenx, sizeof(double));//host input
   double *Xcblas = (double*)calloc(lenx, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
 /* Implementation type I - Inputs and Outputs are HCC double array containers */
   double* devX = hc::am_alloc(sizeof(double) * lenx, acc[1], 0);
   for(int i = 0; i < lenx; i++){
           X[i] = rand() % 10;
	   Xcblas[i] = X[i];
   }
   hc::am_copy(devX, X, lenx * sizeof(double));
   status = hc.hcblas_dscal(accl_view, N, alpha, devX, incX, xOffset);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   hc::am_copy(X, devX, lenx * sizeof(double));
   cblas_dscal( N, alpha, Xcblas, incX );
   for(int i = 0; i < lenx ; i++){
	   EXPECT_EQ(X[i], Xcblas[i]);
   }
}

TEST(hcblas_dscal, return_correct_dscal_Implementation_type_2) {
   Hcblaslibrary hc;
   int N = 19;
   int incX = 1;
   int batchSize = 32;
   long xOffset = 0;
   double alpha = 1;
   hcblasStatus status;
   long X_batchOffset = N; 
   long lenx = 1 + (N-1) * abs(incX);
   double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));//host input
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
   double* devXbatch = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[1], 0);
   double* devX1batch = NULL; 
/* Implementation type II - Inputs and Outputs are HCC double array containers with batch processing */
   for(int i = 0;i < lenx * batchSize;i++){
            Xbatch[i] = rand() % 10;
   }
   hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(double));
   /* x1 is NULL */
   status= hc.hcblas_dscal(accl_view, N, alpha, devX1batch, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* alpha is some scalar */
   status= hc.hcblas_dscal(accl_view, N, alpha, devXbatch, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* alpha is 0 */
   alpha = 0;  
   status= hc.hcblas_dscal(accl_view, N, alpha, devXbatch, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_dscal(accl_view, N, alpha, devXbatch, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_dscal(accl_view, N, alpha, devXbatch, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_dscal, function_correct_dscal_Implementation_type_2) {
   Hcblaslibrary hc;
   int N = 19;
   int incX = 1;
   int batchSize = 32;
   long xOffset = 0;
   double alpha = 1;
   hcblasStatus status;
   long X_batchOffset = N;
   long lenx = 1 + (N-1) * abs(incX);
   double *Xbatch = (double*)calloc(lenx * batchSize, sizeof(double));//host input
   double *Xcblasbatch = (double*)calloc(lenx * batchSize, sizeof(double));
   std::vector<hc::accelerator>acc = hc::accelerator::get_all();
   accelerator_view accl_view = (acc[1].get_default_view());
   double* devXbatch = hc::am_alloc(sizeof(double) * lenx * batchSize, acc[1], 0);
   /* Implementation type II - Inputs and Outputs are HCC double array containers with batch processing */
   for(int i = 0;i < lenx * batchSize;i++){
               Xbatch[i] = rand() % 10;
	       Xcblasbatch[i] =  Xbatch[i];
   }
   hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(double));
   status= hc.hcblas_dscal(accl_view, N, alpha, devXbatch, incX, xOffset, X_batchOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCEEDS);
   hc::am_copy(Xbatch, devXbatch, lenx * batchSize * sizeof(double));
   for(int i = 0; i < batchSize; i++)
        cblas_dscal( N, alpha, Xcblasbatch + i * N, incX);
   for(int i =0; i < lenx * batchSize; i ++){
	EXPECT_EQ(Xbatch[i], Xcblasbatch[i]);
   }
}


