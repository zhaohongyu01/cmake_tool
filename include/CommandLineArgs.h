#pragma once

#include <vector>
#include <string>

class CommandLineArgs
{
private:
    struct CommandLineOpt
    {
        std::string long_name;
        std::string short_name;
        std::string description;
        std::string value;
        size_t step{0};
        bool required;
        std::string toString(int first_column_width, int spaces = 0) const;
        size_t getAttachArgsNumber() const;
    };

    std::string m_title;
    bool m_is_case_sensitive;
    bool m_help_requested{false};
    std::vector<CommandLineOpt> m_options;
    std::vector<std::string> m_package_paths;
    std::vector<std::string> m_string_inputs;
    std::vector<std::string> m_error_messages;

    int m_first_column_width{0};

public:
    CommandLineArgs(const std::string &title, int argc, char **argv, bool case_sensitive = false);
    ~CommandLineArgs();

    void addOption(const std::string &long_arg_name,
                   const std::string &short_arg_name,
                   bool required = false,
                   const std::string &description = "");

    std::string value(const std::string &long_or_short_arg) const;

    bool exists(const std::string &long_or_short_arg) const;

    bool operator()(const std::string &long_or_short_arg) const;

    std::string operator[](const std::string &long_or_short_arg) const;

    void prepare();

    void printHelp();

    void setTitle(const std::string &title);

    size_t getOptionStep(const std::string &string_input) const;

    const std::vector<std::string> &getPackagePaths()
    {
        return m_package_paths;
    }

    void printStrArgs();
    void printArgs();
};
