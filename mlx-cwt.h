/**
 * @file    mlx-cwt.h
 * @brief   Complex Wavelet Transformation
 * 
 * @version 1.0
 * @date    2023-09-18
 * 
 * @author  M. Anschuetz (marbuntu)
 * 
 *
 * 
 * @copyright Copyright (c) 2023
 * 
 */



#pragma once

#include "structures/mlx-vector.h"
#include "wavelets/mlx-wvt-gauss.h"
#include <math.h>
#include <fstream>
#include <gsl/gsl_wavelet.h>


namespace mlx {
    

typedef enum {
    MLX_WAVELET_GAUSS = 1,
    MLX_WAVELET_GAUSS_CENTERED = 11,
    GLS_WAVELET_BSPLINE = 2,
    GLS_WAVELET_BSPLINE_CENTERED = 21,
    GSL_WAVELET_DAUBECHIES = 3,
    GSL_WAVELET_DAUBECHIES_CENTERED = 31,
    GSL_WAVELET_HAAR = 4,
    GSL_WAVELET_HAAR_CENTERED = 41,
} WaveletTemplate_t;



class MlxWaveletTransformation
{
public:
    MlxWaveletTransformation(size_t win_length, size_t wvt_length);
    MlxWaveletTransformation(size_t win_length, size_t wvt_length, WaveletTemplate_t wvt_template);
    ~MlxWaveletTransformation();

    size_t length() const;


    void outputWvt(std::ofstream &fst, size_t padding) const;


    std::shared_ptr<MlxFixedVector<double>> WVT_1D(const MlxFixedVector<double> &signal);



protected:

    const size_t _length;

    gsl_wavelet *_wavelet;
    gsl_wavelet_workspace *_wrk;



};   /* MlxWaveletTransformation1D */



}   /* namespace mlx */