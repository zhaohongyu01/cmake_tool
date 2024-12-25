#include <iostream>
#include <cassert>
#include <algorithm>

#include "CommandLineArgs.h"
#include "utils/StringUtils.h"


CommandLineArgs::CommandLineArgs(const std::string &title, int argc, char **argv, bool case_sensitive)
{
    m_title = title;
    m_is_case_sensitive = case_sensitive;
    for (int a1 = 1; a1 < argc; ++a1)
    {
        std::string arg = std::string(argv[a1]);
        if (arg == "--help" || arg == "-h")
        {
            m_help_requested = true;
        }
        m_string_inputs.emplace_back(arg);
    }
    // printStrArgs();
}

CommandLineArgs::~CommandLineArgs()
{

}

void CommandLineArgs::addOption(const std::string &long_opt_name,
                                const std::string &short_opt_name,
                                bool required,
                                const std::string &description)
{
    bool exists = false;
    CommandLineOpt opt;
    opt.description = description;
    opt.long_name = long_opt_name;
    opt.short_name = short_opt_name;
    opt.required = required;
    if (m_is_case_sensitive)
    {
        for (size_t a1 = 0; a1 < m_string_inputs.size();)
        {
            if (m_string_inputs[a1].at(0) != '-')
            {
                if (m_package_paths.end() == std::find(m_package_paths.begin(), m_package_paths.end(), m_string_inputs[a1]))
                {
                    m_package_paths.emplace_back(m_string_inputs[a1]);
                }
                ++a1;
                continue;
            }
            opt.step = getOptionStep(m_string_inputs[a1]);
            if (m_string_inputs[a1] == long_opt_name ||
                m_string_inputs[a1] == short_opt_name)
            {
                if (opt.step == 1)
                {
                    opt.value = "enable";
                }
                else
                {
                    for (size_t i = 1; i < opt.step; i++)
                    {
                        if ((a1 + i) < m_string_inputs.size())
                        {
                            opt.value += m_string_inputs[a1 + i] + " ";
                        }
                    }
                    StringUtils::trimmedInPlace(opt.value);
                }
                exists = true;
                break;
            }
            a1 += opt.step;
        }
    }
    else
    {
        for (size_t a1 = 0; a1 < m_string_inputs.size();)
        {
            if (m_string_inputs[a1].at(0) != '-')
            {
                if (m_package_paths.end() == std::find(m_package_paths.begin(), m_package_paths.end(), m_string_inputs[a1]))
                {
                    m_package_paths.emplace_back(m_string_inputs[a1]);
                }
                ++a1;
                continue;
            }
            opt.step = getOptionStep(m_string_inputs[a1]);
            if (StringUtils::equals(m_string_inputs[a1], long_opt_name) ||
                StringUtils::equals(m_string_inputs[a1], short_opt_name))
            {
                if (opt.step == 1)
                {
                    opt.value = "enable";
                }
                else
                {
                    for (size_t i = 1; i < opt.step; i++)
                    {
                        if ((a1 + i) < m_string_inputs.size())
                        {
                            opt.value += m_string_inputs[a1 + i] + " ";
                        }
                    }
                    StringUtils::trimmedInPlace(opt.value);
                }
                exists = true;
                break;
            }
            a1 += opt.step;
        }
    }

    m_options.emplace_back(opt);
    if (!exists && required)
    {
        m_error_messages.emplace_back("ERROR: The argument " + long_opt_name + " or " + short_opt_name + " is required but has not been defined!");
    }
}

std::string CommandLineArgs::value(const std::string &long_or_short_opt) const
{
    if (m_is_case_sensitive)
    {
        for (const auto &option : m_options)
        {
            if (option.long_name == long_or_short_opt ||
                option.short_name == long_or_short_opt)
            {
                return option.value;
            }
        }
    }
    else
    {
        for (const auto &option : m_options)
        {
            if (StringUtils::equals(option.long_name,long_or_short_opt) ||
                StringUtils::equals(option.short_name,long_or_short_opt))
            {
                return option.value;
            }
        }
    }
    return "";
}

size_t CommandLineArgs::getOptionStep(const std::string &string_input) const
{
    size_t step = 1;
    size_t begin = 0, end = 0;
    bool find = false;

    if (string_input.at(0) != '-')
    {
        return step;
    }

    for (size_t i = 0; i < m_string_inputs.size(); i++)
    {
        if (find && m_string_inputs[i].at(0) == '-')
        {
            end = i;
        }
        if (!find && m_string_inputs[i] == string_input)
        {
            begin = i;
            find = true;
        }
    }

    if (find)
    {
        if (end == 0)
        {
            end = m_string_inputs.size();
        }
    }
    else
    {
        end = begin + 1;
    }

    step = end - begin;
    assert(step > 0);
    return step;
}

bool CommandLineArgs::exists(const std::string &long_or_short_opt) const
{
    if (m_is_case_sensitive)
    {
        for (const auto &option : m_options)
        {
            if (option.long_name == long_or_short_opt ||
                option.short_name == long_or_short_opt)
            {
                return !(option.value.empty());
            }
        }
    }
    else
    {
        for (const auto &option : m_options)
        {
            if (StringUtils::equals(option.long_name,long_or_short_opt) ||
                StringUtils::equals(option.short_name,long_or_short_opt))
            {
                return !(option.value.empty());
            }
        }
    }
    return false;
}

bool CommandLineArgs::operator()(const std::string &long_or_short_opt) const
{
    return exists(long_or_short_opt);
}

std::string CommandLineArgs::operator[](const std::string &long_or_short_opt) const
{
    return value(long_or_short_opt);
}

void CommandLineArgs::prepare()
{
    std::cerr << std::flush;
    std::cout << std::flush;

    for (const CommandLineOpt &opt: m_options)
    {
        int first_column_width = ("  " + opt.long_name + " [" + opt.short_name + "]:").length() + 1;
        if (first_column_width > m_first_column_width)
        {
            m_first_column_width = first_column_width;
        }
    }

    if (m_error_messages.size())
    {
        for (const auto &error_msg : m_error_messages)
        {
            std::cerr << error_msg << std::endl;
        }
        printHelp();
        exit(-1);
    }
    if (m_help_requested)
    {
        printHelp();
        exit(0);
    }

}

void CommandLineArgs::printHelp()
{
    CommandLineOpt help_arg;
    help_arg.short_name = "-h";
    help_arg.long_name = "--help";
    help_arg.required = false;
    help_arg.description = "displays command help.";      
    size_t index = 0;

    std::cout << std::endl;
    std::cout << "=========================================================" << std::endl;
    if (m_title.find(" list") != m_title.npos || m_title.find(" reset") != m_title.npos)
    {
        std::cout << "Usage: " << m_title << " [options]" << std::endl << std::endl;
    }
    else if(m_title.find(" untar") != m_title.npos)
    {
        std::cout << "Usage: " << m_title << " TAR_FILE1" << " [TAR_FILE2 ...]" << " [options]" << std::endl << std::endl;
    }
    else
    {
        std::cout << "Usage: " << m_title << " PACKAGE1" << " [PACKAGE2 ...]" << " [options]" << std::endl << std::endl;
    }

    std::cout << "Allowed options:" << std::endl;
    std::cout << help_arg.toString(m_first_column_width) << std::endl;
    for (const auto &option : m_options)
    {
        std::cout << option.toString(m_first_column_width) << std::endl;
        ++index;
    }
    std::cout << "=========================================================" << std::endl;
}

void CommandLineArgs::setTitle(const std::string &title)
{
    m_title = title;
}

std::string CommandLineArgs::CommandLineOpt::toString(int first_column_width, int spaces) const
{
    if (spaces < 0)
    {
        spaces = 0;
    }
    if (first_column_width == 0)
    {
        return long_name + " [" + short_name + "]: \t" + StringUtils::repeat(" ", spaces) + description;
    }
    char output_str[128];
    snprintf(output_str, sizeof(output_str) - 1, "%-*s %s", first_column_width, ("  " + long_name + " [" + short_name + "]:").c_str(), (StringUtils::repeat(" ", spaces) + description).c_str());
    return output_str;

}

size_t CommandLineArgs::CommandLineOpt::getAttachArgsNumber() const
{
    return step - 1;
}

void CommandLineArgs::printStrArgs()
{
    std::cout << "str_args:" << std::endl;
    for (const auto &str_arg : m_string_inputs)
    {
        std::cout << str_arg << std::endl;
    }
}


void CommandLineArgs::printArgs()
{
    std::cout << "options:" << std::endl;
    for (const auto &option : m_options)
    {
        std::cout << "short_name:" << option.short_name<< std::endl;
        std::cout << "long_name:" << option.long_name<< std::endl;
        std::cout << "description:" << option.description<< std::endl;
        std::cout << "required:" << option.required<< std::endl;
        std::cout << "value:" << option.value<< std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

