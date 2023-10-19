/**
 * @file    mlx-window-statistics.h
 * @brief   Moving Window Statistics
 * 
 * @version 1.0
 * @date    2023-10-17
 * 
 * @author  M. Anschuetz (marbuntu)
 * 
 *
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once

#include "gsl/gsl_movstat.h"
#include "../structures/mlx-vector.h"

namespace mlx
{

class MlxMovingWindowWorkspace
{
public:
    MlxMovingWindowWorkspace(size_t N);
    ~MlxMovingWindowWorkspace();

    gsl_movstat_workspace* get() const;

private:
    gsl_movstat_workspace *_w;

};  /* MlxMovingWindowWorkspace */


class MlxMovingWindow
{
public:

    typedef enum {
        ZEROPADDING = GSL_MOVSTAT_END_PADZERO,
        VALUEPADDING = GSL_MOVSTAT_END_PADVALUE,
        TRUNCATE = GSL_MOVSTAT_END_TRUNCATE,
    } EdgeMode;


    static const EdgeMode EdgeModeDefault = VALUEPADDING;


    static MlxDoubleVector Maxima(const MlxDoubleVector& vect, size_t width, size_t stride);
    static MlxDoubleVector Maxima(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode);


    static MlxDoubleVector Minima(const MlxDoubleVector& vect, size_t width, size_t stride);
    static MlxDoubleVector Minima(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode);


    static MlxDoubleVector Median(const MlxDoubleVector& vect, size_t width, size_t stride);
    static MlxDoubleVector Median(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode);

    static MlxDoubleVector Mean(const MlxDoubleVector& vect, size_t width, size_t stride);
    static MlxDoubleVector Mean(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode);


    static MlxDoubleVector MAD(const MlxDoubleVector& vect, size_t width, size_t stride);
    static MlxDoubleVector MAD(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode);




protected:
    MlxMovingWindow();
    ~MlxMovingWindow();

    static std::unordered_map<size_t, MlxMovingWindowWorkspace*> _workspaces;
    static MlxMovingWindowWorkspace* _getWorkspace(size_t length);
    static void _freeWorkspaces();


};  /* MlxMovingWindow */


}   /* namespace mlx */