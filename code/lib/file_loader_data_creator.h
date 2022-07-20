#ifndef file_loader_data_creator_h
#define file_loader_data_creator_h

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "data_creator.h"

namespace sas
{
    class file_loader_data_creator : public data_creator
    {
    private:
        std::string file_name;

        incidence v; //(X,y) vector pairs

    public:
        explicit file_loader_data_creator(std::string);
        incidence get_data() final;
    };
} // namespace sas

#endif