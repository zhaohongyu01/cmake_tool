#pragma once

#include <string>
#include <vector>

#include "utils/StringUtils.h"

enum class PackageType
{
    CMAKE_UNKNOWN,
    CMAKE_CPP_PACKAGE,
    CMAKE_C_PACKAGE,
};

struct Package
{
    Package()
    {
    }

    Package(const std::string& pkg_path, const std::string& pkg_type)
    : path(pkg_path)
    {
        if (pkg_type.empty() || StringUtils::equals(pkg_type, "CPP"))
        {
            type = PackageType::CMAKE_CPP_PACKAGE;
        }
        else if (StringUtils::equals(pkg_type, "C"))
        {
            type = PackageType::CMAKE_C_PACKAGE;
        }
    }

    Package(const std::string& pkg_path, const PackageType& pkg_type)
    : path(pkg_path)
    , type(pkg_type)
    {
    }

    std::string path;
    PackageType type{PackageType::CMAKE_UNKNOWN};
};

class PackageTool
{

public:
    explicit PackageTool(bool enable_log = false);
    ~PackageTool();

    void setLog(bool enable_log);
    void setForce(bool enable_force);
    void createPackage(const std::string &package_path, const std::string &package_type, bool quiet = false);
    void buildPackage(const std::string &package_path, bool quiet = false);
    void buildAllPackages(bool quiet = false);
    void cleanPackage(const std::string &package_path, bool quiet = false);
    void cleanAllPackages(bool quiet = false);
    void deletePackage(const std::string &package_path, bool quiet = false);
    void deleteAllPackages(bool quiet = false);
    void listPackages(bool basename_only = false, bool path_only = false);
    void resetInfo();
    void runPackage(const std::string &package_path, const std::string &program_name, const std::vector<std::string> &program_args);
    void attachPackage(const std::string &package_path, bool quiet = false);
    void attachPackagesFromFile(const std::string &file_path, bool quiet = false);
    void detachPackage(const std::string &package_path, bool quiet = false);
    void detachAllPackages(bool quiet = false);
    void tarPackages(const std::vector<std::string> &package_paths, const std::string &output_path = "./", bool quiet = false);
    void tarAllPackages(const std::string &output_path = "./", bool quiet = false);
    void untarPackage(const std::string &package_path, const std::string &output_dir = "./", bool quiet = false);

private:
    void _createPackage(const std::string &package_path, PackageType package_type, bool quiet = false);
    void _buildPackage(const std::string &package_path, bool quiet = false);
    void _buildAllPackages(bool quiet = false);
    void _cleanPackage(const std::string &package_path, bool quiet = false);
    void _cleanAllPackages(bool quiet = false);
    void _deletePackage(const std::string &package_path, bool quiet = false);
    void _deleteAllPackages(bool quiet = false);
    void _listPackages(bool basename_only = false, bool path_only = false);
    void _resetInfo();
    void _runPackage(const std::string &package_path, const std::string &program_name, const std::vector<std::string> &program_args);
    void _attachPackage(const std::string &package_path, bool quiet = false);
    void _attachPackagesFromFile(const std::string &file_path, bool quiet = false);
    void _detachPackage(const std::string &package_path, bool quiet = false);
    void _detachAllPackages(bool quiet = false);
    void _tarPackages(const std::vector<std::string> &package_paths, const std::string &output_path = "./", bool quiet = false);
    void _tarAllPackages(const std::string &output_path = "./", bool quiet = false);
    void _untarPackage(const std::string &package_path, const std::string &output_dir = "./", bool quiet = false);

    bool _isValidTemplatePath(const std::string &path);
    bool _isValidPackageType(const PackageType& type);
    bool _scanTemplateFiles();
    void _updateCurrentPackage(const std::string &package_path, const PackageType &package_type = PackageType::CMAKE_UNKNOWN);
    bool _createDirectory();
    void _createCPPProject();
    void _createCProject();
    void _createProject();
    bool _deleteDirectory();
    bool _cleanInstallFiles();
    void _getAllPackagePaths(std::vector<std::string> &output_paths);

    std::string m_createInfoPath;
    std::string m_cppCMakePath;
    std::string m_cppMainPath;
    std::string m_cCMakePath;
    std::string m_cMainPath;

    Package m_currentPackage;

    bool m_force{false};
};
