#include "hcblas.h"
#include <cstdlib> 
#include "gtest/gtest.h"

TEST(hcblas_saxpy, func_correct_saxpy_Implementation_type_1) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    float *X1 = NULL;
    float *Y1 = NULL;
/* Implementation type I - Inputs and Outputs are host float pointers */
    for(int i = 0; i < lenx; i++){
            X[i] = rand() % 10;
    }
     /* X1 is NULL */
    status = hc.hcblas_saxpy(N, &alpha, X1, incX, Y, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
     /* Y1 is NULL */
    status = hc.hcblas_saxpy(N, &alpha, X, incX, Y1, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
     /* alpha is NULL */
    status = hc.hcblas_saxpy(N, NULL, X, incX, Y1, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* Proper call */
    status = hc.hcblas_saxpy(N, &alpha, X, incX, Y, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_SUCCESS); 
    /* alpha is 0*/
    alpha = 0;
    status = hc.hcblas_saxpy(N, &alpha, X, incX, Y, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_SUCCESS); 
    /* N is 0 */
    N = 0;
    status = hc.hcblas_saxpy(N, &alpha, X, incX, Y, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status = hc.hcblas_saxpy(N, &alpha, X, incX, Y, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incY is 0 */
    incX = 1;incY = 0;
    status = hc.hcblas_saxpy(N, &alpha, X, incX, Y, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_saxpy, func_correct_saxpy_Implementation_type_2) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */
    hc::array_view<float> xView(lenx, X);
    hc::array_view<float> yView(leny, Y);
    for(int i = 0; i < lenx; i++) {
            xView[i] = rand() % 10;
    }
    for(int i = 0; i < leny; i++) {
            yView[i] =  rand() % 15;
    }
    /* Proper call */
    status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 */
    alpha = 0;
    status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* N is 0 */
    N = 0;
    status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incY is 0 */
    incX = 1; incY = 0;
    status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_saxpy, func_correct_saxpy_Implementation_type_3) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    int batchSize = 128;
    long xOffset = 0;
    hcblasStatus status;
    float alpha = 1;
    long X_batchOffset = N;
    long Y_batchOffset = N; 
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */
    hc::array_view<float> xbatchView(lenx * batchSize, Xbatch);
    hc::array_view<float> ybatchView(leny * batchSize, Ybatch);
    for(int i = 0; i < lenx * batchSize; i++){
            xbatchView[i] = rand() % 10;
    }
    for(int i = 0; i < leny * batchSize; i++) {
            ybatchView[i] =  rand() % 15;
    }
    /* Proper call */
    status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* alpha is 0 */
    alpha = 0;
    status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_SUCCESS);
    /* N is 0 */
    N = 0;
    status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incX is 0 */
    incX = 0;
    status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
    /* incY is 0 */
    incX =1; incY =0;
    status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
    EXPECT_EQ(status, HCBLAS_INVALID);
}

TEST(hcblas_saxpy, func_correct_saxpy_Implementation_type_4) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    long xOffset = 0;
    hcblasStatus status; 
    float alpha = 1;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
/* Implementation type IV - Inputs and Outputs are HC++ float array containers */
   hc::array<float> xView(lenx, X);
   std::vector<float> HostX(lenx);
   hc::array<float> yView(leny, Y);
   std::vector<float> HostY(leny);
   for(int i = 0; i < lenx; i++){
            HostX[i] = rand() % 10;
   }
   for(int i = 0; i < leny; i++){
            HostY[i] =  rand() % 15;
   }
   hc::copy(begin(HostX), end(HostX), xView);
   hc::copy(begin(HostY), end(HostY), yView);
   /* Proper call */
   status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
   EXPECT_EQ(status, HCBLAS_SUCCESS);
   /* alpha is 0 */
   alpha = 0;
   status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
   EXPECT_EQ(status, HCBLAS_SUCCESS);
   /* N is 0 */
   N = 0;
   status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID); 
   /* incY is 0 */
   incX = 1; incY = 0;
   status = hc.hcblas_saxpy(accl_view, N, alpha, xView, incX, yView, incY , xOffset, yOffset);
   EXPECT_EQ(status, HCBLAS_INVALID); 
}

TEST(hcblas_saxpy, func_correct_saxpy_Implementation_type_5) {
    Hcblaslibrary hc;
    int N = 19;
    int incX = 1;
    int incY = 1;
    long yOffset = 0;
    int batchSize = 128;
    long xOffset = 0;
    float alpha = 1;
    hcblasStatus status;
    long X_batchOffset = N; 
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    
/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */
   hc::array<float> xbatchView(lenx * batchSize, Xbatch);
   std::vector<float> HostX_batch(lenx * batchSize); 
   hc::array<float> ybatchView(leny * batchSize, Ybatch);
   std::vector<float> HostY_batch(leny * batchSize);
   for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
   }
   for(int i = 0;i < leny * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
   }
   hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
   hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
   /* Proper call */
   status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCESS);
   /* alpha is 0*/
   alpha = 0;
   status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_SUCCESS);
   /* N is 0 */
   N = 0;
   status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incX is 0 */
   incX = 0;
   status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
   /* incY is 0 */
   incX = 1; incY = 0;
   status= hc.hcblas_saxpy(accl_view, N, alpha, xbatchView, incX, X_batchOffset, ybatchView, incY, Y_batchOffset, xOffset, yOffset, batchSize);
   EXPECT_EQ(status, HCBLAS_INVALID);
}
