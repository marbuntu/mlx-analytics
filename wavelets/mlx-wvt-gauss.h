/**
 * @file    mlx-wvt-gauss.h
 * @brief   Gauss Wavelet Implementation (GSL Compatible)
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


#ifndef MLX_WAVELETS_GAUSS_H_
#define MLX_WAVELETS_GAUSS_H_


#include <gsl/gsl_vector.h>
#include <gsl/gsl_filter.h>
#include <gsl/gsl_wavelet.h>


#ifdef __cplusplus
    extern "C" {
#endif 


/**
 * @brief   Generate Gauss Wavelet as GSL Vector
 * 
 * @param   alpha  Standard Deviation Parameter
 * @param   N      Number of samples 
 * @param   out    
 */
void generate_gauss_wavelet(const double alpha, size_t N, gsl_vector **out);


gsl_wavelet* mlx_gauss_wavelet_allocate(const double alpha, size_t N);


#ifdef __cplusplus
    }
#endif 

#endif /* MLX_WAVELETS_GAUSS_H_ */