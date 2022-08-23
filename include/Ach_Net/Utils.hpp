#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "Ach_matrix/Ach_matrix.hpp"
#include <iostream>

namespace Ach_Net::inline Ach_utils
{
    Eigen::MatrixXd one_hot(const Eigen::MatrixXd &input, int class_num)
    {
        Eigen::MatrixXd temp = Eigen::MatrixXd::Zero(input.cols(), class_num);
        for (int i = 0; i < temp.rows(); ++i)
        {
            temp(i, static_cast<int>(input(0, i))) = 1;
        }
        return temp;
    }

    template <typename Func>
    Eigen::MatrixXd matrix_derivate(const Func &function, const Eigen::MatrixXd &X, int f_rows, int f_cols)
    {
        Eigen::MatrixXd _x = std::move(X);
        _x.resize(1, _x.rows() * _x.cols());

        Eigen::MatrixXd res(f_cols * f_rows, _x.cols());
        for (int i = 0; i < res.rows(); ++i)
        {
            res.row(i) = function(_x);
        }
        return res;
    }

    Eigen::MatrixXd standard_linear_function(const Eigen::MatrixXd &W, const Eigen::MatrixXd &X)
    {
        Eigen::MatrixXd res(W.rows(), X.rows());
        for (int i = 0; i < res.rows(); ++i)
        {
            res.row(i) = X.transpose();
        }
        return res;
    }

} // namespace Ach_Net::inline Ach_utils

#endif