#ifndef __ACH_GRAD_LIST_HPP__
#define __ACH_GRAD_LIST_HPP__

#include <iostream>
#include <vector>
#include <algorithm>

namespace Ach_Net::inline Ach_grad_list
{

    class grad_list
    {
    private:
        std::vector<Eigen::MatrixXd> grads;

    public:
        grad_list() = default;
        ~grad_list() = default;
        void push(const Eigen::MatrixXd &grad)
        {
            grads.emplace_back(grad);
        }
        Eigen::MatrixXd caculate()
        {
            Eigen::MatrixXd res = grads[0];

            for (size_t i = 1; i < grads.size(); i++)
            {
                res = res * grads[i];
            }
            return res;
        }
        void clear_list()
        {
            grads.clear();
        }
    };
} // namespace Ach_Net::inline Ach_grad_list

#endif