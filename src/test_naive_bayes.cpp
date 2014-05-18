#include "filter.h"
#include "test_naive_bayes.h"

#include <cstddef>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>

namespace homework {
namespace experiment {

TestNaiveBayes::TestNaiveBayes(const std::string &training_dir)
        : training_dir_(training_dir),
          training_files_(),
          training_file_count_(0)
{
    LoadTrainingFileNames();
}

void TestNaiveBayes::CrossValidate() const
{
    std::size_t correct_count = 0;
    for (std::size_t i = 0; i < training_file_count_; i++)
    {
        if (Validate(i))
            correct_count++;
    }

    std::cout << "================ Result ================\n"
                 "Total: " << training_file_count_ << "\n"
                 "Correct: " << correct_count << "\n"
                 "Accuracy: " << static_cast<double>(correct_count) /
                                 training_file_count_ << std::endl;
}

void TestNaiveBayes::LoadTrainingFileNames()
{
    struct dirent* ent = nullptr;
    DIR *p_dir = opendir(training_dir_.c_str());

    if (p_dir == nullptr)
        return;

    while ((ent = readdir(p_dir)) != nullptr)
    {
        if (ent->d_type & DT_DIR)  // category directory
        {
            std::string dir_name(ent->d_name);

            if (dir_name == "." || dir_name == "..")
                continue;

            LoadCategory(dir_name);
        }
    }
}

void TestNaiveBayes::LoadCategory(const std::string &category_name)
{
    std::string category_dir = training_dir_ + '/' + category_name;

    struct dirent* ent = nullptr;
    DIR *p_dir = opendir(category_dir.c_str());

    if (p_dir == nullptr)
        return;

    while ((ent = readdir(p_dir)) != nullptr)
    {
        if (ent->d_type & DT_REG)  // regular file
        {
            training_files_[category_name].push_back(category_dir + '/' +
                                                     ent->d_name);
            training_file_count_++;
        }
    }
}

bool TestNaiveBayes::Validate(std::size_t file_index) const
{
    if (file_index > training_file_count_)  // invalid index
    {
        std::cerr << "Invalid file index (" << file_index << " out of "
                  << training_file_count_ << ")\n";
        return false;
    }

    NaiveBayes classifier(new BasicFilter);
    std::string validater;
    std::string actual_category;

    std::size_t file_count_now = 0;
    for (const auto &category : training_files_)
    {
        for (const Filename &filename : category.second)
        {
            if (file_count_now == file_index)  // file to validate
            {
                validater = filename;
                actual_category = category.first;
            }
            else  // file in train set
            {
                classifier.LearnFile({filename, category.first});
            }
            file_count_now++;
        }
    }

    std::cout << actual_category << ": " << validater << " -> ";

    const std::string *classify_result = classifier.Classify(validater);
    if (classify_result == nullptr)
    {
        std::cout << "Failed\n";
        return false;
    }
    else if (*classify_result == actual_category)
    {
        std::cout << *classify_result << " pass\n";
        return true;
    }
    else
    {
        std::cout << *classify_result << " WRONG\n";
        return false;
    }
}

}  // namespace experiment
}  // namespace homework
