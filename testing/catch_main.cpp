#define CATCH_CONFIG_RUNNER // This tells Catch we provide main.
#include "catch.hpp"
#include "catch_main.h"

// Global! Only do this in a private test harness.
namespace lia
{
    int argc;
    char **argv;
}

int main(int argc, char *const argv[])
{
    lia::argc = argc;
    lia::argv = const_cast<char **>(argv);

    Catch::Session session; // There must be exactly once instance

    int tmpArgc = 1;
    char *tmpCommandName = new char[256];
    strcpy(tmpCommandName, argv[0]);

    int returnCode = session.applyCommandLine(tmpArgc, const_cast<char **>(&tmpCommandName));
    if (returnCode != 0) // Indicates a command line error
        return returnCode;

    int sessionReturnCode = session.run();

    delete[] tmpCommandName;
    return sessionReturnCode;
}