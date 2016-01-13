#include <iostream>
#include "hcblas.h"
#include <cstdlib> 
#include "cblas.h"
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
    int batchSize = 128;
    long X_batchOffset = N;
    long Y_batchOffset = N;
    long lenx = 1 + (N-1) * abs(incX);
    long leny = 1 + (N-1) * abs(incY);
    float *X = (float*)calloc(lenx, sizeof(float));
    float *Y = (float*)calloc(leny, sizeof(float));
    float *Xbatch = (float*)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float*)calloc(leny * batchSize, sizeof(float));
    /* CBLAS implementation */
    bool ispassed = 1;
    float *Ycblas = (float*)calloc(leny, sizeof(float));
    float *Ycblasbatch = (float*)calloc(leny * batchSize, sizeof(float));
    std::vector<hc::accelerator>acc = hc::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());

/* Implementation type I - Inputs and Outputs are host float pointers */

    if (Imple_type == 1) {
        for(int i = 0;i < lenx;i++) {
            X[i] = rand() % 10;
        }
        for(int i = 0;i < leny;i++) {
            Y[i] =  rand() % 15;
            Ycblas[i] = Y[i];
        }

	status = hc.hcblas_scopy(N, X, incX, xOffset, Y, incY, yOffset);
        cblas_scopy( N, X, incX, Ycblas, incY);
        for(int i = 0; i < leny ; i++){
            if (Y[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << Y[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        free(Ycblas);
        if(status) cout << "TEST FAILED" << endl; 
        free(X);
        free(Y);
    }

/* Implementation type II - Inputs and Outputs are HC++ float array_view containers */

    else if (Imple_type ==2) {
        hc::array_view<float> xView(lenx, X);
        hc::array_view<float> yView(leny, Y);
        for(int i = 0;i < lenx;i++) {
            xView[i] = rand() % 10;
            X[i] = xView[i];
        }
        for(int i = 0;i < leny;i++) {
            yView[i] =  rand() % 15;
            Ycblas[i] = yView[i];
        }
        status = hc.hcblas_scopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
        cblas_scopy( N, X, incX, Ycblas, incY );
        for(int i = 0; i < N ; i++) {
            if (yView[i] != Ycblas[i]) {
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << yView[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 

     }

/* Implementation type III - Inputs and Outputs are HC++ float array_view containers with batch processing */

     else if(Imple_type == 3) {
        hc::array_view<float> xbatchView(lenx * batchSize, Xbatch);
        hc::array_view<float> ybatchView(leny * batchSize, Ybatch);
        for(int i = 0;i < lenx * batchSize;i++) {
            xbatchView[i] = rand() % 10;
            Xbatch[i] = xbatchView[i];
        }
        for(int i = 0;i < leny * batchSize;i++) {
            ybatchView[i] =  rand() % 15;
            Ycblasbatch[i] = ybatchView[i];
        }
        status= hc.hcblas_scopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
        for(int i = 0; i < batchSize; i++)
                cblas_scopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < N * batchSize; i++) {
            if (ybatchView[i] != Ycblasbatch[i]) {
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << ybatchView[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else
              continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
    }

/* Implementation type IV - Inputs and Outputs are HC++ float array containers */
    
    else if (Imple_type == 4) {
        hc::array<float> xView(lenx, X);
        hc::array<float> yView(leny, Y);
        std::vector<float> HostX(lenx);
        std::vector<float> HostY(leny);
        for(int i = 0;i < lenx;i++){
            HostX[i] = rand() % 10;
            X[i] = HostX[i];
        }
        for(int i = 0;i < leny;i++){
            HostY[i] =  rand() % 15;
            Ycblas[i] = Y[i];
        }
        hc::copy(begin(HostX), end(HostX), xView);
        hc::copy(begin(HostY), end(HostY), yView);
        status = hc.hcblas_scopy(accl_view, N, xView, incX, xOffset, yView, incY, yOffset);
        hc::copy(yView, begin(HostY));
        cblas_scopy( N, X, incX, Ycblas, incY );
        for(int i = 0; i < leny; i++){
            if (HostY[i] != Ycblas[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " << HostY[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblas[i] << endl;
                break;
            }
            else
                continue;
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
     }

/* Implementation type V - Inputs and Outputs are HC++ float array containers with batch processing */

    else{ 
        hc::array<float> xbatchView(lenx * batchSize, Xbatch);
        hc::array<float> ybatchView(leny * batchSize, Ybatch);
        std::vector<float> HostX_batch(lenx * batchSize);
        std::vector<float> HostY_batch(leny * batchSize);
        for(int i = 0;i < lenx * batchSize;i++){
            HostX_batch[i] = rand() % 10;
            Xbatch[i] = HostX_batch[i];
        }
        for(int i = 0;i < leny * batchSize;i++){
            HostY_batch[i] =  rand() % 15;
            Ycblasbatch[i] = HostY_batch[i];
         }
        hc::copy(begin(HostX_batch), end(HostX_batch), xbatchView);
        hc::copy(begin(HostY_batch), end(HostY_batch), ybatchView);
        status= hc.hcblas_scopy(accl_view, N, xbatchView, incX, xOffset, ybatchView, incY, yOffset, X_batchOffset, Y_batchOffset, batchSize);
        hc::copy(ybatchView, begin(HostY_batch));
        for(int i = 0; i < batchSize; i++)
        	cblas_scopy( N, Xbatch + i * N, incX, Ycblasbatch + i * N, incY );
        for(int i =0; i < leny * batchSize; i ++){
            if (HostY_batch[i] != Ycblasbatch[i]){
                ispassed = 0;
                cout <<" HCSCOPY[" << i<< "] " <<HostY_batch[i] << " does not match with CBLASSCOPY[" << i <<"] "<< Ycblasbatch[i] << endl;
                break;
            }
            else 
              continue;  
        }
        if(!ispassed) cout << "TEST FAILED" << endl; 
        if(status) cout << "TEST FAILED" << endl; 
    }
    return 0;
}
