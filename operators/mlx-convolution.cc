

#include <iostream>
#include <fstream>
#include <sstream>

#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include "mlx-convolution.h"


namespace mlx
{

#define REAL(z,i) ((z)[2*(i)])
#define IMAG(z,i) ((z)[2*(i)+1])


void run_gsl_convolution_test()
{

    std::cout << "Run Convolution TEST\r\n";

    const size_t N = 400;
    const size_t N_2 = (N >> 1);
    double val_dat = 0.0;
    double val_win = 0.0;

    size_t lim = (size_t) 150;

    gsl_fft_complex_wavetable *_wvt = gsl_fft_complex_wavetable_alloc(N);
    gsl_fft_complex_workspace *_wrk = gsl_fft_complex_workspace_alloc(N);
    
    //gsl_vector_complex *vec_dat = gsl_vector_complex_alloc(N);
    //gsl_vector_complex *vec_win = gsl_vector_complex_alloc(N);

    MlxDoubleVector vec_dat(N);
    MlxDoubleVector vec_win(N);

    std::ofstream out_test("conv-signal.txt");

    for (size_t i = 0; i < N; i++)
    {
        double val_dat = 0.0;
        double val_win = 0.0;

        if (i < N_2) 
        {
            val_dat = i / 400.0;
        }
        else
        {
            val_dat = (400.0 - i) / 400.0;
        }


        if ((i > lim) && (i < (N - lim)))
        {
            val_win = 1.0;
        }


        //gsl_vector_complex_set(vec_dat, i, gsl_complex_rect( val_dat, 0.0 ));
        //gsl_vector_complex_set(vec_win, i, gsl_complex_rect( val_win, 0.0 ));

        vec_dat[i] = val_dat;
        vec_win[i] = val_win;

        out_test << i << "\t" << val_dat << "\t" << val_win << "\n";
    }

    out_test.close();

    /*
    gsl_vector* inp; //signal.toGslVector();


    gsl_fft_complex_forward(vec_dat->data, 1, N, _wvt, _wrk);
    gsl_fft_complex_forward(vec_win->data, 1, N, _wvt, _wrk);

    // **2 for all Vector Elements

    std::ofstream fft_out("conv-fft.txt");
    for (size_t i = 0; i < N; i++)
    {
        fft_out << i << "\t" << GSL_REAL(gsl_vector_complex_get(vec_dat, i)) << "\t" << GSL_IMAG(gsl_vector_complex_get(vec_dat, i)) << "\t" << GSL_REAL(gsl_vector_complex_get(vec_win, i)) << "\t" << GSL_IMAG(gsl_vector_complex_get(vec_win, i)) << "\n";
    }
    fft_out.close();

    int ret = gsl_vector_complex_mul(vec_dat, vec_win);
    
    std::cout << "Mul ret " << ret << "\n";

    gsl_fft_complex_inverse(vec_dat->data, 1, N, _wvt, _wrk);


    std::ofstream conv_out("conv-result.txt");
    for (size_t n = 0; n < N; n++)
    {
        double rel = GSL_REAL(gsl_vector_complex_get(vec_dat, n));
        double img = GSL_IMAG(gsl_vector_complex_get(vec_dat, n));

        conv_out << sqrt(pow(rel, 2) + pow(img, 2)) << "\t" << rel << "\t" << img << "\t" << GSL_REAL(gsl_vector_complex_get(vec_win, n)) << "\n";
    }

    conv_out.close();

    //std::shared_ptr<MlxDoubleVector> res = std::make_shared<MlxDoubleVector>(integ);

    */

    MlxDoubleVector res = convolve1D(vec_dat, vec_win);

    //gsl_vector_free(inp);

    gsl_fft_complex_wavetable_free(_wvt);
    gsl_fft_complex_workspace_free(_wrk);

}


MlxDoubleVector convolve1D(const MlxDoubleVector& signal, const MlxDoubleVector& kernel)
{
    // First start with the valid range
    size_t M = std::max(signal.size(), kernel.size());
    size_t K = std::min(signal.size(), kernel.size());
    size_t N = M + K;

    gsl_fft_complex_wavetable *_wvt = gsl_fft_complex_wavetable_alloc(N);
    gsl_fft_complex_workspace *_wrk = gsl_fft_complex_workspace_alloc(N);

    gsl_vector_complex *vec_sig = gsl_vector_complex_alloc(N);
    gsl_vector_complex_set_zero(vec_sig);

    gsl_vector_complex *vec_krn = gsl_vector_complex_alloc(N);
    gsl_vector_complex_set_zero(vec_krn);

    for (size_t n = 0; n < M; n++)
    {
        if (n < signal.size()) gsl_vector_complex_set(vec_sig, n, gsl_complex_rect(signal.at(n), 0.0));
        if (n < kernel.size()) gsl_vector_complex_set(vec_krn, n, gsl_complex_rect(kernel.at(n), 0.0));
    }

    gsl_fft_complex_forward(vec_sig->data, 1, N, _wvt, _wrk);
    gsl_fft_complex_forward(vec_krn->data, 1, N, _wvt, _wrk);

    gsl_vector_complex_mul(vec_sig, vec_krn);

    gsl_fft_complex_inverse(vec_sig->data, 1, N, _wvt, _wrk);

    MlxDoubleVector res(M);
    size_t K_2 = (K >> 1);

    std::ofstream conv_out("conv-result.txt");
    for (size_t m = 0; m < M; m++)
    {
        res[m] = GSL_REAL(gsl_vector_complex_get(vec_sig, K_2 + m));
        conv_out << res[m] << "\n";
    }

    conv_out.close();

    // Clean
    gsl_vector_complex_free(vec_sig);
    gsl_vector_complex_free(vec_krn);
    gsl_fft_complex_wavetable_free(_wvt);
    gsl_fft_complex_workspace_free(_wrk);

    return res;
}


} /* namespace mlx */