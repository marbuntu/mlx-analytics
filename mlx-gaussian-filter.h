/**
 * @file     mlx-gaussian-filter.h
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


#pragma once

#include <memory>
#include "structures/mlx-vector.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_filter.h>


namespace mlx 
{

    static const size_t DEFAULT_FILTER_KERNEL_SIZE = 51;
    static const size_t DEFAULT_FILTER_WINDOW_SIZE = 500;
    static const size_t DEFAULT_GAUSS_FILTER_ALPHA = 0.5;


    class MlxGaussianFilter final
    {
    public:
        MlxGaussianFilter();
        MlxGaussianFilter(size_t kernelSize, size_t windowSize, double alpha);
        
        /**
         * @brief Desctructor
         * 
         */
        ~MlxGaussianFilter();

        void setKernelSize(const size_t K);
        size_t getKernelSize() const;

        void setWindowSize(const size_t W);
        size_t getWindowSize() const;

        void setAlpha(const double a);
        double getAlpha() const;


        bool apply(const gsl_vector *input, gsl_vector *output);

        bool apply(const std::shared_ptr<MlxDoubleVector> input, std::shared_ptr<MlxDoubleVector> output);


    protected:

        void _initializeWorkspace();
        void _freeWorkspace();

    private:

        size_t _kernelSize;
        size_t _windowSize;
        double _alpha;

        gsl_filter_gaussian_workspace *_ws;
        gsl_vector *_kernel;


    };  /* MlxGaussianFiler */



}   /* namespace mlx */