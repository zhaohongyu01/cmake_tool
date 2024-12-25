#pragma once

#include "UtilityCommon.hpp"

#include <chrono>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
    #include <shlobj.h>
    #include <lmcons.h>
#else
    #include <dirent.h>
    #include <unistd.h>
    #include <libgen.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <pwd.h>
    #include <dlfcn.h>
#endif


class SystemUtils
{
public:
    /**
     * @brief getUserHomeDirectory Gets user home directory
     * @return Returns string representing user home directory
     */
    static std::string getUserHomeDirectory();

    /**
     * @brief getUserDesktopDirectory
     * @return
     */
    static std::string getUserDesktopDirectory();

    /**
     * @brief getUserAppDirectory Gets current user application directory
     * @return Return string representing the current users application directory
     */
    static std::string getUserAppDirectory();

    /**
     * @brief getUserTempDirectory Gets temporary directory for current user
     * @return Returns string representing the current users temporary directory
     */
    static std::string getUserTempDirectory();

    /**
     * @brief getUserName Gets the login name of the current user
     * @return Returns string representing current users login name
     */
    static std::string getUserName();

    /**
     * @brief getTotalSystemMemory
     * @return
     */
    static size_t getTotalSystemMemory();

    /**
     * @brief getNumCPUThreads
     * @return
     */
    static size_t getNumCPUThreads();

    /**
     * @brief execute Simple command to start an external application
     * @param command Full path to the command to run
     * @return
     */
    static int execute(const std::string &command);

    /**
     * @brief getCurrentDirectory Gets the current directory
     * @return Returns string representing the current directory
     */
    static std::string getCurrentDirectory();

    /**
     * @brief getCurrentExecutableDirectory Gets the executable directory
     * @return Returns string representing directory where executable launched from
     */
    static std::string getCurrentExecutableDirectory();

    static std::string getEnv(const std::string &env);
    static void setEnv(const std::string &env, const std::string &value);
    static void appendEnvValue(const std::string &env, const std::string &value);
};

