#ifndef FILTER_H_
#define FILTER_H_

#include <string>

namespace homework {
namespace experiment {

class Filter
{
 public:
    Filter() = default;
    virtual ~Filter() = default;

    virtual bool IsValidWord(const std::string &word) = 0;
};

class BasicFilter : public Filter
{
 public:
    BasicFilter() = default;
    virtual ~BasicFilter() = default;

    virtual bool IsValidWord(const std::string &word)
    {
        return word.size() >= 3;
    }
};

}  // namespace experiment
}  // namespace homework

#endif  // FILTER_H_
