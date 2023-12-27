

// tutorial.cpp

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include "TutorialConfig.hh"

#ifdef USE_MYMATH
    #include "MathFunctions.hh"
#endif


int main(int argc, char* argv[])
{
    // if (argc < 2) {
    if (true) {
        // report version
        std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
                    << Tutorial_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
    }

    // convert input to double
    const double inputValue = atof(argv[1]);
    std::cout << "inputValue = " << inputValue << std::endl;

    // calculate square root
    #ifdef USE_MYMATH
    const double outputValue = mysqrt(inputValue);
    #else
    const double outputValue = sqrt(inputValue);
    #endif
    std::cout << "The square root of " << inputValue
              << " is " << outputValue
              << std::endl;
    return 0;
}