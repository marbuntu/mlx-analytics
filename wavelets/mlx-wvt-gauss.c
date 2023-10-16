



#include "mlx-wvt-gauss.h"
#include <string.h>


static double mlx_default_gaussian_wavelet_alpha = 10.5;


void generate_gauss_wavelet(const double alpha, size_t N, gsl_vector **out)
{
    *out = gsl_vector_alloc(N);
    gsl_filter_gaussian_kernel(alpha, 0, 1, *out);
}


gsl_wavelet* mlx_gauss_wavelet_allocate(const double alpha, size_t N_half)
{
    gsl_wavelet *wvt;
    gsl_vector *vec;
    size_t N = (N_half << 1);
    generate_gauss_wavelet(alpha, N, &vec);

    double *h = calloc(N, sizeof(double));
    double *g = calloc(N, sizeof(double));

    //memcpy((void*) wvt->g1, &vec->data[0], N * sizeof(double));
    //memcpy((void*) wvt->h1, &vec->data[N_half], N * sizeof(double));

    for (size_t n = 0; n < N; n++)
    {
        h[n] = vec->data[n];
        g[n] = vec->data[N - n];
    }

    gsl_vector_free(vec);
    
    *(&wvt->h1) = h;
    *(&wvt->g1) = g;
    wvt->h2 = wvt->h1;
    wvt->g2 = wvt->g1;

    wvt->nc = (N_half << 1);
    wvt->offset = 0;

    return wvt;
}


void mlx_gauss_wavelet_free(gsl_wavelet* wavelet)
{


}


static int
gaussian_init (const double **h1, const double **g1,
                          const double **h2, const double **g2, size_t * nc,
                          size_t * offset, size_t member)
{

    gsl_vector *vec;
    size_t N = (member << 1);
    generate_gauss_wavelet(mlx_default_gaussian_wavelet_alpha, N, &vec);

    double *h = calloc(N, sizeof(double));
    double *g = calloc(N, sizeof(double));

    //memcpy((void*) wvt->g1, &vec->data[0], N * sizeof(double));
    //memcpy((void*) wvt->h1, &vec->data[N_half], N * sizeof(double));

    for (size_t n = 0; n < N; n++)
    {
        h[n] = vec->data[n];
        g[n] = vec->data[N - n];
    }

    *h1 = h;
    *g1 = g;
    *h2 = h;
    *g2 = g;

    *nc = N;
    *offset = 0;

    return GSL_SUCCESS;
}



static int
gaussian_centered_init (const double **h1, const double **g1,
                          const double **h2, const double **g2, size_t * nc,
                          size_t * offset, size_t member)
{

    gsl_vector *vec;
    size_t N = 100; //(member << 1);
    mlx_default_gaussian_wavelet_alpha = (double) N / member;
    generate_gauss_wavelet(mlx_default_gaussian_wavelet_alpha, N, &vec);

    double *h = calloc(N, sizeof(double));
    double *g = calloc(N, sizeof(double));

    //memcpy((void*) wvt->g1, &vec->data[0], N * sizeof(double));
    //memcpy((void*) wvt->h1, &vec->data[N_half], N * sizeof(double));

    for (size_t n = 0; n < N; n++)
    {
        h[n] = -vec->data[n];
        g[n] = -vec->data[N - n];
    }

    *h1 = h;
    *g1 = g;
    *h2 = h;
    *g2 = g;

    *nc = N;
    *offset = N >> 1;

    return GSL_SUCCESS;
}


static const gsl_wavelet_type gaussian_type = {
  "gaussian",
  &gaussian_init
};

static const gsl_wavelet_type gaussian_centered_type = { 
  "gaussian-centered",
  &gaussian_centered_init
};

const gsl_wavelet_type *mlx_wavelet_gaussian = &gaussian_type;
const gsl_wavelet_type *mlx_wavelet_gaussian_centered = &gaussian_centered_type;


/*
Pinzetten Heraussuchen, Schneidepinzette

Lötstation, Lötzinn

Fliegenbeine

Oszilloskope
*/

