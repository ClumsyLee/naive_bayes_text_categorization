#include <iostream>
#include "naive_bayes.h"
#include "filter.h"

using namespace homework::experiment;  // for convenience

void InteractiveLearn(NaiveBayes &classifier);
void InteractiveClassify(const NaiveBayes &classifier);

int main()
{
    NaiveBayes classifier(new BasicFilter);

    while (true)
    {
        InteractiveLearn(classifier);
        InteractiveClassify(classifier);
    }

    return 0;
}

void InteractiveLearn(NaiveBayes &classifier)
{
    std::vector<NaiveBayes::File> files;

    std::string category_name;
    std::cout << "Please enter the category: ";
    std::getline(std::cin, category_name);

    std::cout << "Please enter file names (Category: " << category_name << ")\n"
                 "Enter an empty line to stop:\n";

    std::string filename;
    while (std::getline(std::cin, filename) && !filename.empty())
    {
        files.push_back({filename, category_name});
    }

    std::cout << classifier.Learn(files) << " file(s) learned\n";
}

void InteractiveClassify(const NaiveBayes &classifier)
{
    std::string filename;
    std::cout << "Please enter file name: ";

    while (std::getline(std::cin, filename) && !filename.empty())
    {
        const std::string *category_name = classifier.Classify(filename, true);
        if (category_name)
        {
            std::cout << "Classify to " << *category_name << std::endl;
        }
        else
        {
            std::cout << "Classification failed\n";
        }
    }
}
