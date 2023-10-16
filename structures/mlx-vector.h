/**
 * @file    mlx-vector.h
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


#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <math.h>
#include <gsl/gsl_vector.h>


namespace mlx
{

    template <typename T>
    class MlxValArray final : public std::valarray<T>
    {

    };

    class MlxVector final
    {
    public:
        MlxVector(size_t size);
        MlxVector(const std::vector<double> vect);

        ~MlxVector();

        size_t size() const;
        double at(size_t idx) const;
        void push_back(double value);
        double pop_back();
        std::vector<double> toStdVector() const;
        gsl_vector* getGslVector() const;

        void reset();


    private:

        void _fromStdVector(const std::vector<double> vect);
        void _allocVector(size_t size);
        void _freeVector();
        
        gsl_vector *_vector;
        std::vector<double> _inner;


    };  /* MlxVector */

    class MlxComplexVector final
    {
    public:
        MlxComplexVector();
        MlxComplexVector(const std::vector<double> real, const std::vector<double> imag);

        ~MlxComplexVector();
    };  /* MlxComplexVector */



    template <
        typename T,
        typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type    
    >
    class MlxFixedVector
    {
    public:
        MlxFixedVector(size_t length) 
        {
            _vector.resize(length);
            reset();
        };

        MlxFixedVector(const std::vector<T>& vect)
        {
            _vector.resize(vect.size());
            std::copy(vect.begin(), vect.end(), _vector.begin());
        }


        MlxFixedVector(const gsl_vector* vect)
        {
            // @todo: check for type of gsl_vector!
            _vector.resize(vect->size);
            memcpy(_vector.data(), vect->data, sizeof(vect->data[0]) * vect->size);
        }


        virtual ~MlxFixedVector() {};
        
        
        size_t size() const
        {
            return _vector.size();
        }


        MlxFixedVector<T> operator* (T value) const
        {
            MlxFixedVector<T> vec(_vector);
            vec *= value;
            return vec;
        }


        MlxFixedVector<T> operator* (const MlxFixedVector<T>& vector) const
        {
            if (vector->size() != _vector.size()) return MlxFixedVector<T>(1);

            MlxFixedVector<T> vec(_vector);
            vec *= vector;
            return vec;
        }


        MlxFixedVector<T> &operator*= (T value)
        {
            std::transform(_vector.begin(), _vector.end(), _vector.begin(), std::bind(std::multiplies<T>(), std::placeholders::_1, value));
            return *this;
        }


        MlxFixedVector<T> &operator*= (const MlxFixedVector<T>& vector)
        {
            if (vector.size() != _vector.size()) return *this;

            for (size_t n = 0; n < _vector.size(); n++)
            {
                _vector.at(n) *= vector.at(n);
            }

            return *this;
        }



        T &operator[] (size_t idx)
        {
            if ((idx < 0) || (idx >= _vector.size()))
            {
                return _vector[0];
            }

            return _vector[idx];
        }


        T at(size_t idx) const
        {
            if (idx < _vector.size()) return _vector.at(idx);
            else return _vector.front();
        }

        MlxFixedVector<T> slice(size_t start, size_t end)
        {
            if  ((start <= 0) || (start >= _vector.size()))
            {
                std::cerr << "Start Index Out of Bound!\n";
                return MlxFixedVector<T>(1);
            }

            if ((end <= 0) || (end >=_vector.size()))
            {
                std::cerr << "End Index Out of Bound!\n";
                return MlxFixedVector<T>(1);
            }

            if ((start - end) == 0)
            {
                std::cerr << "Slice Length is Zero!\n";
                return MlxFixedVector<T>(1);
            }


            size_t dir = (start < end ? 1 : -1);
            size_t p_start = (start < end ? start : end);
            size_t p_end = (start < end ? end : start);
            //size_t N = (start < end ? end - start : start - end);

            MlxFixedVector<T> vec(p_end - p_start);

            size_t i = (dir == 1 ? 0 : (p_end - p_start) -1 );
            for (size_t n = p_start; n < p_end; n ++)
            {
                vec[i] = _vector[n];
                i += dir;
            }

            return vec;
        }


        void set(size_t idx, T value)
        {
            _vector.at(idx) = value;
        }


        void fill(T value)
        {
            std::fill(_vector.begin(), _vector.end(), value);
        }


        void reset()
        {
            fill(0);
        }


        void dump()
        {
            for (const auto& v : _vector) std::cout << v << "\t";
            std::cout << "\n";
        }


        gsl_vector* toGslVector() const
        {
            //std::shared_ptr<gsl_vector> ptr = std::make_shared<gsl_vector>();
            gsl_vector *ptr = gsl_vector_alloc(_vector.size());
            memcpy(ptr->data, _vector.data(), sizeof(T) * _vector.size());
        
            return ptr;
        }


        typename std::vector<T>::iterator begin()
        {
            return _vector.begin();
        }


        typename std::vector<T>::iterator end()
        {
            return _vector.end();
        }

        

    protected:
        std::vector<T> _vector;

    };  /* MlxFixedVector */



    template <class T>
    class MlxWindow final : public MlxFixedVector<T>
    {
    public:
        
        MlxWindow(size_t width) : MlxFixedVector<T>(width) { };

        MlxWindow(const std::vector<T>& vect) : MlxFixedVector<T>(vect) { };

        MlxWindow(const gsl_vector* vect) : MlxFixedVector<T>(vect) { };

        ~MlxWindow() { };


        /**
         * @brief   Move Window Left by N, fill with Zeros
         * 
         * @param   N  - Num of Elements to Shift
         */
        void moveLeft(size_t N)
        {
            if (N >= MlxFixedVector<T>::_vector.size()) return;

            _shiftLeft(N);
            std::fill(MlxFixedVector<T>::_vector.end() - N, MlxFixedVector<T>::_vector.end(), 0);
        }


        /**
         * @brief   Move Window Right by N, fill with Zeros
         * 
         * @param   N   - Num of Elements to Shift
         */
        void moveRight(size_t N)
        {
            if (N >= MlxFixedVector<T>::_vector.size()) return;

            _shiftRight(N);
            std::fill(MlxFixedVector<T>::_vector.begin(), MlxFixedVector<T>::_vector.begin() + N, 0);
        }


        /**
         * @brief   Move Window Left by vect.size(), fill with Values form vect
         * 
         * @param vect  -   Input Vector
         */
        void pushLeft(std::vector<T> vect)
        {
            size_t N = vect.size();

            if (N > MlxFixedVector<T>::_vector.size()) return;

            _shiftLeft(N);
            std::copy(vect.begin(), vect.end(), MlxFixedVector<T>::_vector.end() - N);
        }


        /**
         * @brief   Move Window Right by vect.size(), fill with Values form vect
         * 
         * @param vect  -   Input Vector
         */
        void pushRight(std::vector<T> vect)
        {
            size_t N = vect.size();

            if (N > MlxFixedVector<T>::_vector.size()) return;

            _shiftRight(N);
            std::copy(vect.begin(), vect.end(), MlxFixedVector<T>::_vector.begin());
        }


    private:

        void _shiftLeft(size_t N)
        {
            std::copy(MlxFixedVector<T>::_vector.begin() + N, MlxFixedVector<T>::_vector.end(), MlxFixedVector<T>::_vector.begin());
        }


        void _shiftRight(size_t N)
        {
            std::copy(MlxFixedVector<T>::_vector.begin(), MlxFixedVector<T>::_vector.end() - N, MlxFixedVector<T>::_vector.begin() + N);
        }


    };  /* MlxWindow */


}   /* namespace mlx */