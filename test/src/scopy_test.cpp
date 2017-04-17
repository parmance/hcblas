#include <iostream>
#include "hcblaslib.h"
#include <cstdlib> 
#include "cblas.h"
#include "hc_am.hpp"

using namespace std;
int main(int argc, char** argv)
{   
    /* HCBLAS implementation */
    Hcblaslibrary hc;
    if (argc < 3){
        cout<<"No sufficient commandline arguments specified"<<"argc :"<<argc<<endl;
        return -1;
    }
    int N = atoi(argv[1]);
    int Imple_type = atoi(argv[2]);
    int incX = 1;
    long xOffset = 0;
    int incY = 1;
    long yOffset = 0;
    hcblasStatus status;
    int batchSize = 32;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    /* CBLAS implementation */
    bool ispassed = 1;
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].get_default_view());

/* Implementation type I - Inputs and Outputs are HCC float array containers */
    
    if (Imple_type == 1) {
	float *X = (float*)calloc(lenx, sizeof(float));
	float *Y = (float*)calloc(leny, sizeof(float));
        float *Ycblas = (float*)calloc(leny, sizeof(float));
	float* devX = hc::am_alloc(sizeof(float) * lenx, acc[1], 0);
	float* devY = hc::am_alloc(sizeof(float) * leny, acc[1], 0);
        for(int i = 0;i < lenx;i++){
            X[i] = rand() % 10;
        }
        for(int i = 0;i < leny;i++){
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
        }
	hc::am_copy(devX, X, lenx * sizeof(float));
	hc::am_copy(devY, Y, leny * sizeof(float));
        status = hc.hcblas_scopy(accl_view, N, devX, incX, xOffset, devY, incY, yOffset);
	hc::am_copy(Y, devY, leny * sizeof(float));
        cblas_scopy( N, X, incX, Ycblas, incY );
        for(int i = 0; i < leny; i++){
            if (Y[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << Y[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl;
        free(X);
        free(Y);
        free(Ycblas);
        hc::am_free(devX);
        hc::am_free(devY);	
     }

/* Implementation type II - Inputs and Outputs are HCC float array containers with batch processing */

    else{
	float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
        float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
        float *Ycblasbatch = (float*)calloc(leny * batchSize, sizeof(float));	
	float* devXbatch = hc::am_alloc(sizeof(float) * lenx * batchSize, acc[1], 0);
	float* devYbatch = hc::am_alloc(sizeof(float) * leny * batchSize, acc[1], 0);
        for(int i = 0;i < lenx * batchSize;i++){
            Xbatch[i] = rand() % 10;
        }
        for(int i = 0;i < leny * batchSize;i++){
            Ybatch[i] =  rand() % 15;
            Ycblasbatch[i] = Ybatch[i];
         }
	hc::am_copy(devXbatch, Xbatch, lenx * batchSize * sizeof(float));
	hc::am_copy(devYbatch, Ybatch, leny * batchSize * sizeof(float));
        status= hc.hcblas_scopy(accl_view, N, devXbatch, incX, xOffset, devYbatch, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
	hc::am_copy(Ybatch, devYbatch, leny * batchSize * sizeof(float));
        for(int i = 0; i < batchSize; i++)
        	cblas_scopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < leny * batchSize; i++){
            if (Ybatch[i] != Ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " <<Ybatch[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
	free(Xbatch);
	free(Ybatch);
	free(Ycblasbatch);
	hc::am_free(devXbatch);
	hc::am_free(devYbatch);
    }
    return 0;
}
