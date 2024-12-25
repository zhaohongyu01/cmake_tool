#include <iostream>
#include <cstring>
#include <string>

#include "utils/StringUtils.h"
#include "utils/SystemUtils.h"

#include "CommandLineArgs.h"
#include "PackageTool.hpp"

static void _printValidSubcmds()
{
    printf("Available <subcommands>:\n");
    printf("   %-8s  %s\n", "help", "Show help message.");
    printf("   %-8s  %s\n", "create", "Create cmake projects.");
    printf("   %-8s  %s\n", "build", "Build and install cmake projects.");
    printf("   %-8s  %s\n", "clean", "Clean cmake projects only those install files and cache files.");
    printf("   %-8s  %s\n", "delete", "Delete cmake projects all files (including 'src/' directory). Be careful!");
    printf("   %-8s  %s\n", "list", "List cmake projects path info.");
    printf("   %-8s  %s\n", "reset", "Reset cmake projects path info.");
    printf("   %-8s  %s\n", "run", "Run a cmake project program.");
    printf("   %-8s  %s\n", "attach", "Attach cmake projects to cmake_tool.");
    printf("   %-8s  %s\n", "detach", "Detach cmake projects from cmake_tool.");
    printf("   %-8s  %s\n", "tar", "Tar cmake_tool projects output to a compression package.");
    printf("   %-8s  %s\n", "untar", "Untar a compression package output to cmake_tool projects.");
}

static void _printHelp()
{
    printf("\nUsage: cmake_tool <subcommand> [options]\n");
    _printValidSubcmds();
}

int main(int argc, char **argv)
{
    if (argc < 2 || 0 == strcmp(argv[1], "help") || 0 == strcmp(argv[1], "-h"))
    {
        _printHelp();
        return 0;
    }

    ++argv;
    --argc;

    PackageTool package_tool;

    if (0 == strcmp(argv[0], "create"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool create PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        std::ostringstream info;
        info << "type of packages." << std::endl;
        info << "\t\tCPP - CMake C++ Package (Default)" << std::endl;
        info << "\t\tC   - CMake C Package" << std::endl;

        CommandLineArgs create_args("cmake_tool create", argc, argv);
        create_args.addOption("--log", "-l", false, "log debug info to file.");
        create_args.addOption("--type", "-t", false, info.str());
        create_args.prepare();

        // get enable log
        bool enable_log = create_args.exists("-l");
        package_tool.setLog(enable_log);

        // get package type
        std::string package_type = create_args.value("-t");

        // create packages
        for (const std::string &package_path : create_args.getPackagePaths())
        {
            package_tool.createPackage(package_path, package_type);
        }
    }
    else if (0 == strcmp(argv[0], "build"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool build PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs build_args("cmake_tool build", argc, argv);
        build_args.addOption("--log", "-l", false, "log debug info to file.");
        build_args.addOption("--all", "-a", false, "build all packages of 'cmake_tool list'.");
        build_args.addOption("--force", "-f", false, "force build all same name packages.");
        build_args.prepare();

        // get enable log
        bool enable_log = build_args.exists("-l");
        package_tool.setLog(enable_log);
        // get enable force
        bool enable_force = build_args.exists("-f");
        package_tool.setForce(enable_force);

        // build packages
        if (build_args.exists("-a"))
        {
            package_tool.buildAllPackages();
        }
        else
        {
            for (const std::string &package_path : build_args.getPackagePaths())
            {
                package_tool.buildPackage(package_path);
            }
        }
    }
    else if (0 == strcmp(argv[0], "clean"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool clean PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs clean_args("cmake_tool clean", argc, argv);
        clean_args.addOption("--log", "-l", false, "log debug info to file.");
        clean_args.addOption("--all", "-a", false, "clean all packages of 'cmake_tool list'.");
        clean_args.addOption("--force", "-f", false, "force clean all same name packages.");
        clean_args.prepare();

        // get enable log
        bool enable_log = clean_args.exists("-l");
        package_tool.setLog(enable_log);
        // get enable force
        bool enable_force = clean_args.exists("-f");
        package_tool.setForce(enable_force);

        // clean packages
        if (clean_args.exists("-a"))
        {
            package_tool.cleanAllPackages();
        }
        else
        {
            for (const std::string &package_path : clean_args.getPackagePaths())
            {
                package_tool.cleanPackage(package_path);
            }
        }
    }
    else if (0 == strcmp(argv[0], "delete"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool delete PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs delete_args("cmake_tool delete", argc, argv);
        delete_args.addOption("--log", "-l", false, "log debug info to file.");
        delete_args.addOption("--all", "-a", false, "delete all packages of 'cmake_tool list'.");
        delete_args.addOption("--force", "-f", false, "force delete all same name packages.");
        delete_args.prepare();

        // get enable log
        bool enable_log = delete_args.exists("-l");
        package_tool.setLog(enable_log);
        // get enable force
        bool enable_force = delete_args.exists("-f");
        package_tool.setForce(enable_force);

        // delete packages
        if (delete_args.exists("-a"))
        {
            package_tool.deleteAllPackages();
        }
        else
        {
            for (const std::string &package_path : delete_args.getPackagePaths())
            {
                package_tool.deletePackage(package_path);
            }
        }
    }
    else if (0 == strcmp(argv[0], "list"))
    {
        CommandLineArgs list_args("cmake_tool list", argc, argv);
        list_args.addOption("--log", "-l", false, "log debug info to file.");
        list_args.addOption("--path", "-p", false, "only show packages full path.");
        list_args.addOption("--basename", "-b", false, "only show packages basename.");
        list_args.prepare();

        // get enable log
        bool enable_log = list_args.exists("-l");
        package_tool.setLog(enable_log);

        // list packages
        bool basename_only = list_args.exists("-b");
        bool path_only = list_args.exists("-p");
        package_tool.listPackages(basename_only, path_only);
    }
    else if (0 == strcmp(argv[0], "reset"))
    {
        CommandLineArgs reset_args("cmake_tool reset", argc, argv);
        reset_args.addOption("--log", "-l", false, "log debug info to file.");
        reset_args.prepare();

        // get enable log
        bool enable_log = reset_args.exists("-l");
        package_tool.setLog(enable_log);

        // reset install info
        package_tool.resetInfo();
    }
    else if (0 == strcmp(argv[0], "run"))
    {
        if (argc < 3)
        {
            printf("cmake_tool: error: You must specify a package name and a program name.\n");
            printf("\nUsage: cmake_tool run PACKAGE PROGRAM [program options].\n");
            return 0;
        }

        std::string package_path = std::string(argv[1]);
        std::string program_name = std::string(argv[2]);
        std::vector<std::string> program_args;
        for (int a1 = 3; a1 < argc; ++a1)
        {
            program_args.emplace_back(std::string(argv[a1]));
        }

        // run a package
        package_tool.runPackage(package_path, program_name, program_args);
    }
    else if (0 == strcmp(argv[0], "attach"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool attach PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs attach_args("cmake_tool attach", argc, argv);
        attach_args.addOption("--log", "-l", false, "log debug info to file.");
        attach_args.addOption("--read_file", "-r", false, "read file contents and attach to cmake_tool.");
        attach_args.prepare();

        // get enable log
        bool enable_log = attach_args.exists("-l");
        package_tool.setLog(enable_log);

        // attach packages
        for (const std::string &package_path : attach_args.getPackagePaths())
        {
            package_tool.attachPackage(package_path);
        }
        std::string file_path = attach_args.value("-r");
        if (!file_path.empty())
        {
            package_tool.attachPackagesFromFile(file_path);
        }
    }
    else if (0 == strcmp(argv[0], "detach"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool detach PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs detach_args("cmake_tool detach", argc, argv);
        detach_args.addOption("--log", "-l", false, "log debug info to file.");
        detach_args.addOption("--all", "-a", false, "detach all packages from cmake_tool.");
        detach_args.addOption("--force", "-f", false, "force detach all same name packages.");
        detach_args.prepare();

        // get enable log
        bool enable_log = detach_args.exists("-l");
        package_tool.setLog(enable_log);
        // get enable force
        bool enable_force = detach_args.exists("-f");
        package_tool.setForce(enable_force);

        // attach packages
        if (detach_args.exists("-a"))
        {
            package_tool.detachAllPackages();
        }
        else
        {
            for (const std::string &package_path : detach_args.getPackagePaths())
            {
                package_tool.detachPackage(package_path);
            }
        }
    }
    else if (0 == strcmp(argv[0], "tar"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool tar PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs tar_args("cmake_tool tar", argc, argv);
        tar_args.addOption("--log", "-l", false, "log debug info to file.");
        tar_args.addOption("--all", "-a", false, "tar all packages of 'cmake_tool list'.");
        tar_args.addOption("--output", "-o", false, "output path of tar file. [default = ./cmake_tool_packages.tar.gz]");
        tar_args.prepare();

        // get enable log
        bool enable_log = tar_args.exists("-l");
        package_tool.setLog(enable_log);

        // get output path
        std::string output_path = tar_args.value("-o");

        // tar packages
        if (!output_path.empty())
        {
            if (tar_args.exists("-a"))
            {
                package_tool.tarAllPackages(output_path);
            }
            else
            {
                package_tool.tarPackages(tar_args.getPackagePaths(), output_path);
            }
        }
        else
        {
            if (tar_args.exists("-a"))
            {
                package_tool.tarAllPackages();
            }
            else
            {
                package_tool.tarPackages(tar_args.getPackagePaths());
            }
        }
    }
    else if (0 == strcmp(argv[0], "untar"))
    {
        if (argc < 2)
        {
            printf("cmake_tool: error: You must specify at least one package name.\n");
            printf("\nUsage: cmake_tool untar PACKAGE1 [PACKAGE2 ...] [options]\nIf you need more help, please add option \"-h\".\n");
            return 0;
        }

        CommandLineArgs untar_args("cmake_tool untar", argc, argv);
        untar_args.addOption("--log", "-l", false, "log debug info to file.");
        untar_args.addOption("--output", "-o", false, "output path of untar dir. [default = ./]");
        untar_args.prepare();

        // get enable log
        bool enable_log = untar_args.exists("-l");
        package_tool.setLog(enable_log);

        // get output path
        std::string output_path = untar_args.value("-o");

        // untar packages
        for (const std::string &package_path : untar_args.getPackagePaths())
        {
            if (!output_path.empty())
            {
                package_tool.untarPackage(package_path, output_path);
            }
            else
            {
                package_tool.untarPackage(package_path);
            }
        }
    }
    else
    {
        printf("Invalid command, please check.\n\n");
        _printHelp();
    }

    return 0;
}