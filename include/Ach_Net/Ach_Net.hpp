#ifndef __ACH_NET_HPP__
#define __ACH_NET_HPP__

#include "Ach_dataSolver/Ach_dataSolver.hpp"
#include "Ach_Net/Ach_layer.hpp"
#include "functions/loss.hpp"
#include "Ach_Net/Ach_grad_list.hpp"
#include "Ach_Net/Utils.hpp"

namespace Ach_Net
{
    class Net
    {

        std::vector<layer> layers;
        sample train_sample;
        Eigen::VectorXd now_feature;
        Eigen::VectorXd now_label;

        double learning_rate;
        loss *loss_fun;
        std::string loss_name;
        grad_list grads;

        void front_propagation()
        {
            for (size_t i = 0; i < layers.size() - 1; ++i)
            {
                layers[i].linear_out = layers[i].weights * layers[i].values + layers[i + 1].bias;
                layers[i + 1].values =
                    layers[i].act_fun(layers[i].linear_out);
            }
        }
        void back_propagation()
        {
            grads.clear_list();
            for (size_t i = layers.size() - 1; i > 0; --i)
            {
                if (i == layers.size() - 1)
                {
                    // push loss function's grad
                    if (layers[i].fun_name == Softmax && loss_name == cross_entropy_loss)
                    {
                        grads.push(CELoss::grad_With_softmax(layers[i].values, now_label).transpose());
                    }
                    else if (layers[i].fun_name != Softmax && loss_name != cross_entropy_loss)
                    {
                        loss_fun->set_Y_pre(layers[i].values);
                        grads.push(loss_fun->grad().transpose());
                        grads.push(matrix_derivate(layers[i].derivate_fun, layers[i].values, layers[i].neuron_num, 1));
                    }
                    else
                    {
                        printf("active function error!\n");
                        abort();
                    }
                }
                // push activate function's grad
                else
                {
                    grads.push(layers[i].weights);

                    grads.push(matrix_derivate(layers[i].derivate_fun, layers[i].values, layers[i].neuron_num, 1));
                }
                Eigen::MatrixXd grad = grads.caculate();

                layers[i].delta_bias = grad.transpose();

                Eigen::MatrixXd _X = standard_linear_function(layers[i - 1].weights, layers[i - 1].values);

                Eigen::MatrixXd temp = matrix_derivate(derivate_linear, _X, layers[i].neuron_num, 1);

                grad = grad * temp;
                grad.resize(layers[i - 1].delta_weights.rows(), layers[i - 1].delta_weights.cols());

                layers[i - 1].delta_weights = grad;
            }
        }

    public:
        Net(const Dense &dense, const std::string &lossFun, const sample &train_sample, double lr = 0.01)
            : train_sample(train_sample), learning_rate(lr), loss_name(lossFun)
        {
            now_feature = train_sample.getFeature().row(0);
            now_label = train_sample.getLabel().row(0);

            if (lossFun == mean_square_loss)
            {
                loss_fun = new MSELoss(now_feature, now_label);
            }
            else if (lossFun == cross_entropy_loss)
            {
                loss_fun = new CELoss(now_feature, now_label);
            }
            layers = dense.layer_dense;

            for (size_t i = 0; i < layers.size() - 1; ++i)
            {
                layers[i].weights = Eigen::MatrixXd::Random(layers[i + 1].neuron_num, layers[i].neuron_num);
                layers[i].delta_weights = Eigen::MatrixXd(layers[i + 1].neuron_num, layers[i].neuron_num);
            }
        }

        void set_train_sample(const sample &train_sample)
        {
            this->train_sample.set_feature(train_sample.getFeature());
            this->train_sample.set_label(train_sample.getLabel());
        }

        void update()
        {
            for (size_t i = 0; i < layers.size() - 1; ++i)
            {
                layers[i].weights += -1 * learning_rate * layers[i].delta_weights;
                layers[i + 1].bias += -1 * learning_rate * layers[i + 1].delta_bias;
            }
        }

        void show_net()
        {
            for (size_t i = 0; i < layers.size(); ++i)
            {
                printf("---------------------------------------------\n");
                std::cout << "layer id: " << i + 1 << std::endl;
                std::cout << "neuron nums: " << layers[i].neuron_num << std::endl;
                std::cout << "values size: (" << layers[i].values.rows() << ", " << layers[i].values.cols() << ")" << std::endl;
                if (i != layers.size() - 1)
                {
                    std::cout << "weights size: (" << layers[i].weights.rows() << ", " << layers[i].weights.cols() << ")" << std::endl;
                }
                if (i != 0)
                {
                    std::cout << "bias size: (" << layers[i].bias.rows() << ", " << layers[i].bias.cols() << ")" << std::endl;
                }
            }
            printf("---------------------------------------------\n");
        }

        void train(int epoches, int each_times)
        {
            show_net();

            for (int cnt = 1; cnt <= epoches; ++cnt)
            {
                std::cout << "Epoches: " << cnt << "/" << epoches << std::endl;

                for (int i = 0; i < train_sample.getFeature().rows(); ++i)
                {
                    now_feature = train_sample.getFeature().row(i);
                    now_label = train_sample.getLabel().row(i).transpose();
                    now_feature.normalize();
                    layers[0].values = now_feature.transpose();

                    loss_fun->set_Y(now_label);

                    for (int times = 1; times <= each_times; ++times)
                    {
                        std::cout << "\tTimes: " << times << std::endl;
                        this->front_propagation();
                        loss_fun->set_Y_pre(layers[layers.size() - 1].values);
                        std::cout << "\tLoss: " << loss_fun->operator()() << std::endl;
                        // std::cout << "\toutput: " << layers[layers.size() - 1].values.transpose() << std::endl;
                        this->back_propagation();
                        this->update();
                    }
                }
            }
        }
    };
}

#endif