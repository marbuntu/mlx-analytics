

#include "mlx-fft.h"

#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_complex.h>

#include <stdio.h>

#define REAL(z,i) ((z)[2*(i)])
#define IMAG(z,i) ((z)[2*(i)+1])


namespace mlx {

    MlxFastFourrierProcessor::MlxFastFourrierProcessor()
    {}


    MlxFastFourrierProcessor::~MlxFastFourrierProcessor()
    {}



    void MlxFastFourrierProcessor::run_test()
    {
        int i; double data[2*128];

        for (i = 0; i < 128; i++)
            {
            REAL(data,i) = 0.0; 
            IMAG(data,i) = 0.0;
            }

        REAL(data,0) = 1.0;

        for (i = 1; i <= 10; i++)
            {
            REAL(data,i) = REAL(data,128-i) = 1.0;
            }

        for (i = 0; i < 128; i++)
            {
            printf ("%d %e %e\n", i,
                    REAL(data,i), IMAG(data,i));
            }
        printf ("\n\n");

        gsl_fft_complex_radix2_forward (data, 1, 128);

        for (i = 0; i < 128; i++)
            {
            printf ("%d %e %e\n", i,
                    REAL(data,i)/sqrt(128),
                    IMAG(data,i)/sqrt(128));
            }
    }




    MlxMixedRadixRealFFT::MlxMixedRadixRealFFT(size_t length)
    : _length(length)
    , _wvt(gsl_fft_real_wavetable_alloc(length))
    , _wrk(gsl_fft_real_workspace_alloc(length))
    {
    }


    MlxMixedRadixRealFFT::~MlxMixedRadixRealFFT()
    {
        gsl_fft_real_wavetable_free(_wvt);
        gsl_fft_real_workspace_free(_wrk);
    }


    size_t MlxMixedRadixRealFFT::length() const
    {
        return _length;
    }


    std::shared_ptr<MlxDoubleVector> MlxMixedRadixRealFFT::normalizedMagnitude(const MlxDoubleVector &signal)
    {

        gsl_vector* inp = signal.toGslVector();

        // Generate the Half Complex Coeffs
        gsl_fft_real_transform(inp->data, 1, inp->size, _wvt, _wrk);

        // **2 for all Vector Elements
        gsl_vector_mul(inp, inp);


        std::shared_ptr<MlxDoubleVector> res = std::make_shared<MlxDoubleVector>(inp->size);

        size_t i = 1;
        double factor = 2.0 / inp->size; /// (inp->size * inp->size);
        
        double val = factor * inp->data[0]; ///(inp->data[0] * inp->data[0]);
        res->set(0, val);
        res->set(res->size()-1, val);
        
        for (size_t n = 1; n < inp->size; n += 2)
        {
            val = factor * sqrt(inp->data[n] + inp->data[n+1]);

            res->set(i,val);
            res->set(inp->size - (1+i), val);

            i++;
        }

        gsl_vector_free(inp);

        return res;
    }


    std::shared_ptr<MlxDoubleVector> MlxMixedRadixRealFFT::pwrSpectralDensity(const MlxDoubleVector &signal, const double fs, const double df)
    {

        gsl_vector* inp = signal.toGslVector();

        // Generate the Half Complex Coeffs
        gsl_fft_real_transform(inp->data, 1, inp->size, _wvt, _wrk);

        // **2 for all Vector Elements
        gsl_vector_mul(inp, inp);
       

        double factor = 2.0 / (inp->size); // * inp->size); /// (inp->size * inp->size);
        double val = 0 ; //factor * inp->data[0]; ///(inp->data[0] * inp->data[0]);

        std::vector<double> integ;

        integ.push_back(factor * inp->data[0]);

        for (size_t n = 1; n < inp->size; n += 2)
        {
            val = factor * abs(inp->data[n] + inp->data[n + 1]);
            integ.push_back(val);
        }   

        std::shared_ptr<MlxDoubleVector> res = std::make_shared<MlxDoubleVector>(integ);

        return res;
    }



} /*    namespace mlx   */