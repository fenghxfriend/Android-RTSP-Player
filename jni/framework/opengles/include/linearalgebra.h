/*******************************************************************************
 *        Module: linearalgebra
 *          File: linearalgebra.h
 * Functionality: linear algebra
 *       Related:
 *        System: generic
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2014-02-26  v1.0        huangxuefeng  created
 * 2014-03-15  v1.1        huangxuefeng  added LU solver and inverse method,
 *                                       implemented in Crout algorithm.
 ******************************************************************************/
#ifndef _ANTS360_LINEARALGEBRA_H_
#define _ANTS360_LINEARALGEBRA_H_

#include <cassert>
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>


namespace ants360 {

template<typename T1, typename T2, typename T3>
bool in_ascend_order(T1 v1, T2 v2, T3 v3)
{
    return v1 <= v2 && v2 <= v3;
}

template<typename Scalar>
class matrix
{
public:
    Scalar * val;
    int rows;
    int cols;

    typedef Scalar value_type;

    matrix() : rows(0), cols(0), val(0) { }

    matrix(int _rows, int _cols)
        : rows(_rows), cols(_cols)
    {
        assert(_rows>0 && _cols>0);
        val = new value_type[rows * cols];
        for (int i = 0; i < rows * cols; ++i)
        {
            val[i] = 0;
        }
    }

    matrix(const matrix & m)
    {
        rows = m.rows;
        cols = m.cols;
        val = new value_type[rows * cols];
        for (int i = 0; i < rows * cols; ++i)
        {
            val[i] = m.val[i];
        }
    }

    ~matrix()
    {
        if (val)
        {
            delete [] val;
        }
    }

    matrix & operator = (const matrix & m)
    {
        if(rows*cols < m.rows * m.cols)
        {
            delete [] val;
            val = new value_type[m.rows * m.cols];
        }
        rows = m.rows;
        cols = m.cols;
        for (int i = 0; i < rows * cols; ++ i)
        {
            val[i] = m.val[i];
        }
        return *this;
    }

    void resize(int rows, int cols, bool reserve_values = true)
    {
        int new_size = rows * cols;
        int this_size = this->rows * this->cols;
        if (this_size >= new_size)
        {
            this->rows = rows;
            this->cols = cols;
            if (!reserve_values)
            {
                for (int i = 0; i < this_size; ++i)
                {
                    val[i] = 0;
                }
            }
        }
        else
        {
            Scalar * new_val = 0;
            if (new_size)
            {
                new_val = new Scalar[new_size];
            }
            if (val)
            {
                if (reserve_values)
                {
                    for (int i = 0; i < this_size; ++i)
                    {
                        new_val[i] = val[i];
                    }
                    for (int i = this_size; i < new_size; ++i)
                    {
                        new_val[i] = 0;
                    }
                }
                else
                {
                    for (int i = 0; i < new_size; ++i)
                    {
                        new_val[i] = 0;
                    }
                }
                delete [] val;
            }
            val = new_val;
            this->rows = rows;
            this->cols = cols;
        }
    }

    int get_rows() const
    { return rows; }

    int get_cols() const
    { return cols; }

    value_type get(int row, int col) const
    {
        assert( in_ascend_order(0, row, rows-1) );
        assert( in_ascend_order(0, col, cols-1) );
        return val[row*cols+col];
    }

    void set(int row, int col, value_type _val)
    {
        assert( in_ascend_order(0, row, rows-1) );
        assert( in_ascend_order(0, col, cols-1) );
        val[row*cols+col] = _val;
    }

    value_type& operator()(int row, int col)
    {
        assert( in_ascend_order(0, row, rows-1) );
        assert( in_ascend_order(0, col, cols-1) );
        return val[row*cols+col];
    }

    const value_type& operator()(int row, int col) const
    {
        assert( in_ascend_order(0, row, rows-1) );
        assert( in_ascend_order(0, col, cols-1) );
        return val[row*cols+col];
    }

    value_type& operator()(int index)
    {
        assert( in_ascend_order(0, index, rows * cols) );
        return val[index];
    }

    const value_type& operator()(int index) const
    {
        assert( in_ascend_order(0, index, rows * cols) );
        return val[index];
    }

    matrix operator + (const matrix & m) const
    {
        assert(cols == m.cols && rows == m.rows);
        matrix result(rows, cols);
        for (int i = 0; i < rows * cols; ++i)
        {
            result.val[i] = val[i] + m.val[i];
        }
        return result;
    }

    matrix operator - (const matrix & m) const
    {
        assert(cols == m.cols && rows == m.rows);
        matrix result(rows, cols);
        for (int i = 0; i < rows * cols; ++i)
        {
            result.val[i] = val[i] - m.val[i];
        }
        return result;
    }

    matrix operator * (const matrix & m) const
    {
        assert(cols == m.rows);
        matrix result(rows, m.cols);
        // TODO (xiezhigang) try to promote the algorithm.
        for (int i = 0; i < rows; ++i)
        {
            for (int k = 0; k < m.cols; ++k)
            {
                for (int j = 0; j < cols; ++j)
                {
                    result.val[ i * result.cols + k ] +=
                        ( val[ i * cols + j ] * m.val[ j * m.cols + k ] );
                }
            }
        }
        return result;
    }

    matrix transpose() const
    {
        matrix result(cols, rows);
        // TODO (xiezhigang) try to promote the algorithm.
        for (int r = 0; r < cols; ++r)
        {
            for (int c = 0; c < rows; ++c)
            {
                result.val[r*rows+c] = val[c*cols+r];
            }
        }
        return result;
    }

    matrix<double> inverse() const;

    template<typename _NewScalarType>
    matrix<_NewScalarType> cast() const
    {
        matrix<_NewScalarType> mat(rows, cols);
        int size = rows * cols;
        for (int i = 0; i < size; ++i)
        {
            mat.val[i] = static_cast<_NewScalarType>(val[i]);
        }
        return mat;
    }

    class CommaIterator
    {
    public:
        matrix& mat;
        int index;

        CommaIterator(matrix & _mat) : mat(_mat), index(0)
        { }

        CommaIterator & operator , (value_type scalar)
        {
            assert(index < mat.rows * mat.cols);
            mat(++index) = scalar;
            return *this;
        }
    };

    CommaIterator operator << (value_type scalar)
    {
        assert(rows > 0 && cols > 0);
        val[0] = scalar;
        CommaIterator ci(*this);
        return ci;
    }

    static matrix Zero(int rows, int cols)
    {
        return matrix(rows, cols);
    }

    static matrix Identity(int rows, int cols)
    {
        matrix result(rows, cols);
        int m = rows < cols ? rows : cols;
        for (int i = 0; i < m; ++i)
        {
            result.val[i * rows + i] = 1;
        }
        return result;
    }
};

template<typename Scalar>
std::ostream& operator << (std::ostream &os, const matrix<Scalar>& mat)
{
    for (int i = 0; i < mat.rows; ++i)
    {
        for (int j = 0; j < mat.cols; ++j)
        {
            os << std::setw(14) << mat(i,j);
        }
        os << std::endl;
    }
    return os;
}

template<typename Scalar1, typename Scalar2>
matrix<Scalar2> operator * (Scalar1 r, const matrix<Scalar2>& mat)
{
    matrix<Scalar2> mat2(mat.rows, mat.cols);
    int size = mat.rows * mat.cols;
    for (int i = 0; i < size; ++i)
    {
        mat2(i) = r * mat(i);
    }
    return mat2;
}

/*!
 * \brief  LU decomposition solver in Crout algorithm.
 * \ref    http://www.sci.utah.edu/~wallstedt/LU.htm 
 * \author huangxuefeng
 * \date   2014-03-15
 */
class LUSolver
{
public:
    bool solvable;
    matrix<double> LU;

    template<typename Scalar>
    LUSolver(const matrix<Scalar> & A) : solvable(false)
    {
        if (A.rows != A.cols || A.rows <= 0)
        {
            return;
        }

        int d = A.rows;
        LU.resize(d, d);
        for (int k = 0; k < d; ++k)
        {
            for (int i = k; i < d; ++i)
            {
                double sum = 0.0;
                for (int p = 0; p < k; ++p)
                {
                    sum += LU(i,p) * LU(p,k);
                }
                LU(i,k) = static_cast<double>(A(i,k)) - sum;
            }
            for (int j = k + 1; j < d; ++j)
            {
                double sum = 0.0;
                for (int p=0; p < k; ++p)
                {
                    sum += LU(k,p) * LU(p,j);
                }
                if (std::abs(LU(k,k)) <
                        std::numeric_limits<double>::epsilon())
                {
                    return;
                }
                LU(k,j) = (A(k,j) - sum) / LU(k,k);
            }
        }
        solvable = true;
    }

    /*!
     * \brief  whether the LU is solvable.
     */
    bool isSolvable() const
    {
        return solvable;
    }

    /*!
     * \brief  solve system of Ax = b.
     * \note   the solver must be solvable, and the rows of b equals rows of A.
     * \param  b : vector or matrix(rows,?).
     * \return x, as vector or matrix(rows,?).
     */
    template<typename Scalar>
    matrix<double> solve(const matrix<Scalar> &b)
    {
        int d = LU.rows;
        matrix<double> x(d,b.cols);
        matrix<double> y(d,b.cols);
        if (solvable && b.rows == d)
        {
            if (b.cols == 1)
            {
                for (int i = 0; i < d; ++i)
                {
                    double sum = 0.0;
                    for (int k = 0; k < i; ++k)
                    {
                        sum += LU(i,k) * y(k);
                    }
                    y(i) = (b(i) - sum) / LU(i,i);
                }
                for (int i = d - 1; i >=0 ; --i)
                {
                    double sum = 0.0;
                    for (int k = i + 1; k < d; ++k)
                    {
                        sum += LU(i,k) * x(k);
                    }
                    // not dividing by diagonals
                    x(i) = y(i) - sum;
                }
            }
            else
            {
                for (int i = 0; i < d; ++i)
                {
                    matrix<double> sum(1,b.cols);
                    for (int j = 0; j < b.cols; ++j)
                    {
                        for (int k = 0; k < i; ++k)
                        {
                            sum(j) += LU(i,k) * y(k,j);
                        }
                        y(i,j) = (b(i,j) - sum(j)) / LU(i,i);
                    }
                }
                for (int i = d - 1; i >=0 ; --i)
                {
                    matrix<double> sum(1,b.cols);
                    for (int j = 0; j < b.cols; ++j)
                    {
                        for (int k = i + 1; k < d; ++k)
                        {
                            sum(j) += LU(i,k) * x(k,j);
                        }
                        // not dividing by diagonals
                        x(i,j) = y(i,j) - sum(j);
                    }
                }
            }
        }
        return x;
    }

    /*!
     * \brief  get the inverse matrix of A: A * A' = I.
     */
    matrix<double> inverse()
    {
        return solve(matrix<double>::Identity(LU.rows, LU.rows));
    }
};

template<typename Scalar>
matrix<double> matrix<Scalar>::inverse() const
{
    return LUSolver(*this).inverse();
}


} // namespace ants360
#endif // _ANTS360_LINEARALGEBRA_H_