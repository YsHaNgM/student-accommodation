#include <iostream>
#include <stdexcept>
#include <memory>
#include "file_loader_data_creator.h"
#include "solver.h"

using namespace std;

static void show_help(const string &app_name)
{
    cerr << "Usage: "
         << app_name << " <file>" << endl
         << "\t-h,--help\t\tShow this help message." << endl;
}

int main(int argc, char const *argv[])
{
    try
    {
        if (argc != 2) //app name +file name
        {
            show_help(argv[0]);
            return 1; //exit abnormally
        }
        auto file_loader = make_unique<sas::file_loader_data_creator>(argv[1]);
        auto file_data = file_loader->get_data();

        if (typeid(file_data) != typeid(sas::Incidence))
        {
            throw runtime_error("Incorrect file content.");
        }

        auto solver_ptr = make_unique<sas::solver>(file_data);
        auto student_order = solver_ptr->allocation();

        std::cout << "Student allocation: ";
        for (auto i : student_order)
            std::cout
                << i << ' ';
        std::cout << std::endl;

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
