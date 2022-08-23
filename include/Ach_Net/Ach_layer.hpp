#ifndef __ACH_LAYER_HPP__
#define __ACH_LAYER_HPP__

#include "Ach_matrix/Ach_matrix.hpp"
#include "functions/function.hpp"
#include <functional>
#include <vector>

namespace Ach_Net
{
    struct layer
    {
        int neuron_num;
        Eigen::VectorXd values;
        Eigen::MatrixXd weights;
        Eigen::MatrixXd delta_weights;
        Eigen::VectorXd bias;
        Eigen::VectorXd delta_bias;
        Eigen::VectorXd linear_out;

        std::string fun_name;
        std::function<Eigen::MatrixXd(const Eigen::MatrixXd &)> act_fun;
        std::function<Eigen::MatrixXd(const Eigen::MatrixXd &)> derivate_fun;
        layer() = default;
        layer(int neuronNum, const std::string &functionName, bool is_First = false)
            : neuron_num(std::move(neuronNum)), values(std::move(Eigen::VectorXd(neuronNum))),
              bias(std::move(Eigen::VectorXd(neuronNum))), delta_bias(std::move(Eigen::VectorXd(neuronNum))),
              linear_out(std::move(Eigen::VectorXd(neuronNum))), fun_name(std::move(functionName))
        {
            if (!is_First)
                bias = Eigen::MatrixXd::Random(bias.rows(), bias.cols());
            if (functionName == Softmax)
            {
                act_fun = softmax;
                derivate_fun = NULL_fun;
            }
            else if (functionName == Sigmoid)
            {
                act_fun = sigmoid;
                derivate_fun = derivate_sigmoid;
            }
        }
        layer(const layer &t)
        {
            fun_name = std::move(t.fun_name);

            if (fun_name == "softmax")
            {
                act_fun = softmax;
                derivate_fun = NULL_fun;
            }
            else if (fun_name == "sigmoid")
            {
                act_fun = sigmoid;
                derivate_fun = derivate_sigmoid;
            }
            neuron_num = std::move(t.neuron_num);
            values = std::move(t.values);
            bias = std::move(t.bias);
            delta_bias = std::move(t.delta_bias);
        }
        ~layer() = default;
    };

    struct Dense
    {
        std::vector<layer> layer_dense;
    };

} // namespace Ach_Net

#endif