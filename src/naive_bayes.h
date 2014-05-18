#ifndef NAIVE_BAYES_H_
#define NAIVE_BAYES_H_

#include <cstddef>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "filter.h"

namespace homework {
namespace experiment {

class Category
{
 public:
    Category();

    void AddArticle() { article_number_++; }
    void AddWord(const std::string &word)
    {
        word_map_[word].count++;
        word_number_++;
    }
    void CalculateLogProbability(std::size_t total_article_number,
                                 std::size_t dictionary_size);
    double LogProbability(const std::string &word) const;

 private:
    struct WordInfo
    {
        int count = 0;
        double log_probability = 0.0;
    };

    std::size_t article_number_;
    std::size_t word_number_;

    double basic_log_probability_;

    std::unordered_map<std::string, WordInfo> word_map_;
};


class NaiveBayes
{
 public:
    struct File
    {
        std::string filename;
        std::string category;
    };

    explicit NaiveBayes(Filter *filter);
    ~NaiveBayes();

    std::size_t Learn(const std::vector<File> &files);
    const std::string * Classify(const std::string &filename,
                                 bool verbose = false) const;

    NaiveBayes(const NaiveBayes &) = delete;
    NaiveBayes & operator=(const NaiveBayes &) = delete;

 private:
    bool LearnFile(const File &file);
    bool ReadFile(const std::string &filename,
                  std::vector<std::string> &words,
                  bool only_read_known_words = false) const;

    // the words should only contains words that are in dictionary_
    double LogProbability(const Category &category,
                          const std::vector<std::string> &words) const;

    std::map<std::string, Category> categories_;

    // record all the words in the examples.
    std::unordered_set<std::string> dictionary_;

    std::size_t total_article_number_;

    Filter *filter_;
};

}  // namespace experiment
}  // namespace homework

#endif  // NAIVE_BAYES_H_
