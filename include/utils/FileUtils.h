#pragma once

#include <string>
#include <vector>
#include <list>
#include <set>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "UtilityCommon.hpp"

class FileUtils
{
public:
    /**
     * @brief getSearchPath Prepares a directory for searching. This is really
     * only required for windows. On non windows systems input_path_string
     * is returned.
     * @param input_path_string Path to file or directory
     * @return Return formated version of input_path_string that is ready
     * for directory searching
     */
    static std::string getSearchPath(const std::string &input_path_string);

    /**
     * @brief isDirectory Determines if given input_path_string is a directory
     * @param input_path_string Path to file or directory
     * @return Return true if input_path_string is directory, false otherwise
     */
    static bool isDirectory(const std::string &input_path_string);

    /**
     * @brief fileExists Checks if a file exists
     * @param input_path_string Full path to the file
     * @return true or false for if the file exists or does not exist
     */
    static bool fileExists(const std::string &input_path_string);

    /**
     * @brief This function returns all lines from the file as a vector
     *        of strings.
     * @param input_path_string Full path to the location to read the file
     * @param[out] output_file_lines Reference to an output vector that will contain all
     *             line strings from the file
     * @return Return true if file read was successful
     */

    static bool getFileLines(const std::string &input_path_string,
                             std::vector<std::string> &output_file_lines);

    ///
    /// @brief This function returns all lines from the file as a vector
    ///        of strings
    /// @param input_path_string Full path to the file to open
    /// @return Return a vector of strings where each element is a line from the file
    ///
    static std::vector<std::string> getFileLines(const std::string &input_path_string);

    ///
    /// @brief This function returns all characters from a file as a string
    /// @param input_path_string Full path to the file to open
    /// @param[out] output_file_data std::string of all data from input_path_string
    ///
    static void getFileContents(const std::string &input_path_string, std::string &output_file_data);

    ///
    /// @brief This function returns all characters from a file as a string
    /// @param input_path_string Full path to the file to open
    /// @return std::string containing all data from the given file
    ///
    static std::string getFileContents(const std::string &input_path_string);

    /**
     * @brief getRawFileContents
     * @param input_path_string
     * @param output_file_data
     */
    static void getRawFileContents(const std::string &input_path_string,
                                   std::vector<char> &output_file_data);

    ///
    /// @brief Writes the contents of data to a file at input_path_string
    /// @param input_path_string Location on disk to write the data
    /// @param output_file_data The data to be written
    /// @return Return true upon successful writing, false otherwise
    ///
    static bool writeFileContents(const std::string &input_path_string,
                                  const std::string &output_file_data);

    ////
    /// @brief appendToFile
    /// @param input_path_string
    /// @param data_to_append_string
    /// @return
    ///
    static bool appendToFile(const std::string &input_path_string,
                             const std::string &data_to_append_string);

    /**
     * @brief This function opens output stream to a binary file
     * @param input_path_string The path to the input_path_string to write
     * @param[out] output_stream The reference to the output stream
     * @return Returns true if the file was opened, false otherwise
     */
    static bool openBinaryFileForWrite(const std::string &input_path_string,
                                       std::ofstream &output_stream);
    /**
     * @brief This function opens input stream to a binary file
     * @param input_path_string The input_path_string to read
     * @param[out] input_stream The reference to the input stream
     * @return Returns true if the file was opened, false otherwise
     */
    static bool openBinaryFileForRead(const std::string &input_path_string,
                                      std::ifstream &input_stream);

    /**
     * @brief This function opens input stream to a text file
     * @param input_path_string The input_path_string to read
     * @param[out] input_stream The reference to the input stream
     * @return Returns true if the file was opened, false otherwise
     */
    static bool openTextFileForRead(const std::string &input_path_string,
                                    std::ifstream &input_stream);

    /**
     * @brief This function opens an output stream to a text file
     * @param input_path_string The path to the input_path_string to write
     * @param[out] out The reference to the output stream
     * @return Returns true if the file was opened, false otherwise
     */
    static bool openTextFileForWrite(const std::string &input_path_string,
                                     std::ofstream &out);

    /**
     * @brief Constructs a full file path from a directory
     * path and filename.
     * @param input_path_string A directory path (eg. C:\windows, /usr/lib/,etc)
     * @param input_filename_string A file name (eg. test.xml,  run.bat, /exits/exit.bat,etc)
     * @return Constructed file path with system path seperators
     */
    static std::string buildFilePath(const std::string &input_path_string,
                                     const std::string &input_filename_string);

    /**
     * @brief Get list of files from directory
     * @param input_path_string Path to directory
     * @param recursive Enables recursive search of files in input_path_string
     * @param keepFullPath Appends the input_path_string to the file entry
     * @param filteredExtensions List of extensions to search for. Example:
     * txt, mp3, doc, etc
     * @param[out] entryListOut List of files from input_path_string.
     * This excludes the .. and . path symbols.*
     */
    static void getFileEntries(const std::string &input_path_string,
                               bool keepFullPath,
                               const std::vector<std::string> &filteredExtensions,
                               std::vector<std::string> &entryListOut,
                               bool allow_folders = false);

    /**
     * @brief getRecursiveFolders Populate folders with list of
     * recursive directories located in path at input_path_string
     * @param input_path_string Path to the recursive directory folder
     * @param folders Vector of folder paths
     */
    static void getRecursiveFolders(const std::string &input_path_string,
                                    std::set<std::string> &folders);
    /**
     * @brief getRecursiveFileEntries
     * @param input_path_string
     * @param extensionFilters
     * @param entryListOut
     */
    static void getRecursiveFileEntries(const std::string &input_path_string,
                                        const std::vector<std::string> &extensionFilters,
                                        std::vector<std::string> &entryListOut);

    /**
     * @brief getRecursiveFileEntries
     * @param input_path_string
     * @param extensionFilters
     * @return
     */
    static std::vector<std::string> getRecursiveFileEntries(const std::string &input_path_string,
                                                            const std::vector<std::string> &extensionFilters = std::vector<std::string>());

    /**
     * @brief Returns the directory portion of a input_path_string
     * @param input_path_string The full path to a file
     * @return directory of input_path_string
     */
    static std::string getDirPath(const std::string &input_path_string);

    /**
     * @brief Returns the filename portion of full file path
     * @param input_path_string The full path to a file
     * @return Returns the filename portion of input_path_string
     */
    static std::string getFileName(const std::string &input_path_string);

    /**
     * @brief Computes the total size (in bytes) of the file
     * at input_path_string
     * @param input_path_string Full path to the file
     * @return Returns the size of input_path_string in bytes
     */

    static std::uint64_t getFileSize(const std::string &input_path_string);

    /**
     * @brief getCurrentPath
     * @return
     */
    static std::string getCurrentPath();

    /**
     * @brief This function copies a file from a source to its destination
     * @param sourceFile The location of the source file
     * @param destinationFile The location of the destination file
     * @throws FileNotFoundException if either the source or destination file
     * location is invalid
     */
    static void copyFile(const std::string &sourceFile, const std::string &destinationFile);

    /**
     * @brief deleteFile
     * @param sourceFile
     */
    static void deleteFile(const std::string &sourceFile);

    /**
     * @brief createDirectory
     * @param input_path_string
     * @return
     */
    static bool createDirectory(const std::string &input_path_string);

    /**
     * @brief writeRawContentsToFile Writes buffer to input_path_string as binary
     * @param input_path_string The path to the file to write
     * @param buffer The buffer to write
     * @param buffer_size The size of the buffer to write
     */
    static void writeRawContentsToFile(const std::string &input_path_string,
                                       const char *buffer,
                                       size_t buffer_size);

    /**
     * @brief renameFile
     * @param input_path_string_orig
     * @param input_path_string_new
     */
    static void renameFile(const std::string &input_path_string_orig, const std::string &input_path_string_new);

    /**
     * @brief getRandomFolderName Get random name for a folder
     * @param length
     * @return
     */
    static std::string getRandomFolderName(int length);

    /**
     * @brief getRandomFileName
     * @param length
     * @param extension_length
     * @return
     */
    static std::string getRandomFileName(int length, int extension_length = 3);

    /**
     * @brief getFileExtension
     * @param input_path_string
     * @return
     */
    static std::string getFileExtension(const std::string &input_path_string);

    /**
     * @brief createFolder
     * @param dirFolderPath
     * @return
     */
    static int createFolder(const std::string &dirFolderPath);

    /**
     * @brief deleteFolder
     * @param input_path_string
     * @return
     */
    static int deleteFolder(const std::string &input_path_string);

    /**
     * @brief isDirectoryWritable Determines if directories at input_path_string
     * @param input_path_string
     * @return
     */
    static bool isDirectoryWritable(const std::string &input_path_string);

    /**
     * @brief getSanitizedPathName
     * @param input_path_string
     * @param use_underscore
     * @return
     */
    static std::string getSanitizedPathName(const std::string &input_path_string,
                                            bool use_underscore = false);

    /**
     * @brief getFolderEntries
     * @param input_path_string
     * @param include_full_path
     * @return
     */
    static std::vector<std::string> getFolderEntries(const std::string &input_path_string,
                                                     bool include_full_path = true);

    /**
     * @brief getFileEntries
     * @param input_path_string
     * @param filter
     * @param include_full_path
     * @return
     */
    static std::vector<std::string> getFileEntries(const std::string &input_path_string,
                                                   const std::string &filter = "",
                                                   bool include_full_path = true);

    /**
     * @brief createTemporaryFilePath
     * @param customExtension
     * @return
     */
    static std::string createTemporaryFilePath(const std::string &customExtension = "");

    static std::string getAbsolutePath(const std::string &input_path_string);
};
