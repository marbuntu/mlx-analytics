/**
 * @file    mlx-fft.h
 * @brief   Interface to GSL Fast Fourrer Transformation 
 * 
 * @version 1.0
 * @date    2023-09-14
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
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>


namespace mlx {

/**
 * @brief   Generates a Workspace for GSL Real Input FFT
 * 
 */
class MlxFastFourrierProcessor final
{
public:
    MlxFastFourrierProcessor();
    ~MlxFastFourrierProcessor();

    


    void run_test();

protected:



private:


}; /*  MlxFastFourrierProcessor */



class MlxMixedRadixRealFFT
{
public:

    MlxMixedRadixRealFFT(size_t length);
    ~MlxMixedRadixRealFFT();


    size_t length() const;


    std::shared_ptr<MlxDoubleVector> normalizedMagnitude(MlxDoubleVector &signal);

    std::shared_ptr<MlxDoubleVector> pwrSpectralDensity(MlxDoubleVector &signal, const double fs, const double df);



protected:

    const size_t _length;
    gsl_fft_real_wavetable *_wvt;
    gsl_fft_real_workspace *_wrk;    


};  /* MlxMixedRadixRealFFT */


}   /* namespace mlx */