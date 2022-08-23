#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include <eigen3/Eigen/Dense>

namespace Ach_Net::inline Ach_function
{

    constexpr char Sigmoid[] = "sigmoid";
    constexpr char Softmax[] = "softmax";

    Eigen::MatrixXd NULL_fun(const Eigen::MatrixXd &X)
    {
        Eigen::MatrixXd a;
        return a;
    }

    Eigen::MatrixXd derivate_linear(const Eigen::MatrixXd &X)
    {
        return X;
    }

    Eigen::MatrixXd sigmoid(const Eigen::MatrixXd &X)
    {
        return (1 / (1 + (-1 * X).array().exp()));
    }

    Eigen::MatrixXd derivate_sigmoid(const Eigen::MatrixXd &X)
    {
        Eigen::MatrixXd temp = sigmoid(X);
        return temp.array() * (1 + (-1 * temp).array());
    }

    Eigen::MatrixXd softmax(const Eigen::MatrixXd &X)
    {
        return X / X.sum();
    }

} // namespace Ach_Net::function

#endif