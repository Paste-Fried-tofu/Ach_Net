#ifndef __LOSS_HPP__
#define __LOSS_HPP__

#include <eigen3/Eigen/Dense>
#include <cmath>

namespace Ach_Net::inline Ach_function
{
    constexpr char cross_entropy_loss[] = "CELoss";
    constexpr char mean_square_loss[] = "MSELoss";
    class loss
    {
    protected:
        Eigen::MatrixXd y_hat;
        Eigen::MatrixXd y;

    public:
        loss() = default;
        loss(const Eigen::MatrixXd &Y_pre, const Eigen::MatrixXd &Y) : y_hat(Y_pre), y(Y) {}
        void set_Y_pre(const Eigen::MatrixXd &Y_pre)
        {
            y_hat = Y_pre;
        }
        void set_Y(const Eigen::MatrixXd &Y)
        {
            y = Y;
        }
        Eigen::MatrixXd get_Y()
        {
            return y;
        }
        Eigen::MatrixXd get_Y_pre()
        {
            return y_hat;
        }
        virtual double operator()() = 0;
        virtual Eigen::MatrixXd grad() const = 0;
    };

    class MSELoss : public loss
    {
    public:
        MSELoss() = default;
        MSELoss(const Eigen::MatrixXd &Y_pre, const Eigen::MatrixXd &Y) : loss(Y_pre, Y) {}
        double operator()()
        {
            return (y.array() - y_hat.array()).pow(2).sum() * 0.5;
        }
        Eigen::MatrixXd grad() const
        {
            return (y_hat - y);
        }
    };

    class CELoss : public loss
    {
    public:
        CELoss() = default;
        CELoss(const Eigen::MatrixXd &Y_pre, const Eigen::MatrixXd &Y) : loss(Y_pre, Y) {}
        double operator()()
        {
            return std::abs(-1 * (y.array() * y_hat.array()).sum());
        }
        Eigen::MatrixXd grad() const
        {
            return -1 * (y.array() / y_hat.array());
        }
        static inline Eigen::MatrixXd grad_With_softmax(const Eigen::MatrixXd &Y_pre, const Eigen::MatrixXd &Y)
        {
            return Y_pre - Y;
        }
    };
}

#endif