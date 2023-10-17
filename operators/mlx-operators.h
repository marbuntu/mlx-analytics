

#include "../structures/mlx-vector.h"

#pragma once

namespace mlx
{
    typedef enum
    {
        MODE_FULL = 0,       // Run Operation over full length (can result in len(output)  > len(input))
        MODE_EQUAL = 1,      // Keep len(output) == len(input)
        MODE_OVERLAP = 2,    // Run Operation only in fully overlapping parts of the signal
    } MlxOverlapMode_t;



    /**
     * @brief 
     * 
     * @tparam T 
     * @param vect1 
     * @param vect2 
     * @param mode 
     * @return MlxVector<T> 
     */
    template <class T>
    MlxVector<T> convolve(const MlxVector<T> vect1, const MlxVector<T> vect2, MlxOverlapMode_t mode);





}   /* namespace mlx */

#include "mlx-operators.cc"

