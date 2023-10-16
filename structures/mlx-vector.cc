/**
 * @file    mlx-vector.cc
 * @brief   Wrapper for gsl_vector
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


#include "mlx-vector.h"

#include <cstring>
#include <algorithm>


namespace mlx
{

    MlxVector::MlxVector(size_t size)
    : _vector(nullptr)
    {
        _allocVector(size);
    }


    MlxVector::MlxVector(const std::vector<double> vect)
    : _vector(nullptr)
    {
        _fromStdVector(vect);
    }


    MlxVector::~MlxVector()
    {
        _freeVector();
    }


    size_t MlxVector::size() const 
    {
        if (_vector == nullptr) return 0;

        return _vector->size;
    }


    double MlxVector::at(size_t idx) const
    {
        if (idx >= _vector->size) return 0.0;

        return _vector->data[idx];
    }


    std::vector<double> MlxVector::toStdVector() const
    {
        std::vector<double> out;
        out.resize(_vector->size);

        memcpy(out.data(), _vector->data, _vector->size * sizeof(double));

        return out;
    }


    gsl_vector* MlxVector::getGslVector() const
    {
        return _vector;
    }


    void MlxVector::reset()
    {
        memset(_vector->data, 0, sizeof(double) * _vector->size);
    }


    void MlxVector::push_back(double value)
    {
        //gsl_vector_add_constant(_)
    }


    void MlxVector::_fromStdVector(const std::vector<double> vect)
    {
        _freeVector();
        _allocVector(vect.size());
        memcpy(_vector->data, vect.data(), vect.size() * sizeof(double));
    }


    void MlxVector::_allocVector(size_t size)
    {
        _vector = gsl_vector_alloc(size);
    }


    void MlxVector::_freeVector()
    {
        if (_vector != nullptr)
        {
            gsl_vector_free(_vector);
            _vector = nullptr;
        }
    }



/// Start - MlxWindow


//template <class T = double>
//MlxFixedVector<double>::MlxFixedVector(size_t length)

/*
    template <class T>
    void MlxWindow<T>::move(size_t N)
    {
        if (N >= _window.size()) return;

        std::copy(_window.end() - N, _window.end(), _window.begin());
        std::fill(_window.end() - N, _window.end(), 0);
    }


    template <class T>
    void MlxWindow<T>::push(std::vector<T> vect)
    {
        
    }


    template <class T>
    void MlxWindow<T>::fill(T value)
    {
        std::fill(_window.begin(), _window.end(), value);
    }


    template <class T>
    void MlxWindow<T>::reset()
    {
        fill(0);
    }
*/

/// End - MlxWindow



}   /* namespace mlx */