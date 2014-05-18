#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

#include "naive_bayes.h"

namespace homework {
namespace experiment {

Category::Category()
        : article_number_(0),
          word_number_(0),
          basic_log_probability_(0.0),
          word_map_()
{
}

void Category::CalculateLogProbability(std::size_t total_article_number,
                                       std::size_t dictionary_size)
{
    double category_probability = static_cast<double>(article_number_) /
                                  static_cast<double>(total_article_number);
    basic_log_probability_ = std::log(1.0 / (word_number_ + dictionary_size));

    for (auto &word : word_map_)
    {
        word.second.log_probability =
                std::log(category_probability *
                         (word.second.count + 1) /
                         (word_number_ + dictionary_size));
    }
}

double Category::LogProbability(const std::string &word) const
{
    auto iter = word_map_.find(word);
    if (iter == word_map_.end())
        return basic_log_probability_;
    else
        return iter->second.log_probability;
}



NaiveBayes::NaiveBayes(Filter *filter)
        : categories_(),
          dictionary_(),
          total_article_number_(0),
          filter_(filter)
{
}

NaiveBayes::~NaiveBayes()
{
    delete filter_;
}

std::size_t NaiveBayes::Learn(const std::vector<File> &files)
{
    std::size_t loaded_file_number = 0;

    for (const File &file : files)
    {
        if (LearnFile(file))
            loaded_file_number++;
    }

    total_article_number_ += loaded_file_number;

    // All the files read, calculate  possibilities.
    for (auto &category : categories_)
    {
        category.second.CalculateLogProbability(total_article_number_,
                                                dictionary_.size());
    }

    return loaded_file_number;
}


const std::string * NaiveBayes::Classify(const std::string &filename,
                                         bool verbose) const
{
    std::vector<std::string> words;
    if (!ReadFile(filename, words, true))
        return nullptr;

    const std::string *best_category = nullptr;
    double max_log_probability = std::numeric_limits<double>::lowest();

    for (const auto &category : categories_)
    {
        double log_probability = LogProbability(category.second, words);

        if (verbose)
        {
            // show category name and probability
            std::cout << category.first << ": " << log_probability << std::endl;
        }

        if (log_probability > max_log_probability)
        {
            max_log_probability = log_probability;
            best_category = &category.first;
        }
    }

    return best_category;
}



bool NaiveBayes::LearnFile(const File &file)
{
    std::vector<std::string> words;

    if (!ReadFile(file.filename, words))
        return false;

    Category &this_category = categories_[file.category];
    this_category.AddArticle();

    for (const std::string &word : words)
    {
        if (filter_->IsValidWord(word))
        {
            dictionary_.insert(word);
            this_category.AddWord(word);
        }
    }

    return true;
}

bool NaiveBayes::ReadFile(const std::string &filename,
                          std::vector<std::string> &words,
                          bool only_read_known_words) const
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        std::cerr << "Cannot open input file: (" << filename << ")\n";
        return false;
    }

    while (fin)
    {
        char ch;
        std::string word;

        while (fin.get(ch))
        {
            if (std::isalpha(ch))  // still in a word
            {
                word.push_back(std::tolower(ch));
            }
            else  // get out of the word
            {
                if (word.empty() ||
                    (only_read_known_words && dictionary_.count(word) == 0))
                    break;
                else
                {
                    words.push_back(word);
                    break;
                }
            }
        }

        if (!fin)  // EOF reached
        {
            if (word.empty() ||
                (only_read_known_words && dictionary_.count(word) == 0))
            {}
            else
                words.push_back(word);
        }
    }

    return true;
}



double NaiveBayes::LogProbability(const Category &category,
                                  const std::vector<std::string> &words) const
{
    double log_probability = 0.0;

    for (const auto &word : words)
    {
        log_probability += category.LogProbability(word);
    }

    return log_probability;
}




}  // namespace experiment
}  // namespace homework
