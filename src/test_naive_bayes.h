#ifndef TEST_NAIVE_BAYES_H_
#define TEST_NAIVE_BAYES_H_

#include "naive_bayes.h"

#include <cstddef>

#include <map>
#include <string>
#include <vector>

namespace homework {
namespace experiment {

class TestNaiveBayes
{
 public:
    explicit TestNaiveBayes(const std::string &training_dir);

    void CrossValidate() const;

 private:
    typedef std::string Filename;

    void LoadTrainingFileNames();
    void LoadCategory(const std::string &category_name);

    bool Validate(std::size_t file_index) const;

    std::string training_dir_;
    std::map<std::string, std::vector<Filename>> training_files_;
    std::size_t training_file_count_;
};


}  // namespace experiment
}  // namespace homework

#endif  // TEST_NAIVE_BAYES_H_
