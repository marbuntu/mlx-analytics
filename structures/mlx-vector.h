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

#include <valarray>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <math.h>
#include <gsl/gsl_vector.h>


#define _min(x, y) (x < y ? x : y)
#define _max(x, y) (x > y ? x : y)


namespace mlx
{

    template <class T>
    class MlxVector
    {
    public:
        MlxVector(size_t length)
        {
            _checkType();
            _fromFixdLength(length);
        }


        MlxVector(const std::vector<T>& vect)
        {
            _checkType();
            _fromStdVector(vect);
        }


        MlxVector(const gsl_vector* vect)
        {
            _checkType();
            _fromGslVector(vect);
        }


        MlxVector(const std::valarray<T>& vect)
        {
            _checkType();
            _inner = std::valarray<T>(vect);
        }



        ~MlxVector()
        {

        }

    /// VECTOR MODIFIERS

        MlxVector<T> slice(size_t str, size_t end)
        {
            //MlxVector<T> slc(_inner[std::slice(str, end - str, 1)]);
            return slice(str, end, 1);
        }


        MlxVector<T> slice(size_t str, size_t end, size_t stride)
        {
            MlxVector<T> slc(_inner[ std::slice( _min(str, end), _max(str, end) - _min(str, end), stride ) ]    );

            if (end < str) 
            {
                slc.reverse();
            }

            return slc;
        }


        T &operator[] (size_t idx)
        {
            if ((idx < 0 ) || (idx >= _inner.size()))
            {
                return _inner[0];
            }

            return _inner[idx];
        }

        T at(size_t idx) const
        {
            if ((idx < 0 ) || (idx >= _inner.size()))
            {
                return _inner[0];
            }

            return _inner[idx];
        }


        size_t size() const
        {
            return _inner.size();
        }


        void fill(T value)
        {
            std::fill(_inner.begin(), _inner.end(), value);
        }


        /**
         * @brief   Reverse Operation applied to this MlxVector Object
         * 
         */
        void reverse()
        {
            std::reverse(std::begin(_inner), std::end(_inner));
        }


        /**
         * @brief   Returns a Reversed Copy of the Vector Object
         * 
         * @return MlxVector<T> Reversed Copy
         */
        MlxVector<T> reversed() const
        {
            MlxVector<T> rev(*this);
            rev.reverse();
            return rev;
        }



    /// ARITHMETIC FUNCTIONS

        MlxVector<T> operator* (const T value) const
        {
            std::valarray<T> vec = _inner * value;
            return MlxVector<T>(vec);
        }


        MlxVector<T> operator* (const MlxVector<T>& other) const
        {
            if (other->size() != _inner.size()) return MlxVector<T>(1);

            MlxVector<T> vect(_inner);
            vect *= other;
            return vect;
        }


        MlxVector<T> &operator *= (const T value)
        {
            _inner *= value;
            return *this;
        }


        MlxVector<T> &operator *= (const MlxVector<T>& other)
        {
            if (other.size() != _inner.size()) return *this;

            _inner *= other._inner;

            return *this;
        }



    /// GENERAL FUNCTIONS


        /**
         * @brief   Get Vector Sum
         * 
         * @return T 
         */
        T sum() const
        {
            return _inner.sum();
        }


        /**
         * @brief   Get Minimum Element Value
         * 
         * @return T 
         */
        T min() const
        {
            return _inner.min();
        }


        /**
         * @brief   Get Minimum Element Index
         * 
         * @return size_t 
         */
        size_t arg_min() const
        {
            return std::min_element(std::begin(_inner), std::end(_inner)) - std::begin(_inner);
        }


        /**
         * @brief   Get Maximum Element Value
         * 
         * @return T 
         */
        T max() const
        {
            return _inner.max();
        }


        /**
         * @brief   Get Maximum Element Index
         * 
         * @return size_t 
         */
        size_t arg_max() const
        {
            return std::max_element(std::begin(_inner), std::end(_inner)) - std::begin(_inner);
        }


        /**
         * @brief   Calculate Vector Mean 
         * 
         * @return double 
         */
        double mean() const
        {
            return _inner.sum() / _inner.size();
        }


        /**
         * @brief   Calculate Vector Median
         * 
         * @return T 
         */
        T median() const
        {
            std::valarray<T> ocp = std::valarray<T>(_inner);
            auto m = std::begin(ocp) + ocp.size() / 2;
            std::nth_element(std::begin(ocp), m, std::end(ocp));
            return ocp[_inner.size() / 2];
        }

        
        /**
         * @brief   Calculate Vector Variance
         * 
         * @return double 
         */
        double variance() const
        {
            double mean = this->mean();
            std::valarray<T> cp = std::pow((_inner - mean), 2);
            return cp.sum() / cp.size();
        }


        /**
         * @brief   Calculate Vector Standard Deviation
         * 
         * @return double 
         */
        double stddev() const
        {
            return std::sqrt(this->variance());
        }


        /**
         * @brief   Reset All Elements to Zero
         * 
         */
        void reset()
        {
            fill(0);
        }


        /**
         * @brief   Prints the Array to Stream COUT
         * 
         */
        void dumpcout()
        {
            for (const auto& v : _inner) std::cout << v << "\t";
            std::cout << "\n";
        }




    private:

        /**
         * @brief   Internal Array
         * 
         */
        std::valarray<T> _inner;
    
    
        /* Construction Helpers */
        void _checkType()
        {
            static_assert(
                std::is_arithmetic<T>::value, "Template Argument for MlxVector must be of Arithmetic Type!"
            );
        }
        
        void _fromFixdLength(size_t length)
        {
            _inner = std::valarray<T>(length);
        }


        void _fromStdVector(const std::vector<T>& vect)
        {
            _inner = std::valarray<T>(vect);
        }


        void _fromGslVector(const gsl_vector* vect)
        {
            if (typeid(T) != typeid(vect->data[0]))
            {
                std::cerr << __FUNCTION__ << "\t ERROR - Construction Vector must be of same numeric Type!\n";
            }

            _inner = std::valarray<T>(vect->size);
            for (size_t n = 0; n < vect->size; n++) _inner[n] = vect->data[n];
            
            //memcpy(_inner.data(), vect->data, sizeof(T) * vect->size);
        }
        


        /* Shift Functions */
        void _shiftLeft(size_t N)
        {
            std::copy(_inner.begin() + N, _inner.end(), _inner._vector.begin());
        }


        void _shiftRight(size_t N)
        {
            std::copy(_inner.begin(), _inner.end() - N, _inner.begin() + N);
        }







    };  /* MlxVector */


    typedef MlxVector<double>   MlxDoubleVector;
    typedef MlxVector<long>     MlxLongVector;



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