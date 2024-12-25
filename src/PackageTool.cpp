#include <iostream>
#include <numeric>
#include <algorithm>

#include "PackageTool.hpp"

#include "utils/Exception.hpp"
#include "utils/StringUtils.h"
#include "utils/FileUtils.h"
#include "utils/SystemUtils.h"

static std::ofstream g_log;
static std::fstream g_create_info;

std::string _executeCmd(const std::string &strCmd)
{
    std::string result;
    FILE *pipe = popen(strCmd.c_str(), "r");

    if (!pipe)
    {
        printf("execute_cmd error: popen(%s) failed", strCmd.c_str());
        return "";
    }

    try
    {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        printf("_executeCmd error: cmd(%s) execute failed", strCmd.c_str());
        return "";
    }

    pclose(pipe);
    return result;
}

PackageTool::PackageTool(bool enable_log)
{
    if (enable_log)
    {
        g_log.open((FileUtils::buildFilePath(SystemUtils::getUserHomeDirectory(), "cmake_tool.log").c_str()), std::ios::app);
    }

    if (!_scanTemplateFiles())
    {
        g_log << (EXCEPTION_TAG + "Could not find location of template files for cmake_tool!") << std::endl;
        throw InvalidOperationException(EXCEPTION_TAG + "Could not find location of template files for cmake_tool!");
    }
}

PackageTool::~PackageTool()
{
    if (g_log.is_open())
    {
        g_log.close();
    }
}

void PackageTool::setLog(bool enable_log)
{
    if (enable_log)
    {
        if (!g_log.is_open())
        {
            g_log.open((FileUtils::buildFilePath(SystemUtils::getUserHomeDirectory(), "cmake_tool.log").c_str()), std::ios::app);
        }
    }
    else
    {
        if (g_log.is_open())
        {
            g_log.close();
        }
    }
}

void PackageTool::setForce(bool enable_force)
{
    m_force = enable_force;
}

void PackageTool::_updateCurrentPackage(const std::string &package_path, const PackageType &package_type)
{
    m_currentPackage.path = FileUtils::getAbsolutePath(package_path);

    if (package_type != PackageType::CMAKE_UNKNOWN)
    {
        m_currentPackage.type = package_type;
    }
}

bool PackageTool::_isValidTemplatePath(const std::string &path)
{
    if (path.empty())
    {
        return false;
    }

    if (!FileUtils::fileExists(FileUtils::buildFilePath(path, "share")))
    {
        return false;
    }

    // CPP Path
    m_cppCMakePath = FileUtils::buildFilePath(path, "share/cmake_tool/c++_application.cmake.in");
    m_cppMainPath = FileUtils::buildFilePath(path, "share/cmake_tool/c++_main.cpp.in");
    // C Path
    m_cCMakePath = FileUtils::buildFilePath(path, "share/cmake_tool/c_application.cmake.in");
    m_cMainPath = FileUtils::buildFilePath(path, "share/cmake_tool/c_main.c.in");

    bool hasTemplateFiles = FileUtils::fileExists(m_cppMainPath) &&
                            FileUtils::fileExists(m_cppCMakePath) &&
                            FileUtils::fileExists(m_cMainPath) &&
                            FileUtils::fileExists(m_cCMakePath);

    g_log << "Template Check at " << path << std::endl;
    g_log << "{" << std::endl;
    g_log << "\tCPP CMakeLists.txt Template Path: " << m_cppCMakePath << std::endl;
    g_log << "\tCPP main.cpp Template Path: " << m_cppMainPath << std::endl;
    g_log << "\tC CMakeLists.txt Template Path: " << m_cCMakePath << std::endl;
    g_log << "\tC main.c Template Path: " << m_cMainPath << std::endl;

    g_log << "\tHAS CPP APPLICATION FILES: " << (hasTemplateFiles ? "TRUE" : "FALSE") << std::endl;
    g_log << "}" << std::endl;

    if (hasTemplateFiles)
    {
        m_createInfoPath = FileUtils::buildFilePath(path, "share/cmake_tool/create.info");
    }

    if (!FileUtils::fileExists(m_createInfoPath))
    {
        pid_t status = system(("touch " + m_createInfoPath).c_str());
        (void)status;
    }

    return hasTemplateFiles;
}

bool PackageTool::_isValidPackageType(const PackageType &type)
{
    if (type == PackageType::CMAKE_CPP_PACKAGE)
        return true;
    if (type == PackageType::CMAKE_C_PACKAGE)
        return true;
    return false;
}

bool PackageTool::_scanTemplateFiles()
{
    if (_isValidTemplatePath(SystemUtils::getEnv("CMAKE_TOOL_DIR")) || _isValidTemplatePath(SystemUtils::getEnv("CMAKE_TOOL_PATH")))
    {
        // printf("_scanTemplateFiles success: getEnv\n");
        return true;
    }
    if (_isValidTemplatePath(SystemUtils::getCurrentExecutableDirectory()))
    {
        // printf("_scanTemplateFiles success: getCurrentExecutableDirectory\n");
        return true;
    }
    if (_isValidTemplatePath(FileUtils::buildFilePath(SystemUtils::getCurrentExecutableDirectory(), "../")))
    {
        // printf("_scanTemplateFiles success: getCurrentExecutableDirectory ..\n");
        return true;
    }
    if (_isValidTemplatePath(SystemUtils::getUserHomeDirectory()))
    {
        // printf("_scanTemplateFiles success: getUserHomeDirectory\n");
        return true;
    }
    if (_isValidTemplatePath(SystemUtils::getUserAppDirectory()))
    {
        // printf("_scanTemplateFiles success: getUserAppDirectory\n");
        return true;
    }
#ifdef __linux__
    if (_isValidTemplatePath("/usr/local/"))
    {
        return true;
    }
    if (_isValidTemplatePath("/usr/"))
    {
        return true;
    }
#endif

    return false;
}

bool PackageTool::_createDirectory()
{
    bool ret = false;

    if (FileUtils::fileExists(m_currentPackage.path))
    {
        std::cout << "Warning: package already exists [" << m_currentPackage.path << "]" << std::endl;
        g_log << "Warning: package already exists [" << m_currentPackage.path << "]" << std::endl;
    }
    else
    {
        if (FileUtils::createDirectory(m_currentPackage.path))
        {
            g_log << "Successfully created output path at \"" << m_currentPackage.path << "\"" << std::endl;
            ret = true;
        }
        else
        {
            std::cerr << "Failed to create package: \"" << m_currentPackage.path << "\"" << std::endl;
            g_log << "Failed to create package: \"" << m_currentPackage.path << "\"" << std::endl;
        }
    }

    if (!m_createInfoPath.empty())
    {
        // 在share/create.info中记录创建package的路径
        g_create_info.open(m_createInfoPath, std::ios::in | std::ios::out);
        std::string line;
        while (std::getline(g_create_info, line))
        {
            if (line.empty())
            {
            }

            StringUtils::trimmedInPlace(line);
            if (line.front() == '~')
            {
                const char *homeDir = getenv("HOME");
                if (homeDir != nullptr)
                {
                    line.replace(0, 1, homeDir);
                }
            }

            if (line == m_currentPackage.path)
            {
                g_create_info.close();
                return ret;
            }
        }
        g_create_info.clear();
        g_create_info.seekp(0, std::ios::end); // 将写指针设置到文件末尾
        g_create_info << m_currentPackage.path << std::endl;
        g_create_info.close();
    }

    return ret;
}

void PackageTool::_createCPPProject()
{
    std::string incPath;
    std::string srcPath;
    std::string mainPath;
    std::string cmakePath;
    g_log << "_createCPPProject" << std::endl
          << "{" << std::endl;
    std::string contents = FileUtils::getFileContents(m_cppCMakePath);
    std::string mainContents = FileUtils::getFileContents(m_cppMainPath);

    if (contents.size() == 0)
    {
        g_log << (EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cppCMakePath + "\"") << std::endl;
        g_log.close();
        throw InvalidOperationException(EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cppCMakePath + "\"");
    }

    if (mainContents.size() == 0)
    {
        g_log << (EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cppMainPath + "\"") << std::endl;
        g_log.close();
        throw InvalidOperationException(EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cppMainPath + "\"");
    }

    StringUtils::replaceInPlace(contents, "%%PROJECT_NAME%%", FileUtils::getFileName(m_currentPackage.path));

    incPath = FileUtils::buildFilePath(m_currentPackage.path, "include/");
    srcPath = FileUtils::buildFilePath(m_currentPackage.path, "src/");
    cmakePath = FileUtils::buildFilePath(m_currentPackage.path, "CMakeLists.txt");
    mainPath = FileUtils::buildFilePath(srcPath, FileUtils::getFileName(m_currentPackage.path) + ".cpp");

    FileUtils::createDirectory(incPath);
    FileUtils::createDirectory(srcPath);
    FileUtils::writeFileContents(cmakePath, contents);
    FileUtils::writeFileContents(mainPath, mainContents);

    g_log << "\tCreating path at \"" << incPath << "\"" << std::endl;
    g_log << "\tCreating path at \"" << srcPath << "\"" << std::endl;
    g_log << "\tCreating path at \"" << cmakePath << "\"" << std::endl;
    g_log << "\tCreating path at \"" << mainPath << "\"" << std::endl
          << "}" << std::endl;
}

void PackageTool::_createCProject()
{
    std::string incPath;
    std::string srcPath;
    std::string mainPath;
    std::string cmakePath;
    g_log << "_createCProject" << std::endl
          << "{" << std::endl;
    std::string contents = FileUtils::getFileContents(m_cCMakePath);
    std::string mainContents = FileUtils::getFileContents(m_cMainPath);

    if (contents.size() == 0)
    {
        g_log << (EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cCMakePath + "\"") << std::endl;
        g_log.close();
        throw InvalidOperationException(EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cCMakePath + "\"");
    }

    if (mainContents.size() == 0)
    {
        g_log << (EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cMainPath + "\"") << std::endl;
        g_log.close();
        throw InvalidOperationException(EXCEPTION_TAG + "Failed to get any contents from file at \"" + m_cMainPath + "\"");
    }

    StringUtils::replaceInPlace(contents, "%%PROJECT_NAME%%", FileUtils::getFileName(m_currentPackage.path));

    incPath = FileUtils::buildFilePath(m_currentPackage.path, "include/");
    srcPath = FileUtils::buildFilePath(m_currentPackage.path, "src/");
    cmakePath = FileUtils::buildFilePath(m_currentPackage.path, "CMakeLists.txt");
    mainPath = FileUtils::buildFilePath(srcPath, FileUtils::getFileName(m_currentPackage.path) + ".c");

    FileUtils::createDirectory(incPath);
    FileUtils::createDirectory(srcPath);
    FileUtils::writeFileContents(cmakePath, contents);
    FileUtils::writeFileContents(mainPath, mainContents);

    g_log << "\tCreating path at \"" << incPath << "\"" << std::endl;
    g_log << "\tCreating path at \"" << srcPath << "\"" << std::endl;
    g_log << "\tCreating path at \"" << cmakePath << "\"" << std::endl;
    g_log << "\tCreating path at \"" << mainPath << "\"" << std::endl
          << "}" << std::endl;
}

void PackageTool::_createProject()
{
    switch (m_currentPackage.type)
    {
    case PackageType::CMAKE_CPP_PACKAGE:
        _createCPPProject();
        break;

    case PackageType::CMAKE_C_PACKAGE:
        _createCProject();
        break;

    default:
        throw InvalidOperationException("Invalid type was detected for cmake_tool.");
        break;
    }
}

bool PackageTool::_deleteDirectory()
{
    if (!FileUtils::fileExists(m_currentPackage.path))
    {
        std::cout << "Warning: package not exists [" << m_currentPackage.path << "]" << std::endl;
        g_log << "Warning: package not exists [" << m_currentPackage.path << "]" << std::endl;
        return false;
    }

    if (FileUtils::deleteFolder(m_currentPackage.path) != 0)
    {
        std::cerr << "!! delete faild: \"" << m_currentPackage.path << "\"" << std::endl;
        g_log << "!! delete faild: \"" << m_currentPackage.path << "\"" << std::endl;
        return false;
    }

    return true;
}

bool PackageTool::_cleanInstallFiles()
{
    std::string cache_path = FileUtils::buildFilePath(m_currentPackage.path, "build/");
    if (FileUtils::fileExists(cache_path))
    {
        std::string cmd = ("cd " + cache_path + " && make clean 2> /dev/null || true && (cat install_manifest.txt; echo) | sh -c 'while read line; do echo \"    rm -f $line\"; rm -f \"$line\"; rmdir --ignore-fail-on-non-empty -p \"${line%/*}\" 2> /dev/null || true; done' && cd - > /dev/null 2>&1;") + ("echo \"    rm -rf \"" + cache_path + " && rm -rf " + cache_path);
        pid_t status = system(cmd.c_str());
        if (0 != WEXITSTATUS(status))
        {
            return false;
        }
    }

    return true;
}

void PackageTool::_getAllPackagePaths(std::vector<std::string> &output_paths)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    _resetInfo();

    std::string line;
    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        output_paths.emplace_back(line);
    }
    g_create_info.close();
}

void PackageTool::_createPackage(const std::string &package_path, PackageType package_type, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    _updateCurrentPackage(package_path, package_type);

    if (_createDirectory())
    {
        _createProject();
        if (!quiet)
        {
            std::cout << "<< create success: \"" << m_currentPackage.path << "\"" << std::endl;
        }
        g_log << "<< create success: \"" << m_currentPackage.path << "\"" << std::endl;
    }
}

void PackageTool::createPackage(const std::string &package_path, const std::string &package_type, bool quiet)
{
    g_log << "-->> run cmake_tool create: " << package_path << std::endl;

    PackageType type{PackageType::CMAKE_UNKNOWN};
    if (package_type.empty() || StringUtils::equals(package_type, "CPP"))
    {
        type = PackageType::CMAKE_CPP_PACKAGE;
    }
    else if (StringUtils::equals(package_type, "C"))
    {
        type = PackageType::CMAKE_C_PACKAGE;
    }

    if (!_isValidPackageType(type))
    {
        std::cerr << "Error: \"" << package_type << "\" is a invalid package type, please check." << std::endl;
        g_log << "Error: \"" << package_type << "\" is a invalid package type, please check." << std::endl;
        return;
    }

    _createPackage(package_path, type, quiet);

    g_log << "<<-- end cmake_tool create: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_listPackages(bool basename_only, bool path_only)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    int max_first_column_width = 0;
    std::vector<std::string> vecPath;

    std::string line;
    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (!line.empty())
        {
            StringUtils::trimmedInPlace(line);
            if (line.front() == '~')
            {
                const char *homeDir = getenv("HOME");
                if (homeDir != nullptr)
                {
                    line.replace(0, 1, homeDir);
                }
            }

            int first_column_width = FileUtils::getFileName(line).length() + 1;
            if (first_column_width > max_first_column_width)
            {
                max_first_column_width = first_column_width;
            }

            vecPath.emplace_back(line);
        }
    }
    g_create_info.close();


    if (basename_only)
    {
        for (const std::string &path: vecPath)
        {
            std::cout << FileUtils::getFileName(path) << std::endl;
        }
    }
    else if (path_only)
    {
        for (const std::string &path: vecPath)
        {
            std::cout << path << std::endl;
        }
    }
    else
    {
        for (const std::string &path: vecPath)
        {
            printf("%-*s %s\n", max_first_column_width, (FileUtils::getFileName(path) + ":").c_str(), path.c_str());
        }
    }
}

void PackageTool::listPackages(bool basename_only, bool path_only)
{
    g_log << "-->> run cmake_tool list" << std::endl;
    _listPackages(basename_only, path_only);
    g_log << "<<-- end cmake_tool list" << std::endl;
}

void PackageTool::_buildPackage(const std::string &package_path, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    _updateCurrentPackage(package_path);

    std::string line;
    bool package_find = false;
    std::vector<std::string> match_basename_paths;

    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        if (line == m_currentPackage.path)
        {
            // 全路径匹配，直接构建
            package_find = true;
            if (FileUtils::fileExists(line))
            {
                if (!quiet)
                {
                    std::cout << std::endl
                              << ">> build start: \"" << m_currentPackage.path << "\"" << std::endl;
                }
                std::string cache_path = FileUtils::buildFilePath(line, "build/");
                std::string cmd = ("mkdir " + cache_path + " > /dev/null 2>&1;") + ("cd " + cache_path + " && cmake .. && make && make install && cd - > /dev/null 2>&1");
                pid_t status = system(cmd.c_str());
                if (0 != WEXITSTATUS(status))
                {
                    std::cerr << "!! build failed: \"" << m_currentPackage.path << "\"" << std::endl;
                    g_log << "!! build failed: \"" << m_currentPackage.path << "\"" << std::endl;
                }
                else
                {
                    if (!quiet)
                    {
                        std::cout << "<< build success: \"" << m_currentPackage.path << "\"" << std::endl;
                    }
                    g_log << "<< build success: \"" << m_currentPackage.path << "\"" << std::endl;
                }
                g_create_info.close();
                return;
            }
        }
        else if (FileUtils::getFileName(line) == package_path)
        {
            // 路径名不匹配，但包名匹配
            package_find = true;
            match_basename_paths.emplace_back(line);
        }
        else
        {
            // 不匹配
        }
    }
    g_create_info.close();

    if (!package_find)
    {
        std::cout << "Warning: not find package [" << package_path << "]" << std::endl;
        g_log << "Warning: not find package [" << package_path << "]" << std::endl;
    }

    size_t match_basename_count = match_basename_paths.size();
    if (match_basename_count == 1)
    {
        _buildPackage(match_basename_paths[0], quiet);
    }
    else if (match_basename_count > 1)
    {
        if (m_force)
        {
            for (const auto &path : match_basename_paths)
            {
                _buildPackage(path);
            }
        }
        else
        {
            std::cout << "find " << match_basename_count << " packages named \"" << package_path << "\":" << std::endl;
            for (const auto &path : match_basename_paths)
            {
                std::cout << "    " << path << std::endl;
            }
            std::cout << "please execute build with full package path, or add option \"-f\" to build all." << std::endl;
        }
    }
}

void PackageTool::buildPackage(const std::string &package_path, bool quiet)
{
    g_log << "-->> run cmake_tool build: " << package_path << std::endl;
    _buildPackage(package_path, quiet);
    g_log << "<<-- end cmake_tool build: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_buildAllPackages(bool quiet)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    std::vector<std::string> all_package_paths;
    _getAllPackagePaths(all_package_paths);

    std::cout << "There are " << all_package_paths.size() << " packages: " << std::endl;
    for (const std::string &package_path : all_package_paths)
    {
        std::cout << "    " << package_path << std::endl;
    }
    if (all_package_paths.size() == 0)
    {
        return;
    }

    std::cout << "Build and install above packages? [y/n] ";
    std::string input;
    while (true)
    {
        getline(std::cin, input);
        if (input == "yes" || input == "y")
        {
            break;
        }
        else if (input == "no" || input == "n")
        {
            return;
        }
        else
        {
            std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
        }
    }

    for (const std::string &package_path : all_package_paths)
    {
        _buildPackage(package_path, quiet);
    }
}

void PackageTool::buildAllPackages(bool quiet)
{
    g_log << "-->> run cmake_tool build all packages" << std::endl;
    _buildAllPackages(quiet);
    g_log << "<<-- end cmake_tool build all packages" << std::endl
          << std::endl;
}

void PackageTool::_cleanPackage(const std::string &package_path, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    _updateCurrentPackage(package_path);

    std::string line;
    bool package_find = false;
    std::vector<std::string> match_basename_paths;

    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        if (line == m_currentPackage.path)
        {
            // 全路径匹配，直接清理
            package_find = true;
            if (FileUtils::fileExists(m_currentPackage.path))
            {
                std::string cache_path = FileUtils::buildFilePath(m_currentPackage.path, "build/");
                if (!quiet)
                {
                    std::cout << std::endl
                              << ">> clean start: \"" << cache_path << "\"" << std::endl;
                }
                if (_cleanInstallFiles())
                {
                    if (!quiet)
                    {
                        std::cout << "<< clean success: \"" << cache_path << "\"" << std::endl;
                    }
                    g_log << "<< clean success: \"" << cache_path << "\"" << std::endl;
                }
                else
                {
                    std::cerr << "!! clean failed: \"" << cache_path << "\"" << std::endl;
                    g_log << "!! clean failed: \"" << cache_path << "\"" << std::endl;
                }
                g_create_info.close();
                return;
            }
        }
        else if (FileUtils::getFileName(line) == package_path)
        {
            // 路径名不匹配，但包名匹配
            package_find = true;
            match_basename_paths.emplace_back(line);
        }
        else
        {
            // 不匹配
        }
    }
    g_create_info.close();

    if (!package_find)
    {
        std::cout << "Warning: not find package [" << package_path << "]" << std::endl;
        g_log << "Warning: not find package [" << package_path << "]" << std::endl;
    }

    size_t match_basename_count = match_basename_paths.size();
    if (match_basename_count == 1)
    {
        _cleanPackage(match_basename_paths[0], quiet);
    }
    else if (match_basename_count > 1)
    {
        if (m_force)
        {
            for (const auto &path : match_basename_paths)
            {
                _cleanPackage(path);
            }
        }
        else
        {
            std::cout << "find " << match_basename_count << " packages named \"" << package_path << "\"" << std::endl;
            for (const auto &path : match_basename_paths)
            {
                std::cout << "    " << path << std::endl;
            }
            std::cout << "please execute clean with full package path, or add option \"-f\" to clean all." << std::endl;
        }
    }
}

void PackageTool::cleanPackage(const std::string &package_path, bool quiet)
{
    g_log << "-->> run cmake_tool clean: " << package_path << std::endl;
    _cleanPackage(package_path, quiet);
    g_log << "<<-- end cmake_tool clean: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_cleanAllPackages(bool quiet)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    std::vector<std::string> all_package_paths;
    _getAllPackagePaths(all_package_paths);

    std::cout << "There are " << all_package_paths.size() << " packages: " << std::endl;
    for (const std::string &package_path : all_package_paths)
    {
        std::cout << "    " << package_path << std::endl;
    }
    if (all_package_paths.size() == 0)
    {
        return;
    }

    std::cout << "Clean install files for above packages? [y/n] ";
    std::string input;
    while (true)
    {
        getline(std::cin, input);
        if (input == "yes" || input == "y")
        {
            break;
        }
        else if (input == "no" || input == "n")
        {
            return;
        }
        else
        {
            std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
        }
    }

    for (const std::string &package_path : all_package_paths)
    {
        _cleanPackage(package_path, quiet);
    }
}

void PackageTool::cleanAllPackages(bool quiet)
{
    g_log << "-->> run cmake_tool clean all packages" << std::endl;
    _cleanAllPackages(quiet);
    g_log << "<<-- end cmake_tool clean all packages" << std::endl
          << std::endl;
}

void PackageTool::_deletePackage(const std::string &package_path, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    _updateCurrentPackage(package_path);

    std::string line;
    bool package_find = false;
    bool full_package_find = false;
    std::vector<std::string> match_basename_paths;

    std::string create_info_path_new = m_createInfoPath + ".new";
    std::ofstream create_info_new(create_info_path_new);

    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        if (line == m_currentPackage.path)
        {
            // 全路径匹配，直接删除
            bool exec_delect = false;
            package_find = true;
            full_package_find = true;
            if (!quiet)
            {
                std::cout << std::endl
                          << ">> delete start: \"" << package_path << "\"" << std::endl;
            }
            std::cout << "Package paths:" << std::endl
                      << "    " << m_currentPackage.path << std::endl
                      << "Delete the above packages and clean install files? [y/n] ";
            std::string input;
            while (true)
            {
                getline(std::cin, input);
                if (input == "yes" || input == "y")
                {
                    exec_delect = true;
                    break;
                }
                else if (input == "no" || input == "n")
                {
                    exec_delect = false;
                    break;
                }
                else
                {
                    std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
                }
            }
            if (exec_delect)
            {
                _cleanInstallFiles();
                if (_deleteDirectory())
                {
                    if (!quiet)
                    {
                        std::cout << "<< delete success: \"" << m_currentPackage.path << "\"" << std::endl;
                    }
                    g_log << "<< delete success: \"" << m_currentPackage.path << "\"" << std::endl;
                }
            }
            else
            {
                create_info_new << line << std::endl;
            }
        }
        else if (FileUtils::getFileName(line) == package_path)
        {
            // 路径名不匹配，但包名匹配
            package_find = true;
            match_basename_paths.emplace_back(line);
            create_info_new << line << std::endl;
        }
        else
        {
            // 不匹配
            create_info_new << line << std::endl;
        }
    }
    g_create_info.close();
    create_info_new.close();

    FileUtils::deleteFile(m_createInfoPath);
    FileUtils::renameFile(create_info_path_new, m_createInfoPath);

    if (!package_find)
    {
        std::cout << "Warning: not find package [" << package_path << "]"
                  << " in cmake_tool" << std::endl;
        g_log << "Warning: not find package [" << package_path << "]"
              << " in cmake_tool" << std::endl;
    }

    size_t match_basename_count = match_basename_paths.size();
    if (match_basename_count == 1 && !full_package_find)
    {
        _deletePackage(match_basename_paths[0], quiet);
    }
    else if (match_basename_count > 1)
    {
        if (m_force)
        {
            for (const auto &path : match_basename_paths)
            {
                _deletePackage(path);
            }
        }
        else
        {
            std::cout << "find " << match_basename_count << " packages named \"" << package_path << "\"" << std::endl;
            for (const auto &path : match_basename_paths)
            {
                std::cout << "    " << path << std::endl;
            }
            std::cout << "please execute delete with full package path, or add option \"-f\" to delete all." << std::endl;
        }
    }
}

void PackageTool::deletePackage(const std::string &package_path, bool quiet)
{
    g_log << "-->> run cmake_tool delete: " << package_path << std::endl;
    _deletePackage(package_path, quiet);
    g_log << "<<-- end cmake_tool delete: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_deleteAllPackages(bool quiet)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    std::vector<std::string> all_package_paths;
    _getAllPackagePaths(all_package_paths);

    std::cout << "There are " << all_package_paths.size() << " packages: " << std::endl;
    for (const std::string &package_path : all_package_paths)
    {
        std::cout << "    " << package_path << std::endl;
    }
    if (all_package_paths.size() == 0)
    {
        return;
    }

    std::cout << "Delete all files for above packages (including src and install files)? [y/n] ";
    std::string input;
    while (true)
    {
        getline(std::cin, input);
        if (input == "yes" || input == "y")
        {
            break;
        }
        else if (input == "no" || input == "n")
        {
            return;
        }
        else
        {
            std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
        }
    }

    for (const std::string &package_path : all_package_paths)
    {
        _updateCurrentPackage(package_path);
        _cleanInstallFiles();
        if (_deleteDirectory())
        {
            if (!quiet)
            {
                std::cout << "<< delete success: \"" << m_currentPackage.path << "\"" << std::endl;
            }
            g_log << "<< delete success: \"" << m_currentPackage.path << "\"" << std::endl;
        }
    }

    FileUtils::deleteFile(m_createInfoPath);
}

void PackageTool::deleteAllPackages(bool quiet)
{
    g_log << "-->> run cmake_tool delete all packages" << std::endl;
    _deleteAllPackages(quiet);
    g_log << "<<-- end cmake_tool delete all packages" << std::endl
          << std::endl;
}

void PackageTool::_resetInfo()
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    std::string line;
    std::string create_info_path_new = m_createInfoPath + ".new";
    std::ofstream create_info_new(create_info_path_new);

    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        if (!FileUtils::fileExists(line))
        {
            continue;
        }

        create_info_new << line << std::endl;
    }
    g_create_info.close();
    create_info_new.close();

    FileUtils::deleteFile(m_createInfoPath);
    FileUtils::renameFile(create_info_path_new, m_createInfoPath);
}

void PackageTool::resetInfo()
{
    g_log << "-->> run cmake_tool reset" << std::endl;
    _resetInfo();
    g_log << "<<-- end cmake_tool reset" << std::endl
          << std::endl;
}

void PackageTool::_runPackage(const std::string &package_path, const std::string &program_name, const std::vector<std::string> &program_args)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        g_log << (EXCEPTION_TAG + "No package_path was specified for running!") << std::endl;
        throw InvalidOperationException(EXCEPTION_TAG + "No package_path was specified for running!");
    }

    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        return;
    }

    _updateCurrentPackage(package_path);

    std::string line;
    bool package_find = false;
    std::vector<std::string> match_basename_paths;

    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        if (line == m_currentPackage.path)
        {
            // 全路径匹配，直接运行程序
            package_find = true;
            if (FileUtils::fileExists(m_currentPackage.path))
            {
                std::string program_path = FileUtils::buildFilePath(m_currentPackage.path, "bin/" + program_name);
                // SystemUtils::appendEnvValue("PATH", FileUtils::buildFilePath(m_currentPackage.path, "bin/"));
                for (const std::string &program_arg : program_args)
                {
                    program_path = program_path + " " + program_arg;
                }
                // printf("%s\n", program_path.c_str());
                pid_t status = system(program_path.c_str());
                if (0 != WEXITSTATUS(status))
                {
                }
                return;
            }
        }
        else if (FileUtils::getFileName(line) == package_path)
        {
            // 路径名不匹配，但包名匹配
            package_find = true;
            match_basename_paths.emplace_back(line);
        }
        else
        {
            // 不匹配
        }
    }
    g_create_info.close();

    if (!package_find)
    {
        std::cout << "Warning: not find package [" << program_name << "]" << std::endl;
        g_log << "Warning: not find package [" << program_name << "]" << std::endl;
    }

    size_t match_basename_count = match_basename_paths.size();
    if (match_basename_count == 1)
    {
        std::string program_path = FileUtils::buildFilePath(match_basename_paths[0], "bin/" + program_name);
        // SystemUtils::appendEnvValue("PATH", FileUtils::buildFilePath(match_basename_paths[0], "bin/"));
        for (const std::string &program_arg : program_args)
        {
            program_path = program_path + " " + program_arg;
        }
        // printf("%s\n", program_path.c_str());
        pid_t status = system(program_path.c_str());
        if (0 != WEXITSTATUS(status))
        {
        }
    }
    else if (match_basename_count > 1)
    {
        std::cout << "find " << match_basename_count << " packages named \"" << program_name << "\"" << std::endl;
        for (const auto &path : match_basename_paths)
        {
            std::cout << "    " << path << std::endl;
        }
        std::cout << "please execute run with full package path." << std::endl;
    }
}

void PackageTool::runPackage(const std::string &package_path, const std::string &program_name, const std::vector<std::string> &program_args)
{
    g_log << "-->> run cmake_tool run: " << package_path << std::endl;
    _runPackage(package_path, program_name, program_args);
    g_log << "<<-- end cmake_tool run: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_attachPackage(const std::string &package_path, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    _updateCurrentPackage(package_path);

    if (FileUtils::fileExists(m_currentPackage.path))
    {
        std::string line;
        g_create_info.open(m_createInfoPath, std::ios::in | std::ios::out);
        while (std::getline(g_create_info, line))
        {
            StringUtils::trimmedInPlace(line);
            if (line.front() == '~')
            {
                const char *homeDir = getenv("HOME");
                if (homeDir != nullptr)
                {
                    line.replace(0, 1, homeDir);
                }
            }

            if (line == m_currentPackage.path)
            {
                g_create_info.close();
                if (!quiet)
                {
                    std::cout << "<< attach success: \"" << m_currentPackage.path << "\" already exist." << std::endl;
                }
                g_log << "<< attach success: \"" << m_currentPackage.path << "\" already exist." << std::endl;
                return;
            }
        }
        // g_create_info.open(m_createInfoPath, std::ios::out | std::ios::app);
        g_create_info.clear();
        g_create_info.seekp(0, std::ios::end); // 将写指针设置到文件末尾
        g_create_info << m_currentPackage.path << std::endl;
        g_create_info.close();
        if (!quiet)
        {
            std::cout << "<< attach success: \"" << m_currentPackage.path << "\"" << std::endl;
        }
        g_log << "<< attach success: \"" << m_currentPackage.path << "\"" << std::endl;
    }
    else
    {
        std::cerr << "!! attach failed: \"" << m_currentPackage.path << "\" not exist in this PC." << std::endl;
        g_log << "!! attach failed: \"" << m_currentPackage.path << "\" not exist in this PC." << std::endl;
    }
}

void PackageTool::attachPackage(const std::string &package_path, bool quiet)
{
    g_log << "-->> run cmake_tool attach: " << package_path << std::endl;
    _attachPackage(package_path, quiet);
    g_log << "<<-- end cmake_tool attach: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_attachPackagesFromFile(const std::string &file_path, bool quiet)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    std::vector<std::string> file_package_paths;
    std::string line;
    g_create_info.open(file_path, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }
        
        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        file_package_paths.emplace_back(line);
    }
    g_create_info.close();

    for (const std::string &package_path : file_package_paths)
    {
        _attachPackage(package_path, quiet);
    }
}

void PackageTool::attachPackagesFromFile(const std::string &file_path, bool quiet)
{
    g_log << "-->> run cmake_tool attach from file: " << file_path << std::endl;
    _attachPackagesFromFile(file_path, quiet);
    g_log << "<<-- end cmake_tool attach from file: " << file_path << std::endl
          << std::endl;
}

void PackageTool::_detachPackage(const std::string &package_path, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    _updateCurrentPackage(package_path);

    std::string line;
    std::string create_info_path_new = m_createInfoPath + ".new";
    std::ofstream create_info_new(create_info_path_new);

    bool package_find = false;
    bool full_package_find = false;
    std::vector<std::string> match_basename_paths;
    g_create_info.open(m_createInfoPath, std::ios::in);
    while (std::getline(g_create_info, line))
    {
        if (line.empty())
        {
            continue;
        }

        StringUtils::trimmedInPlace(line);
        if (line.front() == '~')
        {
            const char *homeDir = getenv("HOME");
            if (homeDir != nullptr)
            {
                line.replace(0, 1, homeDir);
            }
        }

        if (line == m_currentPackage.path)
        {
            // 全路径匹配
            bool exec_detach = false;
            package_find = true;
            full_package_find = true;
            if (FileUtils::fileExists(m_currentPackage.path))
            {
                std::cout << "\"" << m_currentPackage.path << "\""
                          << " exist in this PC." << std::endl
                          << "Are you sure detach the package from cmake_tool? [y/n] " << std::endl;
                while (true)
                {
                    std::string input;
                    getline(std::cin, input);
                    if (input == "yes" || input == "y")
                    {
                        exec_detach = true;
                        break;
                    }
                    else if (input == "no" || input == "n")
                    {
                        exec_detach = false;
                        break;
                    }
                    else
                    {
                        std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
                    }
                }
            }

            if (exec_detach)
            {
                if (!quiet)
                {
                    std::cout << "<< detach success: \"" << m_currentPackage.path << "\"" << std::endl;
                }
                g_log << "<< detach success: \"" << m_currentPackage.path << "\"" << std::endl;
                continue;
            }
        }
        else if (FileUtils::getFileName(line) == package_path)
        {
            // 路径名不匹配，但包名匹配
            package_find = true;
            match_basename_paths.emplace_back(line);
        }
        else
        {
            // 不匹配
        }
        create_info_new << line << std::endl;
    }
    g_create_info.close();
    create_info_new.close();

    FileUtils::deleteFile(m_createInfoPath);
    FileUtils::renameFile(create_info_path_new, m_createInfoPath);

    if (!package_find)
    {
        std::cout << "Warning: not find package [" << package_path << "]"
                  << " in cmake_tool" << std::endl;
        g_log << "Warning: not find package [" << package_path << "]"
              << " in cmake_tool" << std::endl;
    }

    size_t match_basename_count = match_basename_paths.size();
    if (match_basename_count == 1 && !full_package_find)
    {
        _detachPackage(match_basename_paths[0], quiet);
    }
    else if (match_basename_count > 1)
    {
        if (m_force)
        {
            for (const auto &path : match_basename_paths)
            {
                _detachPackage(path);
            }
        }
        else
        {
            std::cout << "find " << match_basename_count << " packages named \"" << package_path << "\"" << std::endl;
            for (const auto &path : match_basename_paths)
            {
                std::cout << "    " << path << std::endl;
            }
            std::cout << "please execute detach with full package path, or add option \"-f\" to detach all." << std::endl;
        }
    }
}

void PackageTool::detachPackage(const std::string &package_path, bool quiet)
{
    g_log << "-->> run cmake_tool detach: " << package_path << std::endl;
    _detachPackage(package_path, quiet);
    g_log << "<<-- end cmake_tool detach: " << package_path << std::endl
          << std::endl;
}

void PackageTool::_detachAllPackages(bool quiet)
{
    if (m_createInfoPath.empty())
    {
        std::cerr << "Error: share path not find!" << std::endl;
        g_log << "Error: share path not find!" << std::endl;
        return;
    }

    std::vector<std::string> all_package_paths;
    _getAllPackagePaths(all_package_paths);

    std::cout << "There are " << all_package_paths.size() << " packages: " << std::endl;
    for (const std::string &package_path : all_package_paths)
    {
        std::cout << "    " << package_path << std::endl;
    }
    if (all_package_paths.size() == 0)
    {
        return;
    }

    std::cout << "Detach above packages from cmake_tool? [y/n] ";
    std::string input;
    while (true)
    {
        getline(std::cin, input);
        if (input == "yes" || input == "y")
        {
            break;
        }
        else if (input == "no" || input == "n")
        {
            return;
        }
        else
        {
            std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
        }
    }

    FileUtils::renameFile(m_createInfoPath, m_createInfoPath + ".bak");
    if (!quiet)
    {
        std::cout << "<< detach above packages and backup success" << std::endl;
    }
    g_log << "<< detach above packages and backup success" << std::endl;
}

void PackageTool::detachAllPackages(bool quiet)
{
    g_log << "-->> run cmake_tool detach all packages" << std::endl;
    _detachAllPackages(quiet);
    g_log << "<<-- end cmake_tool detach all packages" << std::endl
          << std::endl;
}

void PackageTool::_tarPackages(const std::vector<std::string> &package_paths, const std::string &output_path, bool quiet)
{
    std::string tar_output_path = output_path;
    if (tar_output_path.rfind(".tar.") == tar_output_path.npos)
    {
        // 字符串中没找到".tar."，判定output_path不包含不含压缩包的名字，为输出目录
        if (tar_output_path.back() != '/')
        {
            tar_output_path += "/";
        }
        // 如果只有一个包，则将包名作为压缩包的名字
        if (package_paths.size() == 1 && (package_paths[0].rfind("*") == package_paths[0].npos))
        {
            tar_output_path += FileUtils::getFileName(FileUtils::getAbsolutePath(package_paths[0])) + ".tar.gz";
        }
        // 否则，取默认名字
        else
        {
            tar_output_path += "cmake_tool_packages.tar.gz";
        }
    }
    else
    {
        // output_path为输出压缩包的全路径
        if (tar_output_path.find("/") == tar_output_path.npos)
        {
            tar_output_path = "./" + tar_output_path;
        }
    }

    if (!FileUtils::fileExists(FileUtils::getDirPath(tar_output_path)))
    {
        std::cerr << "!! tar failed: can not output to \"" << tar_output_path << "\", because " << FileUtils::getDirPath(tar_output_path) << " not exist." << std::endl;
        g_log << "!! tar failed: can not output to \"" << tar_output_path << "\", because " << FileUtils::getDirPath(tar_output_path) << " not exist." << std::endl;
        return;
    }

    std::string tar_cmd_paths = " --exclude=*/build --exclude=*/include --exclude=*/src --exclude=*/CMakeLists.txt ";
    for (std::string package_path : package_paths)
    {
        size_t pos = package_path.rfind("*");
        if (pos != package_path.npos)
        {
            pos = package_path.rfind("/", pos);
            if (pos != package_path.npos)
            {
                tar_cmd_paths = tar_cmd_paths + "-C " + package_path.substr(0, pos + 1) + " \"" + package_path.substr(pos + 1) + "\" ";
            }
            else
            {
                tar_cmd_paths = tar_cmd_paths + package_path + " ";
            }
        }
        else
        {
            std::string package_full_path = FileUtils::getAbsolutePath(package_path);
            if (!FileUtils::fileExists(package_full_path))
            {
                std::cerr << "!! tar failed: \"" << package_full_path << "\" not exist in this PC." << std::endl;
                g_log << "!! tar failed: \"" << package_full_path << "\" not exist in this PC." << std::endl;
                return;
            }
            // package_full_path一定不会以'/'字符结尾
            // if (package_full_path.back() == '/')
            // {
            //     pos = package_full_path.rfind("/", package_full_path.size() - 2);
            // }
            // else
            // {
            pos = package_full_path.rfind("/");
            // }
            if (pos != package_full_path.npos)
            {
                tar_cmd_paths = tar_cmd_paths + "-C " + package_full_path.substr(0, pos + 1) + " \"" + package_full_path.substr(pos + 1) + "\" ";
            }
            else
            {
                tar_cmd_paths = tar_cmd_paths + package_full_path + " ";
            }
        }
    }

    std::string cmd = "tar -acf " + tar_output_path + " " + tar_cmd_paths;
    // std::cout << cmd << std::endl;
    pid_t status = system(cmd.c_str());
    if (0 != WEXITSTATUS(status))
    {
        std::cerr << "!! tar failed: \"" << cmd << "\"" << std::endl;
        g_log << "!! tar failed: \"" << cmd << "\"" << std::endl;
    }
    else
    {
        if (!quiet)
        {
            std::cout << "<< tar success: \"" << tar_output_path << "\"" << std::endl;
        }
        g_log << "<< tar success: \"" << tar_output_path << "\"" << std::endl;
    }
}

void PackageTool::tarPackages(const std::vector<std::string> &package_paths, const std::string &output_path, bool quiet)
{
    g_log << "-->> run cmake_tool tar" << std::endl;
    _tarPackages(package_paths, output_path, quiet);
    g_log << "<<-- end cmake_tool tar" << std::endl
          << std::endl;
}

void PackageTool::_tarAllPackages(const std::string &output_path, bool quiet)
{
    std::vector<std::string> all_package_paths;
    _getAllPackagePaths(all_package_paths);

    std::string tar_output_path = output_path;
    if (tar_output_path.rfind(".tar.") == tar_output_path.npos)
    {
        // 字符串中没找到".tar."，判定output_path不包含不含压缩包的名字，为输出目录
        if (tar_output_path.back() != '/')
        {
            tar_output_path += "/";
        }
        // 如果只有一个包，则将包名作为压缩包的名字
        if (all_package_paths.size() == 1 && (all_package_paths[0].rfind("*") == all_package_paths[0].npos))
        {
            tar_output_path += FileUtils::getFileName(FileUtils::getAbsolutePath(all_package_paths[0])) + ".tar.gz";
        }
        // 否则，取默认名字
        else
        {
            tar_output_path += "cmake_tool_packages.tar.gz";
        }
    }
    else
    {
        // output_path为输出压缩包的全路径
        if (tar_output_path.find("/") == tar_output_path.npos)
        {
            tar_output_path = "./" + tar_output_path;
        }
    }

    if (!FileUtils::fileExists(FileUtils::getDirPath(tar_output_path)))
    {
        std::cerr << "!! tar failed: can not output to \"" << tar_output_path << "\", because " << FileUtils::getDirPath(tar_output_path) << " not exist." << std::endl;
        g_log << "!! tar failed: can not output to \"" << tar_output_path << "\", because " << FileUtils::getDirPath(tar_output_path) << " not exist." << std::endl;
        return;
    }

    std::cout << "There are " << all_package_paths.size() << " packages: " << std::endl;
    for (const std::string &package_path : all_package_paths)
    {
        std::cout << "    " << package_path << std::endl;
    }
    if (all_package_paths.size() == 0)
    {
        return;
    }

    std::cout << "Tar above packages to \"" << tar_output_path << "\"? [y/n] ";
    std::string input;
    while (true)
    {
        getline(std::cin, input);
        if (input == "yes" || input == "y")
        {
            break;
        }
        else if (input == "no" || input == "n")
        {
            return;
        }
        else
        {
            std::cout << "Please respond with 'yes' or 'no' (or 'y' or 'n')." << std::endl;
        }
    }

    _tarPackages(all_package_paths, tar_output_path, quiet);
}

void PackageTool::tarAllPackages(const std::string &output_path, bool quiet)
{
    g_log << "-->> run cmake_tool tar" << std::endl;
    _tarAllPackages(output_path, quiet);
    g_log << "<<-- end cmake_tool tar" << std::endl
          << std::endl;
}

void PackageTool::_untarPackage(const std::string &package_path, const std::string &output_dir, bool quiet)
{
    if (StringUtils::trimmed(package_path).empty())
    {
        std::cerr << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        g_log << (EXCEPTION_TAG + "No name was specified for this project!") << std::endl;
        return;
    }

    std::string untar_output_dir = output_dir;
    if (untar_output_dir.back() != '/')
    {
        untar_output_dir += "/";
    }

    if (!FileUtils::fileExists(untar_output_dir))
    {
        std::cerr << "!! untar failed: can not output to \"" << untar_output_dir << "\", because dir not exist." << std::endl;
        g_log << "!! untar failed: can not output to \"" << untar_output_dir << "\", because dir not exist." << std::endl;
        return;
    }

    std::string package_full_path = FileUtils::getAbsolutePath(package_path);
    if (!FileUtils::fileExists(package_full_path))
    {
        std::cerr << "!! untar failed: \"" << package_full_path << "\" not exist in this PC." << std::endl;
        g_log << "!! untar failed: \"" << package_full_path << "\" not exist in this PC." << std::endl;
        return;
    }

    std::string output_dir_format = output_dir;
    if (output_dir_format.back() != '/')
    {
        output_dir_format += "/";
    }
    std::string cmd = "tar -axf " + package_full_path + " -C " + output_dir_format;
    // std::cout << cmd << std::endl;
    pid_t status = system(cmd.c_str());
    if (0 != WEXITSTATUS(status))
    {
        std::cerr << "!! untar failed: \"" << cmd << "\"" << std::endl;
        g_log << "!! untar failed: \"" << cmd << "\"" << std::endl;
    }
    else
    {
        if (!quiet)
        {
            std::cout << "<< untar success to \"" << output_dir_format << "\" :" << std::endl;
        }
        g_log << "<< untar success to \"" << output_dir_format << "\" :" << std::endl;
        cmd = "tar -tf " + package_full_path + " | awk -F \"/\" '{print $1}' | sort | uniq";
        std::string tar_contents = _executeCmd(cmd);
        StringUtils::rightTrimmedInPlace(tar_contents);
        for (const std::string &content : StringUtils::split(tar_contents, "\n"))
        {
            std::string filePath = FileUtils::getAbsolutePath(output_dir_format + content);
            if (FileUtils::isDirectory(filePath))
            {
                filePath += "/";
                _attachPackage(filePath, true);
            }
            std::cout << "    " << filePath << std::endl;
        }
    }
}

void PackageTool::untarPackage(const std::string &package_path, const std::string &output_dir, bool quiet)
{
    g_log << "-->> run cmake_tool untar" << std::endl;
    _untarPackage(package_path, output_dir, quiet);
    g_log << "<<-- end cmake_tool untar" << std::endl
          << std::endl;
}
