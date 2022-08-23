#ifndef __ACH_MATRIX_HPP__
#define __ACH_MATRIX_HPP__

#include <cstdint>
#include <cstring>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

namespace Ach_Net::inline Ach_matrix
{
    template <typename Type>
    class matrix
    {
    private:
        Type **Matrix;
        int row;
        int col;

    public:
        matrix()
        {
            Matrix = nullptr;
            row = 0;
            col = 0;
        }
        matrix(int rows, int cols) : row(rows), col(cols)
        {
            Matrix = new Type *[row];
            for (int i = 0; i < row; ++i)
            {
                Matrix[i] = new Type[col];
                std::fill(Matrix[i], Matrix[i] + col, 0);
            }
        }
        matrix(const matrix<Type> &M)
        {
            if (M.Matrix == nullptr)
            {
                this->Matrix = nullptr;
                col = 0;
                row = 0;
            }
            else if (this != &M)
            {
                col = M.col;
                row = M.row;
                Matrix = new Type *[row];
                for (int i = 0; i < row; ++i)
                {
                    Matrix[i] = new Type[col];
                    for (int j = 0; j < col; ++j)
                    {
                        Matrix[i][j] = M.Matrix[i][j];
                    }
                }
            }
        }
        ~matrix()
        {
            if (Matrix != nullptr)
            {
                for (int i = 0; i < row; ++i)
                {
                    delete[] Matrix[i];
                }
                delete[] Matrix;
            }
        }

        void matrix_uniform_random(double low, double up)
        {
            std::random_device rd;
            std::uniform_real_distribution<> dis(low, up);
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    Matrix[i][j] = dis(rd);
                }
            }
        }

        void ones()
        {
            for (int i = 0; i < row; ++i)
            {
                Matrix[i][i] = 1;
            }
        }

        Type *operator[](int i) const
        {
            if (i < row)
            {
                return this->Matrix[i];
            }
            else
            {
                printf("Overflow!");
                abort();
            }
        }

        friend std::ostream &operator<<(std::ostream &out, const matrix<Type> &M)
        {
            for (int i = 0; i < M.row; ++i)
            {
                for (int j = 0; j < M.col; ++j)
                {
                    out << M.Matrix[i][j] << ' ';
                }
                out << std::endl;
            }
            return out;
        }

        matrix<Type> &operator=(const matrix<Type> &M)
        {

            if (M.Matrix == nullptr)
            {
                this->Matrix = nullptr;
                col = 0;
                row = 0;
            }
            else if (this != &M)
            {
                col = M.col;
                row = M.row;
                if (Matrix == nullptr)
                {
                    Matrix = new Type *[row];
                    for (int i = 0; i < row; ++i)
                    {
                        Matrix[i] = new Type[col];
                        for (int j = 0; j < col; ++j)
                        {
                            Matrix[i][j] = M.Matrix[i][j];
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < row; ++i)
                    {
                        delete[] Matrix[i];
                    }
                    delete[] Matrix;
                    Matrix = new Type *[row];
                    for (int i = 0; i < row; ++i)
                    {
                        Matrix[i] = new Type[col];
                        for (int j = 0; j < col; ++j)
                        {
                            Matrix[i][j] = M.Matrix[i][j];
                        }
                    }
                }
            }
            return *this;
        }

        int cols() const
        {
            return col;
        }
        int rows() const
        {
            return row;
        }

        matrix<Type> operator+(const double &a) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] += a;
                }
            }
            return temp;
        }

        matrix<Type> operator+(const matrix<Type> &M) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] += M.Matrix[i][j];
                }
            }
            return temp;
        }

        matrix<Type> operator-(const matrix<Type> &M) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] -= M.Matrix[i][j];
                }
            }
            return temp;
        }

        friend matrix<Type> operator+(const double &a, const matrix<Type> &M)
        {
            matrix<Type> temp = M;
            for (int i = 0; i < M.row; ++i)
            {
                for (int j = 0; j < M.col; ++j)
                {
                    temp[i][j] += a;
                }
            }
            return temp;
        }

        matrix<Type> operator*(const double &a) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] *= a;
                }
            }
            return temp;
        }

        friend matrix<Type> operator*(const double &a, const matrix<Type> &M)
        {
            matrix<Type> temp = M;
            for (int i = 0; i < M.row; ++i)
            {
                for (int j = 0; j < M.col; ++j)
                {
                    temp[i][j] *= a;
                }
            }
            return temp;
        }

        matrix<Type> operator*(const matrix<Type> &M) const
        {
            if (this->row != M.col)
            {
                printf("Can't do matrix multiply because the row != col...\n");
                std::cout << "first matrix size: " << std::endl
                          << "rows=" << row << ' ' << "cols=" << col << std::endl;
                std::cout << "second matrix size: " << std::endl
                          << "rows=" << M.row << ' ' << "cols=" << M.col << std::endl;
                abort();
            }
            matrix<Type> temp(this->row, M.col);
            for (int i = 0; i < this->row; ++i)
            {
                for (int j = 0; j < this->col; ++j)
                {
                    for (int k = 0; k < M.row; ++k)
                    {
                        temp[i][j] += this->Matrix[i][k] * M.Matrix[k][j];
                    }
                }
            }
            return temp;
        }

        matrix<Type> dot(const matrix<Type> &M) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] *= M.Matrix[i][j];
                }
            }
            return temp;
        }

        matrix<Type> exp() const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] = std::exp(static_cast<double>(Matrix[i][j]));
                }
            }
            return temp;
        }

        matrix<Type> log() const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] = std::log(static_cast<double>(Matrix[i][j]));
                }
            }
            return temp;
        }

        Type sum() const
        {
            Type s = 0;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    s += Matrix[i][j];
                }
            }
            return s;
        }

        matrix<Type> operator/(const double &x) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] = static_cast<double>(temp[i][j]) / x;
                }
            }
            return temp;
        }

        friend matrix<Type> operator/(const double &x, const matrix<Type> &M)
        {
            matrix<Type> temp = M;
            for (int i = 0; i < M.row; ++i)
            {
                for (int j = 0; j < M.col; ++j)
                {
                    temp[i][j] = x / static_cast<double>(temp[i][j]);
                }
            }
            return temp;
        }

        matrix<Type> operator/(const matrix<Type> &M) const
        {
            matrix<Type> temp = *this;
            for (int i = 0; i < row; ++i)
            {
                for (int j = 0; j < col; ++j)
                {
                    temp[i][j] /= M.Matrix[i][j];
                }
            }
            return temp;
        }
    };

    typedef matrix<double> matrixd;

} // namespace Ach_Net::inline Ach_matrix

#endif