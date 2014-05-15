#include "naive_bayes.h"
#include <cctype>
#include <fstream>
#include <iostream>

namespace homework {
namespace experiment {

Category::Category()
        : article_number_(0),
          word_number_(0),
          basic_probability_(0.0),
          word_map_()
{
}

void Category::CalculateProbability(std::size_t total_article_number,
                                    std::size_t dictionary_size)
{
    double category_probability = static_cast<double>(article_number_) /
                                  static_cast<double>(total_article_number);
    basic_probability_ = 1.0 / (word_number_ + dictionary_size);

    for (auto &word : word_map_)
    {
        word.second.probability = category_probability *
                                  (word.second.count + 1) /
                                  (word_number_ + dictionary_size);
    }
}

double Category::Probability(const std::string &word) const
{
    auto iter = word_map_.find(word);
    if (iter == word_map_.end())
        return basic_probability_;
    else
        return iter->second.probability;
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
        category.second.CalculateProbability(total_article_number_,
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
    double max_probability = 0.0;

    for (const auto &category : categories_)
    {
        double probability = Probability(category.second, words);

        if (verbose)
        {
            std::cout << category.first << ": " << probability << std::endl;
        }

        if (probability > max_probability)
        {
            max_probability = probability;
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
    }

    return true;
}



double NaiveBayes::Probability(const Category &category,
                               const std::vector<std::string> &words) const
{
    double probability = 1.0;

    for (const auto &word : words)
    {
        probability *= category.Probability(word);
    }

    return probability;
}




}  // namespace experiment
}  // namespace homework
