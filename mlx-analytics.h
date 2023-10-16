/**
 * @file     mlx-analytics.h
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

#include <vector>
#include <memory>
#include <unordered_map>


#include "structures/mlx-vector.h"
#include "mlx-fft.h"
#include "mlx-sos-filter.h"


namespace mlx 
{


    class MlxAnalyticsInterface final 
    {
    public:
        MlxAnalyticsInterface(MlxAnalyticsInterface&) = delete;
        void operator= (const MlxAnalyticsInterface) = delete;


        bool setInputVectors(const std::vector<double> vec_time, const std::vector<double> vec_values);
        bool setInputVectors(const MlxVector<double> &vec_time, const MlxVector<double> &vec_values);


        bool applyGaussianFilter(std::shared_ptr<MlxVector<double>> out) const;

        bool applyFFT(std::shared_ptr<MlxVector<double>> out) const;


        /**
         * @brief   Smoothen Signal
         * 
         * @param signal    Signal Vector
         * @param fs        Sample Frequency in Hz
         * @param cutoff    CutOff Frequency
         * @return          Smoothened Signal Vector 
         */
        static std::shared_ptr<MlxVector<double>> smoothening(std::shared_ptr<MlxVector<double>> signal, double fs, double cutoff);


        /**
         * @brief   Calculate FFT Coefficients
         * 
         * @param   signal    Input Signal
         * @param   fs        Sample Frequency
         * @return  std::shared_ptr<MlxFixedVector<double>>
         */
        static std::shared_ptr<MlxFixedVector<double>> FFTMagnitude(MlxFixedVector<double> &signal, const double fs); 


        /**
         * @brief   Calculate FFT Frequencies
         * 
         * @param   signal     Input Signal
         * @param   fs         Sample Frequency
         * @return  std::shared_ptr<MlxFixedVector<double>> 
         */
        static std::shared_ptr<MlxFixedVector<double>> FFTFrequencies(MlxFixedVector<double> &signal, const double fs);


        /**
         * @brief   Calculate Power Spectral Density
         * 
         * @param   signal    Input Signal
         * @param   fs        Sample Frequency
         * @param   df        Spectral Resolution for Power Density - if 0, standard fft spectral resolution is returned
         * @return  std::shared_ptr<MlxFixedVector<double>> 
         */
        //static std::shared_ptr<MlxFixedVector<double>> PowerSpectralDensity(MlxFixedVector<double> &signal, const double fs);
        static std::shared_ptr<MlxFixedVector<double>> PowerSpectralDensity(MlxFixedVector<double> &signal, const double fs, const double df);


        static std::shared_ptr<MlxVector<double>> WVT(std::shared_ptr<MlxDoubleVector> signal, double fs);


        static std::shared_ptr<MlxVector<double>> detectPeaks(std::shared_ptr<MlxDoubleVector> signal);



        static std::shared_ptr<MlxVector<double>> filtfilt(std::shared_ptr<MlxDoubleVector> signal, std::shared_ptr<MlxSOSFilter> filter);

        


    private:
        MlxAnalyticsInterface();
        ~MlxAnalyticsInterface();
        
        std::shared_ptr<MlxDoubleVector> _time;
        std::shared_ptr<MlxDoubleVector> _vals;

        /* Managing of FFT Workspaces */
        static std::unordered_map<size_t, MlxMixedRadixRealFFT*> _fft_real_workspaces;
        static MlxMixedRadixRealFFT* _getFFTWorkspace(size_t length);
        static void _freeFFTWorkspaces();


    }; /* MlxAnalyticsInterface*/


}   /* namespace mlx */