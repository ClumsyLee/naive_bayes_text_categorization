#include <iostream>
#include "test_naive_bayes.h"

using namespace homework::experiment;  // for convenience

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <training dir>\n";
        std::exit(EXIT_FAILURE);
    }

    TestNaiveBayes tester(argv[1]);
    tester.CrossValidate();

    return 0;
}
