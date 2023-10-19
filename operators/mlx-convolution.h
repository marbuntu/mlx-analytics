/**
 * @file    mlx-convolution.h
 * @brief   Convolution
 * 
 * @version 1.0
 * @date    2023-10-17
 * 
 * @author  M. Anschuetz (marbuntu)
 * 
 *
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once

#include "../structures/mlx-vector.h"
#include <gsl/gsl_fft_complex.h>


namespace mlx
{

class MlxConvolutionWorkspace 
{
public:

    MlxConvolutionWorkspace(size_t N);
    ~MlxConvolutionWorkspace();
    

private:
    gsl_fft_complex_workspace *ws_col;


};  /* MlxConvolutionWorkspace */



class MlxConvolution
{
public:

    typedef enum {
        LINEAR_FULL = 1
    } ConvolutionMode_t;


    MlxDoubleVector Convolve1D(MlxDoubleVector& vect);

protected:
    MlxConvolution();
    ~MlxConvolution();

private:

};

    
} /*    namepace mlx    */
