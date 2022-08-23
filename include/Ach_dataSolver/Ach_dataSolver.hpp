#ifndef __DATA_SOLVER_HPP__
#define __DATA_SOLVER_HPP__

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <iostream>

namespace Ach_Net::inline Ach_data
{
    class sample
    {
        Eigen::MatrixXd feature;
        Eigen::MatrixXd label;

    public:
        sample() = default;
        sample(const sample &s) : feature(s.feature), label(s.label) {}
        sample(const Eigen::MatrixXd &m_feature, const Eigen::MatrixXd &m_label) : feature(m_feature), label(m_label) {}
        Eigen::MatrixXd getFeature() const
        {
            return feature;
        }
        Eigen::MatrixXd getLabel() const
        {
            return label;
        }
        void set_feature(const Eigen::MatrixXd &m_feature)
        {
            feature = std::move(m_feature);
        }
        void set_label(const Eigen::MatrixXd &m_label)
        {
            label = std::move(m_label);
        }
    };

    class data_solver
    {
        sample test_sample;
        sample train_sample;

    public:
        data_solver() = default;
        bool get_trainData_csv(const std::string &filename)
        {
            std::ifstream in(filename, std::ios::in);
            if (!in.is_open())
            {
                std::cout << "Open " << filename << " failed..." << std::endl;
                std::cout << "Please check the path" << std::endl;
                return false;
            }
            std::vector<std::vector<double>> temp;
            std::string str;
            std::string line;
            while (getline(in, line))
            {
                std::stringstream ss(line);
                temp.emplace_back(std::vector<double>());
                while (getline(ss, str, ','))
                {
                    temp.back().push_back(std::stod(str));
                }
            }

            in.close();
            Eigen::MatrixXd feature(temp.size(), temp.back().size() - 1);
            Eigen::MatrixXd label(1, temp.size());
            for (size_t i = 0; i < temp.size(); ++i)
            {
                label(0, i) = temp[i][0];
                for (size_t j = 0; j < temp[i].size() - 1; ++j)
                {
                    feature(i, j) = temp[i][j + 1];
                }
            }
            train_sample.set_feature(feature);
            train_sample.set_label(label);
            return true;
        }
        bool get_testData_csv(const std::string &filename)
        {
            std::ifstream in(filename, std::ios::in);
            if (!in.is_open())
            {
                std::cout << "Open " << filename << " failed..." << std::endl;
                std::cout << "Please check the path" << std::endl;
                return false;
            }
            std::vector<std::vector<double>> temp;
            std::string str;
            std::string line;
            while (getline(in, line))
            {
                std::stringstream ss(line);
                temp.emplace_back(std::vector<double>());
                while (getline(ss, str, ','))
                {
                    temp.back().emplace_back(std::stod(str));
                }
            }
            in.close();

            // for (size_t i = 0; i < temp.size(); ++i)
            // {
            //     std::cout << "len: " << temp[i].size() << std::endl;
            //     for (size_t j = 0; j < temp[i].size(); ++j)
            //     {
            //         std::cout << temp[i][j] << ' ';
            //     }
            //     std::cout << std::endl;
            // }

            Eigen::MatrixXd feature(temp.size(), temp[0].size() - 1);

            Eigen::MatrixXd label(1, temp.size());

            for (size_t i = 0; i < temp.size(); ++i)
            {
                label(0, i) = temp[i][0];
                for (size_t j = 0; j < temp[i].size() - 1; ++j)
                {
                    feature(i, j) = temp[i][j + 1];
                }
            }
            // std::cout << "feature size: (" << feature.rows() << ", " << feature.cols() << ")" << std::endl;

            // std::cout << "label size: (" << label.rows() << ", " << label.cols() << ")" << std::endl;

            test_sample.set_feature(feature);
            test_sample.set_label(label);

            return true;
        }
        void show_testData()
        {
            std::cout << "test feature: \n"
                      << test_sample.getFeature() << std::endl;
            std::cout << "test label: \n"
                      << test_sample.getLabel() << std::endl;
        }
        void show_trainData()
        {
            std::cout << "train feature: \n"
                      << train_sample.getFeature() << std::endl;
            std::cout << "train label: \n"
                      << train_sample.getLabel() << std::endl;
        }

        sample get_train_sample()
        {
            return train_sample;
        }
        sample get_test_sample()
        {
            return test_sample;
        }
    };

} // namespace Ach_Net::inline data

#endif