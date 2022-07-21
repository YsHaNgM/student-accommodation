#ifndef basic_types_h
#define basic_types_h

#include <map>
#include <vector>

//! Single namespace for all code in this package
namespace sas
{
    typedef std::pair<int, std::vector<int>> Friendship;
    typedef std::map<int, std::vector<int>> Incidence;
} // namespace sas

#endif
