#include <iostream>
#include <stdexcept>
#include <memory>
#include "file_loader_data_creator.h"

using namespace std;

static void show_help(const string &app_name)
{
    cerr << "Usage: "
         << app_name << " <file> [OPTION]" << endl
         << "\t-h,--help\t\tShow this help message." << endl
         << "Choose one of following solvors:" << endl
         << "\t-n,--normal\t\tNormal equetion solver." << endl
         << "\t-s,--svd\t\tSVD solver." << endl;
}
int main(int argc, char const *argv[])
{
    try
    {
        if (argc != 3) //app name +file name+solver name = 3
        {
            show_help(argv[0]);
            return 1; //exit abnormally
        }
        auto file_loader = make_unique<sas::file_loader_data_creator>(argv[1]);
        auto FileData = file_loader->get_data();
        // for (const auto it : FileData)
        // {
        //     for (const auto i : it.second)
        //     {
        //         cout << i << ' ';
        //     }
        //     cout << endl;
        // }

        // if (typeid(FileData) != typeid(sas::PairedVector))
        // {
        //     throw runtime_error("Incorrect file content.");
        // }
        // else if (typeid(FileData.at(0)) != typeid(sas::PairedSolution))
        // {
        //     throw runtime_error("Incorrect data structure.");
        // }

        // bool normal = false;
        // bool svd = false;
        // for (auto i = 1; i < argc; i++)
        // {
        //     string argName = argv[i];
        //     if (argName == "-h" || argName == "--help")
        //     {
        //         show_help(string(argv[0]));
        //         return 0;
        //     }
        //     else if (argName == "-n" || argName == "--normal")
        //     {
        //         normal = true;
        //     }
        //     else if (argName == "-s" || argName == "--svd")
        //     {
        //         svd = true;
        //     }
        // }

        // if (normal == svd)
        // {
        //     show_help(string(argv[0]));
        //     return 1;
        // }
        // else if (normal)
        // {
        //     auto ptr = make_unique<sas::NormalEquationSolver>();
        //     auto sol = (*ptr).FitData(FileData);
        //     cout << sol.first << " " << sol.second << endl;
        // }
        // else
        // {
        //     auto ptr = make_unique<sas::SVDsolver>();
        //     auto sol = (*ptr).FitData(FileData);
        //     cout << sol.first << " " << sol.second << endl;
        // }
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}