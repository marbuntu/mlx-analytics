/**
 * @file    mlx-window-statistics.cc
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
 
#include "mlx-window-statistics.h"

namespace mlx
{

    MlxMovingWindowWorkspace::MlxMovingWindowWorkspace(size_t N)
    {
        _w = gsl_movstat_alloc(N);
        std::cout << "Creating new Moving Window Workspace! \n";
    }

    MlxMovingWindowWorkspace::~MlxMovingWindowWorkspace()
    {
        gsl_movstat_free(_w);
        _w = nullptr;
    }


    gsl_movstat_workspace* MlxMovingWindowWorkspace::get() const
    {
        return _w;
    }


/// MlxMovingWindow +++++++++++++

    std::unordered_map<size_t, MlxMovingWindowWorkspace*> MlxMovingWindow::_workspaces = {};


    MlxMovingWindow::MlxMovingWindow()
    {
    }


    MlxMovingWindow::~MlxMovingWindow()
    {
        _freeWorkspaces();
    }


    MlxDoubleVector MlxMovingWindow::Maxima(const MlxDoubleVector& vect, size_t width, size_t stride)
    {
        return Maxima(vect, width, stride, EdgeModeDefault);
    }


    MlxDoubleVector MlxMovingWindow::Maxima(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode)
    {
        gsl_movstat_workspace *wspc = _getWorkspace(width)->get();

        gsl_vector *in = vect.toGslVector();
        MlxDoubleVector out(vect.size());

        gsl_movstat_max((gsl_movstat_end_t) mode, in, out.asGslVector(), wspc);

        return out;
    }


    MlxDoubleVector MlxMovingWindow::Minima(const MlxDoubleVector& vect, size_t width, size_t stride)
    {
        return Minima(vect, width, stride, EdgeModeDefault);
    }

    
    MlxDoubleVector MlxMovingWindow::Minima(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode)
    {
        gsl_movstat_workspace *wspc = _getWorkspace(width)->get();

        gsl_vector *in = vect.toGslVector();
        MlxDoubleVector out(vect.size());
        
        gsl_movstat_min((gsl_movstat_end_t) mode, in, out.asGslVector(), wspc);

        return out;
    }


    MlxDoubleVector MlxMovingWindow::Median(const MlxDoubleVector& vect, size_t width, size_t stride)
    {
        return Median(vect, width, stride, EdgeModeDefault);
    }


    MlxDoubleVector MlxMovingWindow::Median(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode)
    {
        gsl_movstat_workspace *wspc = _getWorkspace(width)->get();

        gsl_vector *in = vect.toGslVector();
        MlxDoubleVector out(vect.size());
        
        gsl_movstat_median((gsl_movstat_end_t) mode, in, out.asGslVector(), wspc);

        return out;
    }
    

    MlxDoubleVector MlxMovingWindow::Mean(const MlxDoubleVector& vect, size_t width, size_t stride)
    {
        return Mean(vect, width, stride, EdgeModeDefault);
    }


    MlxDoubleVector MlxMovingWindow::Mean(const MlxDoubleVector& vect, size_t width, size_t stride, EdgeMode mode)
    {
        gsl_movstat_workspace *wspc = _getWorkspace(width)->get();

        gsl_vector *in = vect.toGslVector();
        MlxDoubleVector out(vect.size());
        
        gsl_movstat_mean((gsl_movstat_end_t) mode, in, out.asGslVector(), wspc);

        return out;
    }


    MlxMovingWindowWorkspace* MlxMovingWindow::_getWorkspace(size_t N)
    {
        if (auto search = _workspaces.find(N); search != _workspaces.end())
        {
            return search->second;
        }

        MlxMovingWindowWorkspace *newspc = new MlxMovingWindowWorkspace(N);
        _workspaces.insert(std::make_pair(N, newspc));

        return newspc;
    }


    void MlxMovingWindow::_freeWorkspaces()
    {
        for (auto it = _workspaces.begin(); it != _workspaces.end();)
        {
            delete(it->second);
            _workspaces.erase(it);
        }
    }


}   /* namespace mlx */