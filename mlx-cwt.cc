/**
 * @file    mlx-cwt.cc
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

#include "mlx-cwt.h"

#include <math.h>
#include <gsl/gsl_errno.h>


extern gsl_wavelet_type *mlx_wavelet_gaussian;
extern gsl_wavelet_type *mlx_wavelet_gaussian_centered;
extern double mlx_default_gaussian_wavelet_alpha;


namespace mlx {






    MlxFixedVector<double> WaveletTransform(const MlxFixedVector<double> &signal, bool WaveletOption, size_t w_start, size_t w_end)
    {

        return MlxFixedVector<double>(1);
    }



    MlxWaveletTransformation::MlxWaveletTransformation(size_t win_length, size_t wvt_length)
    : _length(win_length)
    , _wrk(gsl_wavelet_workspace_alloc(win_length))
    {
        //_wavelet = mlx_gauss_wavelet_allocate(2.8, 10);
        _wavelet = gsl_wavelet_alloc(mlx_wavelet_gaussian_centered, wvt_length);
        //gsl_wavelet_alloc(gsl_wavelet_bspline, 204);
    }


    MlxWaveletTransformation::~MlxWaveletTransformation()
    {
        gsl_wavelet_free(_wavelet);
        gsl_wavelet_workspace_free(_wrk);
    }


    std::shared_ptr<MlxFixedVector<double>> MlxWaveletTransformation::WVT_1D(const MlxFixedVector<double> &signal)
    {
        gsl_vector *inp = signal.toGslVector();

        gsl_wavelet_transform_forward(_wavelet, inp->data, 1, inp->size, _wrk);

        std::shared_ptr<MlxFixedVector<double>> res = std::make_shared<MlxFixedVector<double>>(inp);
    
        gsl_vector_free(inp);

        return res;
    }

    void MlxWaveletTransformation::outputWvt(std::ofstream &fst, size_t padding) const
    {
        fst << _wavelet->nc;

        for (size_t n = 0; n < padding; n++)
        {
            fst << "\t" << (n < _wavelet->nc ? _wavelet->h1[n] : 0.0);
        }

        fst << "\n";
    }


} /*    namespace mlx   */