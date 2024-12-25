#pragma once

#include <cstring>

#ifdef BUILD_CBTEK_UTILITY_DLL
    #ifdef _WIN32
        #define CBTEK_UTILITY_DLL __declspec( dllexport )
    #else
    #   define CBTEK_UTILITY_DLL
    #endif
#else
    #define CBTEK_UTILITY_DLL
#endif


const static int  c_DEFAULT_FLOATING_PRECISION = 12;
const static bool c_DEFAULT_CASE_SENSITIVE = true;
const static int  c_MAX_PATH = 1024;
