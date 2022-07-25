#include <utility>
#include "file_loader_data_creator.h"

namespace sas
{
    Incidence file_loader_data_creator::get_data()
    {
        std::ifstream file_reader;
        file_reader.open(file_name); //open file
        if (!file_reader.is_open())
        {
            std::cout << "Can not open the file.\n"; // error message
            exit(EXIT_FAILURE);
        }

        std::string lined_data; //line of data

        while (!file_reader.eof())
        {
            getline(file_reader, lined_data);        //get one line, from file_reader, stored in lined_data
            std::stringstream string_in(lined_data); //constructor of string_in,
                                                     //stream act as buffer to input/output string
            if (string_in.rdbuf()->in_avail() != 0)
            {
                Friendship pair;
                string_in >> pair.first;
                pair.second = std::unordered_set<int>(std::move(std::istream_iterator<int>(string_in)), {});

                v.insert(pair); //to-do: maybe emplace}
            }
        }
        file_reader.close();
        return v;
    }

    file_loader_data_creator::file_loader_data_creator(std::string _file_name) : file_name(std::move(_file_name))
    {
    }
} // namespace sas
