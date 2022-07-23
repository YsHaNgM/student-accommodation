#ifndef basic_types_h
#define basic_types_h

#include <map>
#include <unordered_set>

//! Single namespace for all code in this package
namespace sas
{
    typedef std::pair<int, std::unordered_set<int>> Friendship;
    typedef std::map<int, std::unordered_set<int>> Incidence;
} // namespace sas

#endif
