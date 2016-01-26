#include "hcblas.h"
#include "gtest/gtest.h"
#include <hc_am.hpp>

/*TEST(hcblasCreateTest, return_Check_hcblasCreate) {
 // Proper call
 hcblasHandle_t handle;
 // Call hcblasCreate
 hcblasStatus_t status = hcblasCreate(&handle); 
 // Check the status
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
}*/

TEST(hcblasSetVectorTest, return_Check_hcblasSetVector) {
 int n = 10;
 int incx = 1, incy = 1;
 float *x1 = (float*) calloc(n, sizeof(float));
 double *x2 = (double*) calloc(n, sizeof(double));
 hcblasStatus_t status;
 hcblasHandle_t *handle = hcblasCreate();
 std::vector<hc::accelerator>accs = hc::accelerator::get_all();
 float *y1 = (float*)am_alloc(n, accs[handle->deviceId], 0);
 double *y2 = (double*)am_alloc(n, accs[handle->deviceId], 0);
 // HCBLAS_STATUS_SUCCESS
 // float type memory transfer from host to device
/* status = hcblasSetVector(handle, n, sizeof(x1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 // double type memory transfer from host to device
 status = hcblasSetVector(handle, n, sizeof(x2), x2 , incx, y2, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 */
 // HCBLAS_STATUS_INVALID_VALUE 
 // incx is 0
 status = hcblasSetVector(handle, n, sizeof(x1), x1 , 0, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // incy is 0
 status = hcblasSetVector(handle, n, sizeof(x1), x1 , incx, y1, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hcblasSetVector(handle, n, 0, x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 
// HCBLAS_STATUS_MAPPING_ERROR
 handle->deviceId = 0;
 status = hcblasSetVector(handle, n, sizeof(x1), x1 , incx, y2, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_MAPPING_ERROR);

 // HCBLAS_STATUS_NOT_INITIALIZED  
 hcblasDestroy(handle);
 status = hcblasSetVector(handle, n, sizeof(x1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

 free(x1);
 free(x2);
 hc::am_free(y1);
 hc::am_free(y2);
}

TEST(hcblasGetVectorTest, return_Check_hcblasGetVector) {
 int n = 10;
 int incx = 1, incy = 1;
 float *y1 = (float*) calloc(n, sizeof(float));
 double *y2 = (double*) calloc(n, sizeof(double));
 hcblasStatus_t status;
 hcblasHandle_t *handle = hcblasCreate();
 std::vector<hc::accelerator>accs = hc::accelerator::get_all();
 float *x1 = (float*)am_alloc(n, accs[handle->deviceId], 0);
 double *x2 = (double*)am_alloc(n, accs[handle->deviceId], 0);
 // HCBLAS_STATUS_SUCCESS
 // float type memory transfer from host to device
/*
 status = hcblasSetVector(handle, n, sizeof(y1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 // double type memory transfer from host to device
 status = hcblasSetVector(handle, n, sizeof(y2), x2 , incx, y2, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
*/
 // HCBLAS_STATUS_INVALID_VALUE
 // incx is 0
 status = hcblasSetVector(handle, n, sizeof(y1), x1 , 0, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // incy is 0
 status = hcblasSetVector(handle, n, sizeof(y1), x1 , incx, y1, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hcblasSetVector(handle, n, 0, x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);

 // HCBLAS_STATUS_MAPPING_ERROR
 handle->deviceId = 0;
 status = hcblasSetVector(handle, n, sizeof(y1), x2 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_MAPPING_ERROR);

 // HCBLAS_STATUS_NOT_INITIALIZED
 hcblasDestroy(handle);
 status = hcblasSetVector(handle, n, sizeof(y1), x1 , incx, y1, incy);
 EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

 free(y1);
 free(y2);
 hc::am_free(x1);
 hc::am_free(x2);
}

TEST(hcblasSetMatrixTest, return_Check_hcblasSetMatrix) {
 int rows = 10;
 int cols = 10;
 int lda = 1, ldb = 1;
 float *x1 = (float*) calloc(rows * cols, sizeof(float));
 double *x2 = (double*) calloc(rows * cols, sizeof(double));
 hcblasStatus_t status;
 hcblasHandle_t *handle = hcblasCreate();
 std::vector<hc::accelerator>accs = hc::accelerator::get_all();
 float *y1 = (float*)am_alloc(rows * cols, accs[handle->deviceId], 0);
 double *y2 = (double*)am_alloc(rows * cols, accs[handle->deviceId], 0);

 // HCBLAS_STATUS_INVALID_VALUE 
 // lda is 0
 status = hcblasSetMatrix(handle, rows, cols, sizeof(x1), x1 , 0, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // ldb is 0
 status = hcblasSetMatrix(handle, rows, cols, sizeof(x1), x1 , lda, y1, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hcblasSetMatrix(handle, rows, cols, 0, x1 , lda, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 
// HCBLAS_STATUS_MAPPING_ERROR
 handle->deviceId = 0;
 status = hcblasSetMatrix(handle, rows, cols, sizeof(x1), x1 , lda, y2, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_MAPPING_ERROR);

 // HCBLAS_STATUS_NOT_INITIALIZED  
 hcblasDestroy(handle);
 status = hcblasSetMatrix(handle, rows, cols, sizeof(x1), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

 free(x1);
 free(x2);
 hc::am_free(y1);
 hc::am_free(y2);
}

TEST(hcblasGetMatrixTest, return_Check_hcblasGetMatrix) {
 int rows = 10;
 int cols = 10;
 int lda = 1, ldb = 1;
 float *y1 = (float*) calloc(cols * rows, sizeof(float));
 double *y2 = (double*) calloc(cols * rows, sizeof(double));
 hcblasStatus_t status;
 hcblasHandle_t *handle = hcblasCreate();
 std::vector<hc::accelerator>accs = hc::accelerator::get_all();
 float *x1 = (float*)am_alloc(rows * cols, accs[handle->deviceId], 0);
 double *x2 = (double*)am_alloc(rows * cols, accs[handle->deviceId], 0);

 // HCBLAS_STATUS_INVALID_VALUE
 // lda is 0
 status = hcblasSetMatrix(handle, rows, cols, sizeof(y1), x1 , 0, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // ldb is 0
 status = hcblasSetMatrix(handle, rows, cols, sizeof(y1), x1 , lda, y1, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
 // elemSize is 0
 status = hcblasSetMatrix(handle, rows, cols, 0, x1 , lda, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);

 // HCBLAS_STATUS_MAPPING_ERROR
 handle->deviceId = 0;
 status = hcblasSetMatrix(handle, rows, cols, sizeof(y1), x2 , lda, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_MAPPING_ERROR);

 // HCBLAS_STATUS_NOT_INITIALIZED
 hcblasDestroy(handle);
 status = hcblasSetMatrix(handle, rows, cols, sizeof(y1), x1 , lda, y1, ldb);
 EXPECT_EQ(status, HCBLAS_STATUS_NOT_INITIALIZED);

 free(y1);
 free(y2);
 hc::am_free(x1);
 hc::am_free(x2);
}

TEST(hcblasDeviceOrderselect, func_return_Check_hcblasDeviceOrderselect) {
 hcblasHandle_t *handle = hcblasCreate();
 hcblasStatus_t status;
 hcblasOrder order = ColMajor;

 //device CPU
 status = hcblasDeviceOrderSelect(handle, 0, order);
 EXPECT_EQ(handle->deviceId, 0);
 EXPECT_EQ(handle->Order, 1);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 
 //device GPU 1
 status = hcblasDeviceOrderSelect(handle, 1, order);
 EXPECT_EQ(handle->deviceId, 1);
 EXPECT_EQ(handle->Order, 1);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

 //Row major and device CPU
 order = RowMajor;
 status = hcblasDeviceOrderSelect(handle, 0, order);
 EXPECT_EQ(handle->deviceId, 0);
 EXPECT_EQ(handle->Order, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);

 //Row major and device GPU 1
 status = hcblasDeviceOrderSelect(handle, 1, order);
 EXPECT_EQ(handle->deviceId, 1);
 EXPECT_EQ(handle->Order, 0);
 EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS);
 status = hcblasDeviceOrderSelect(handle, 2, order);
 EXPECT_EQ(status, HCBLAS_STATUS_INVALID_VALUE);
}
