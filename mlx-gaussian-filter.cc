/**
 * @file     mlx-gaussian-filter.cc
 * @brief   
 * 
 * @version 1.0
 * @date    2023-09-07
 * 
 * @author  M. Anschuetz (marbuntu)
 * 
 *
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mlx-gaussian-filter.h"

#include <gsl/gsl_filter.h>
#include <gsl/gsl_vector.h>


namespace mlx
{

    MlxGaussianFilter::MlxGaussianFilter()
    : 
    _windowSize(DEFAULT_FILTER_WINDOW_SIZE),
    _alpha(DEFAULT_GAUSS_FILTER_ALPHA),
    _kernel(nullptr),
    _ws(nullptr)
    {
        setKernelSize(DEFAULT_FILTER_KERNEL_SIZE);    
    }

    MlxGaussianFilter::MlxGaussianFilter(size_t kernelSize, size_t windowSize, double alpha)
    :
    _windowSize(windowSize),
    _alpha(alpha),
    _kernel(nullptr),
    _ws(nullptr)
    {
        setKernelSize(kernelSize);
    }

    MlxGaussianFilter::~MlxGaussianFilter()
    {
    }


    void MlxGaussianFilter::setKernelSize(const size_t K)
    {
        if (K < 1)
        {
            _kernelSize = DEFAULT_FILTER_KERNEL_SIZE;
            return;
        }
            
        
        // Kernel Size should always be odd
        if ((K % 2) == 0)
        {
            _kernelSize = K + 1;
        }
        else
        {
            _kernelSize = K;
        }
    }


    size_t MlxGaussianFilter::getKernelSize() const
    {
        return _kernelSize;
    }


    void MlxGaussianFilter::setWindowSize(const size_t W)
    {
        if (W < 1) return;

        _windowSize = W;
    }


    size_t MlxGaussianFilter::getWindowSize() const 
    {
        return _windowSize;
    }


    void MlxGaussianFilter::setAlpha(const double a)
    {
        if (a <= 0) return;

        _alpha = a;
    }

    double MlxGaussianFilter::getAlpha() const 
    {
        return _alpha;
    }


    bool MlxGaussianFilter::apply(const gsl_vector *input, gsl_vector *output)
    {
        if (input->size > output->size)
        {
            return false;
        }

        _initializeWorkspace();

        gsl_filter_gaussian(GSL_FILTER_END_PADVALUE, _alpha, 0, input, output, _ws );

        return true;
    }


    bool MlxGaussianFilter::apply(const std::shared_ptr<MlxVector> input, std::shared_ptr<MlxVector> output)
    {
        return apply(input->getGslVector(), output->getGslVector());
    }


    void MlxGaussianFilter::_initializeWorkspace()
    {
        _freeWorkspace();

        _ws = gsl_filter_gaussian_alloc(_kernelSize);
        //_kernel = gsl_vector_alloc(_kernelSize);
        // gsl_filter_gaussian_kernel(_alpha, 5, 1, _kernel);

    }


    void MlxGaussianFilter::_freeWorkspace()
    {
        if (_kernel != nullptr)
        {
            gsl_vector_free(_kernel);
        }

        if (_ws != nullptr)
        {
            gsl_filter_gaussian_free(_ws);
        }

    }



}   /* namespace mlx */