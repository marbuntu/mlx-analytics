/**
 * @file     mlx-analytics.cc
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


#include <iostream>
#include <algorithm>

#include "mlx-analytics.h"
#include "mlx-gaussian-filter.h"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_fft_complex.h>


namespace mlx
{
    std::unordered_map<size_t, MlxMixedRadixRealFFT*> MlxAnalyticsInterface::_fft_real_workspaces = {};


    MlxAnalyticsInterface::MlxAnalyticsInterface()
    {
    }


    MlxAnalyticsInterface::~MlxAnalyticsInterface()
    {
        _freeFFTWorkspaces();
    }


/*
    bool MlxAnalyticsInterface::setInputVectors(const std::vector<double> vec_time, const std::vector<double> vec_values)
    {
        _time = std::make_shared<MlxVector>(vec_time);
        _vals = std::make_shared<MlxVector>(vec_values);

        return true;
    }


    bool MlxAnalyticsInterface::setInputVectors(const MlxVector &vec_time, const MlxVector &vec_values)
    {
        _time = std::make_shared<MlxVector>(vec_time);
        _vals = std::make_shared<MlxVector>(vec_values);

        return true;
    }

    std::shared_ptr<MlxVector> MlxAnalyticsInterface::smoothening(std::shared_ptr<MlxVector> signal, double fs, double cutoff)
    {
        size_t K = 61; // (size_t ((cutoff * fs)) / 2);

        MlxGaussianFilter filter(K, signal->size(), 3.0);
        std::shared_ptr<MlxVector> out = std::make_shared<MlxVector>(signal->size());
        filter.apply(signal, out);

        return out;
    }


    std::shared_ptr<MlxVector> MlxAnalyticsInterface::filtfilt(std::shared_ptr<MlxVector> signal, std::shared_ptr<MlxSOSFilter> filter)
    {

        std::vector<double> inp = signal->toStdVector();
        std::vector<double> tmp;
        std::vector<double> res;

        for (const auto& v : inp)
        {
            tmp.push_back(filter->filter(v));
        }

        std::reverse(tmp.begin(), tmp.end());

        for (const auto& v : tmp)
        {
            res.push_back(filter->filter(v));
        }

        std::reverse(res.begin(), res.end());

        return std::make_shared<MlxVector>(res);;
    }
*/

    std::shared_ptr<MlxFixedVector<double>> MlxAnalyticsInterface::FFTFrequencies(MlxFixedVector<double> &signal, const double fs)
    {
        std::vector<double> frqs;
        frqs.resize(signal.size());

        double df = fs / ( 2.0 * signal.size());
        double f = 0;

        for (size_t n = 0; n < signal.size(); n++)
        {
            frqs.at(n) = f;
            f += df;
        }

        return std::make_shared<MlxFixedVector<double>>(frqs);
    }


    std::shared_ptr<MlxFixedVector<double>> MlxAnalyticsInterface::FFTMagnitude(MlxFixedVector<double> &signal, const double fs)
    {
        MlxMixedRadixRealFFT *_fft = _getFFTWorkspace(signal.size());
        return _fft->normalizedMagnitude(signal);
    }

/*
    std::shared_ptr<MlxFixedVector<double>> MlxAnalyticsInterface::PowerSpectralDensity(MlxFixedVector<double> &signal, const double fs)
    {
    }
*/  

    std::shared_ptr<MlxFixedVector<double>> MlxAnalyticsInterface::PowerSpectralDensity(MlxFixedVector<double> &signal, const double fs, const double df)
    {

        MlxMixedRadixRealFFT *_fft = _getFFTWorkspace(signal.size());      
        return _fft->pwrSpectralDensity(signal, fs, df);
    }
    


    MlxMixedRadixRealFFT* MlxAnalyticsInterface::_getFFTWorkspace(size_t length)
    {

        if (auto search = _fft_real_workspaces.find(length); search != _fft_real_workspaces.end())
        {
            std::cout << "Using existing FFT Workspace!\n";

            // Workspace with given Length already exists
            return search->second;
        }
        else
        {
            // create new Workspace with given Length

            MlxMixedRadixRealFFT *_newWorkspace = new MlxMixedRadixRealFFT(length);

            _fft_real_workspaces.insert(
                std::make_pair(length, _newWorkspace)
            );

            std::cout << "Creating new FFT Workspace!\n";

            return _newWorkspace;
        }

        return nullptr;
    }

    void MlxAnalyticsInterface::_freeFFTWorkspaces()
    {
        for (auto it = _fft_real_workspaces.begin(); it != _fft_real_workspaces.end();)
        {
            delete(it->second);
            _fft_real_workspaces.erase(it);
        }
    }



}   /* namespace mlx */