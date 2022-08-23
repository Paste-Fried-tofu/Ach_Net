#include "include/Ach_Net/Ach_All.hpp"

int main()
{

    Ach_Net::data_solver data;
    data.get_trainData_csv("/home/tofu/projects/Ach_Net/mnist_data/mnist_train.csv");
    data.get_testData_csv("/home/tofu/projects/Ach_Net/mnist_data/mnist_test.csv");

    Eigen::MatrixXd n_label = Ach_Net::Ach_utils::one_hot(data.get_train_sample().getLabel(), 10);

    Ach_Net::sample train_sample(data.get_train_sample().getFeature(), n_label);

    Ach_Net::Dense dense{
        {Ach_Net::layer(784, Ach_Net::Sigmoid),
         Ach_Net::layer(256, Ach_Net::Sigmoid),
         Ach_Net::layer(10, Ach_Net::Softmax)}};

    Ach_Net::Net net(dense, Ach_Net::cross_entropy_loss, train_sample, 0.005);
    net.train(1, 100);

    return 0;
}