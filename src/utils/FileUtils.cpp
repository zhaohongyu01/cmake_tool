#include "utils/FileUtils.h"
#include "utils/RandomUtils.h"
#include "utils/StringUtils.h"
#include "utils/DateTimeUtils.hpp"
#include "utils/Exception.hpp"

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <lmcons.h>
#else
#include <fts.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#endif

#ifdef __unix
#define fopen_s(pFile, filename, mode) ((*(pFile)) = fopen((filename), (mode))) == NULL
#endif

#ifdef _WIN32
#ifndef DWORD
#define DWORD unsigned long
#endif
extern "C" WINBASEAPI DWORD WINAPI GetLongPathNameA(LPCSTR, LPSTR, DWORD);
#endif

std::string FileUtils::getSearchPath(const std::string &input_path_string)
{
#ifdef _WIN32
    std::string path(input_path_string);
    if (path[path.size() - 1] != '*')
    {
        if (path[path.size() - 1] != '/' && path[path.size() - 1] != '\\')
        {
            path += '/';
        }

        path += '*';
    }
    return path;
#else
    return input_path_string;

#endif
}

bool FileUtils::isDirectory(const std::string &input_path_string)
{
#ifdef _WIN32
    DWORD attr;

    if (input_path_string.size() && (attr = GetFileAttributesA(input_path_string.c_str())) != INVALID_FILE_ATTRIBUTES)
    {
        if (attr & FILE_ATTRIBUTE_DIRECTORY)
        {
            return true;
        }
    }

#else
    struct stat si;

    if (input_path_string.size() && (stat(input_path_string.c_str(), &si)) == 0)
    {
        if (S_ISDIR(si.st_mode))
        {
            return true;
        }
    }
#endif

    return false;
}

bool FileUtils::fileExists(const std::string &input_path_string)
{
    std::ifstream in(input_path_string.c_str(), std::ios::in);
    return in.is_open();
}

bool FileUtils::getFileLines(const std::string &input_path_string,
                             std::vector<std::string> &output_file_lines)
{
    if (!fileExists(input_path_string))
    {
        return false;
    }
    std::fstream in(input_path_string.c_str());
    std::string line;
    bool hasData = false;
    while (in)
    {
        std::getline(in, line);
        if (!in)
        {
            break;
        }
        hasData = true;
        output_file_lines.push_back(line);
    }
    in.close();

    return hasData;
}

void FileUtils::getFileContents(const std::string &input_path_string, std::string &output_file_data)
{
    std::fstream in(input_path_string.c_str(), std::ios::in);

    while (in)
    {
        char c = (static_cast<char>(in.get()));
        if (!in)
        {
            break;
        }
        output_file_data.push_back(c);
    }
    in.close();
}

std::vector<std::string> FileUtils::getFileLines(const std::string &input_path_string)
{
    std::vector<std::string> lines;
    getFileLines(input_path_string, lines);
    return lines;
}

std::string FileUtils::getFileContents(const std::string &input_path_string)
{
    std::string data;
    getFileContents(input_path_string, data);
    return data;
}

void FileUtils::getRawFileContents(const std::string &input_path_string, std::vector<char> &output_file_data)
{
    size_t length;
    FILE *rawFilePtr;
    if (fopen_s(&rawFilePtr, input_path_string.c_str(), "rb"))
    {
    }
    if (rawFilePtr)
    {
        fseek(rawFilePtr, 0, SEEK_END);
        length = ftell(rawFilePtr);
        fseek(rawFilePtr, 0, SEEK_SET);
        output_file_data.resize(length);
        if (0 == fread(&output_file_data[0], 1, length, rawFilePtr))
        {
        }
        fclose(rawFilePtr);
    }
}

bool FileUtils::writeFileContents(const std::string &input_path_string,
                                  const std::string &output_file_data)
{
    std::ofstream out(input_path_string.c_str());
    bool success = false;
    if (out)
    {
        out << output_file_data;
        success = true;
    }
    out.close();
    return success;
}

bool FileUtils::appendToFile(const std::string &input_path_string,
                             const std::string &data_to_append_string)
{

    std::ofstream out(input_path_string.c_str(), std::ios::app);
    if (out.is_open())
    {
        out << data_to_append_string;
        out.close();
        return true;
    }
    return false;
}

bool FileUtils::openBinaryFileForWrite(const std::string &input_path_string,
                                       std::ofstream &output_stream)
{
    output_stream.open(input_path_string.c_str(), std::ios::binary);
    return (output_stream.is_open());
}

bool FileUtils::openBinaryFileForRead(const std::string &input_path_string,
                                      std::ifstream &input_stream)
{
    input_stream.open(input_path_string.c_str(), std::ios::binary);
    return (input_stream.is_open());
}

bool FileUtils::openTextFileForRead(const std::string &input_path_string,
                                    std::ifstream &input_stream)
{
    input_stream.open(input_path_string.c_str());
    return (input_stream.is_open());
}

bool FileUtils::openTextFileForWrite(const std::string &input_path_string,
                                     std::ofstream &out)
{
    out.open(input_path_string.c_str());
    return (out.is_open());
}

std::string FileUtils::buildFilePath(const std::string &dirPath, const std::string &input_filename_string)
{
    std::string pathSep, file, dir;

    if (StringUtils::trimmed(dirPath).size() == 0)
    {
        return input_filename_string;
    }

#ifdef __WIN32
    pathSep = "\\";
    dir = StringUtils::replace(dirPath, "/", pathSep);
    file = StringUtils::replace(input_filename_string, "/", pathSep);
#else
    pathSep = "/";
    dir = StringUtils::replace(dirPath, "\\", pathSep);
    file = StringUtils::replace(input_filename_string, "\\", pathSep);
#endif

    bool pathHasSep = StringUtils::endsWith(StringUtils::trimmed(dir), pathSep);
    bool fileHasSep = StringUtils::startsWith(StringUtils::trimmed(file), pathSep);

    if (!pathHasSep && !fileHasSep)
    {
        return dir + pathSep + file;
    }
    if (pathHasSep && fileHasSep)
    {
        int dirSize = static_cast<int>(dir.size());
        for (int a1 = dirSize - 1; a1 >= 0; --a1)
        {
            if (dir[a1] == ' ')
            {
                continue;
            }
            else if (dir[a1] == pathSep[0])
            {
                dir.erase(dir.begin() + a1);
                break;
            }
        }
    }
    return dir + file;
}

void FileUtils::getRecursiveFolders(const std::string &dirPath,
                                    std::set<std::string> &folders)
{
    std::vector<std::string> entryListOut;
    getFileEntries(dirPath, true, std::vector<std::string>(), entryListOut);

    for (const std::string &folder : entryListOut)
    {
        if (isDirectory(folder))
        {
            folders.insert(folder);
            getRecursiveFolders(folder, folders);
        }
    }
}

void FileUtils::getFileEntries(const std::string &dirPath,
                               bool keepFullPath,
                               const std::vector<std::string> &filteredExtensions,
                               std::vector<std::string> &entryListOut,
                               bool allow_folders)
{

    std::string path = getSearchPath(dirPath);
#ifdef _WIN32
    {
        HANDLE h;
        WIN32_FIND_DATAA fdat;

        h = FindFirstFileA(path.c_str(), &fdat);

        if (INVALID_HANDLE_VALUE == h)
        {
            return;
        }

        do
        {

            std::string fileOrFolder(fdat.cFileName);
            std::string fullFileOrFolder = buildFilePath(dirPath, fileOrFolder);

            if ((!filteredExtensions.empty() && !StringUtils::endsWithMultiple(fileOrFolder, filteredExtensions, false)))
            {
                continue;
            }

            int attribute = GetFileAttributes(fullFileOrFolder.c_str());
            if (!allow_folders && attribute == FILE_ATTRIBUTE_DIRECTORY)
            {
                continue;
            }
            entryListOut.push_back(keepFullPath ? fullFileOrFolder : fileOrFolder);

        } while (FindNextFileA(h, &fdat) != 0);

        FindClose(h);
        std::set<std::string> sortedFileSet(entryListOut.begin(), entryListOut.end());
        entryListOut.clear();
        entryListOut = std::vector<std::string>(sortedFileSet.begin(), sortedFileSet.end());
        StringUtils::removeAllThatEndWith(entryListOut, "\\.");
        StringUtils::removeAllThatEndWith(entryListOut, "\\..");
    }
#else
    {
        (void)allow_folders;
        DIR *dir = opendir(path.c_str());
        struct dirent *dp;

        if (NULL == dir)
        {
            return;
        }

        while ((dp = readdir(dir)) != NULL)
        {
            std::string fileOrFolder(dp->d_name);
            if (!filteredExtensions.empty() && !StringUtils::endsWithMultiple(fileOrFolder, filteredExtensions, false))
            {
                continue;
            }

            entryListOut.push_back(dp->d_name);
            if (keepFullPath)
            {
                entryListOut[entryListOut.size() - 1] =
                    buildFilePath(dirPath, entryListOut.back());
            }
        }
        closedir(dir);
        std::set<std::string> sortedFileSet(entryListOut.begin(), entryListOut.end());
        entryListOut.clear();
        entryListOut = std::vector<std::string>(sortedFileSet.begin(), sortedFileSet.end());
        StringUtils::removeAllThatEndWith(entryListOut, "/.");
        StringUtils::removeAllThatEndWith(entryListOut, "/..");
    }
#endif
}

void FileUtils::getRecursiveFileEntries(const std::string &dirPath,
                                        const std::vector<std::string> &extensionFilters,
                                        std::vector<std::string> &entryListOut)
{
    // Add entries from dirpath
    getFileEntries(dirPath, true, extensionFilters, entryListOut);

    std::set<std::string> folders;
    getRecursiveFolders(dirPath, folders);
    for (const std::string &folder : folders)
    {
        getFileEntries(folder, true, extensionFilters, entryListOut);
    }
}

std::vector<std::string> FileUtils::getRecursiveFileEntries(const std::string &dirPath,
                                                            const std::vector<std::string> &extensionFilters)
{
    std::vector<std::string> entryListOut;
    getRecursiveFileEntries(dirPath, extensionFilters, entryListOut);
    return entryListOut;
}

std::string FileUtils::getDirPath(const std::string &input_path_string)
{
    std::string universalFilePath = StringUtils::replace(input_path_string, "\\", "/");
    size_t index = 0;

    if ((index = universalFilePath.find_last_of('/')) != std::string::npos)
    {
        return universalFilePath.substr(0, index + 1);
    }
    return "";
}

std::string FileUtils::getFileName(const std::string &input_path_string)
{
    std::string universalFilePath = StringUtils::replace(input_path_string, "\\", "/");
    size_t index = 0;

    if ((index = universalFilePath.find_last_of('/')) != std::string::npos)
    {
        return universalFilePath.substr(index + 1);
    }
    return input_path_string;
}

std::uint64_t FileUtils::getFileSize(const std::string &input_path_string)
{
    std::ifstream in(input_path_string.c_str(),
                     std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

std::string FileUtils::getCurrentPath()
{
    char dir[c_MAX_PATH];
#ifdef _WIN32
    GetCurrentDirectoryA(c_MAX_PATH, dir);
#else
    if (!getcwd(dir, c_MAX_PATH))
    {
    }
#endif
    return std::string(dir);
}

void FileUtils::copyFile(const std::string &sourceFile,
                         const std::string &destinationFile)
{
    std::ifstream src(sourceFile.c_str(), std::ios::binary);
    std::ofstream dst(destinationFile.c_str(), std::ios::binary);
    if (!src.is_open())
    {
        std::string msg = EXCEPTION_TAG + "Could not open source file at \"" + sourceFile + "\"";
        throw FileNotFoundException(msg);
    }
    if (!dst.is_open())
    {
        std::string msg = EXCEPTION_TAG + "Could not open destination file at \"" + destinationFile + "\"";
        throw FileNotFoundException(msg);
    }
    dst << src.rdbuf();
}

void FileUtils::deleteFile(const std::string &sourceFile)
{
    if (FileUtils::fileExists(sourceFile))
    {
        if (remove(sourceFile.c_str()) != 0)
        {
            std::string msg = EXCEPTION_TAG + "Could not remove file at \"" + sourceFile + "\"";
            throw FileAccessException(msg);
        }
    }
}

bool FileUtils::createDirectory(const std::string &dirPath)
{
#if defined(_WIN32)
    CreateDirectoryA(dirPath.c_str(), NULL);
    // int nError = GetLastError();
#else
    // mkdir(dirPath.c_str(), 0733);
    std::string cmd = "mkdir -p " + dirPath;
    pid_t status = system(cmd.c_str());
    (void)status;
#endif
    return isDirectory(dirPath);
}

void FileUtils::writeRawContentsToFile(const std::string &input_path_string,
                                       const char *buffer,
                                       size_t buffer_size)
{
    std::ofstream out(input_path_string.c_str(), std::ios::out | std::ios::binary);
    if (!out.is_open())
    {
        out.close();
        throw FileAccessException(EXCEPTION_TAG + "Could not open file for write at \"" + input_path_string + "\"");
    }
    out.write(buffer, buffer_size);
    out.close();
}

void FileUtils::renameFile(const std::string &input_path_string_orig, const std::string &input_path_string_new)
{
    std::rename(input_path_string_orig.c_str(), input_path_string_new.c_str());
}

std::string FileUtils::getRandomFolderName(int length)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> choice_rand(0, 100);
    std::uniform_int_distribution<char> az_rand('a', 'z');
    std::uniform_int_distribution<char> AZ_rand('A', 'Z');
    std::uniform_int_distribution<char> AF_rand('A', 'F');
    std::uniform_int_distribution<char> _09_rand('A', 'Z');

    std::string file;
    for (int a1 = 0; a1 < length; ++a1)
    {
        if (choice_rand(rng) > 75)
        {
            file.push_back(az_rand(rng));
        }
        else if (choice_rand(rng) > 50)
        {
            file.push_back(AZ_rand(rng));
        }
        else if (choice_rand(rng) > 25)
        {
            file.push_back(_09_rand(rng));
        }
        else
        {
            file.push_back(AF_rand(rng));
        }
    }
    return file;
}

std::string FileUtils::getRandomFileName(int length, int extension_length)
{
    std::string file = getRandomFolderName(length + extension_length);
    file.push_back('.');
    file.insert(file.begin() + (file.size() - extension_length), '.');
    return file;
}

std::string FileUtils::getFileExtension(const std::string &input_path_string)
{
    std::string path = StringUtils::trimmed(input_path_string);
    std::string ext;
    bool hasExt = false;
    for (int a1 = static_cast<int>(path.size() - 1); a1 >= 0; --a1)
    {
        if (path[a1] == '.')
        {
            if (a1 > 0)
            {
                hasExt = true;
            }
            break;
        }
        else if (path[a1] == '\\' || path[a1] == '/')
        {
            break;
        }
        ext.insert(ext.begin() + 0, path[a1]);
    }
    return hasExt ? ext : "";
}

int FileUtils::createFolder(const std::string &dirFolderPath)
{
    return createDirectory(dirFolderPath);
}

int FileUtils::deleteFolder(const std::string &dirPath)
{
#ifdef _WIN32
    std::string refcstrRootDirectory = StringUtils::replace(dirPath, "/", "\\");
    bool bDeleteSubdirectories = true;

    bool bSubdirectory = false;       // Flag, indicating whether
                                      // subdirectories have been found
    HANDLE hFile;                     // Handle to directory
    std::string strFilePath;          // Filepath
    std::string strPattern;           // Pattern
    WIN32_FIND_DATAA FileInformation; // File information

    strPattern = refcstrRootDirectory + "\\*.*";
    hFile = ::FindFirstFileA(strPattern.c_str(), &FileInformation);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (FileInformation.cFileName[0] != '.')
            {
                strFilePath.erase();
                strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

                if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (bDeleteSubdirectories)
                    {
                        // Delete subdirectory
                        int iRC = deleteFolder(strFilePath);
                        if (iRC)
                            return iRC;
                    }
                    else
                        bSubdirectory = true;
                }
                else
                {
                    // Set file attributes
                    if (::SetFileAttributesA(strFilePath.c_str(),
                                             FILE_ATTRIBUTE_NORMAL) == FALSE)
                        return ::GetLastError();

                    // Delete file
                    if (::DeleteFileA(strFilePath.c_str()) == FALSE)
                        return ::GetLastError();
                }
            }
        } while (::FindNextFileA(hFile, &FileInformation) == TRUE);

        // Close handle
        ::FindClose(hFile);

        DWORD dwError = ::GetLastError();
        if (dwError != ERROR_NO_MORE_FILES)
            return dwError;
        else
        {
            if (!bSubdirectory)
            {
                // Set directory attributes
                if (::SetFileAttributesA(refcstrRootDirectory.c_str(), FILE_ATTRIBUTE_NORMAL) == FALSE)
                {
                    return ::GetLastError();
                }

                // Delete directory
                if (::RemoveDirectoryA(refcstrRootDirectory.c_str()) == FALSE)
                {
                    return ::GetLastError();
                }
            }
        }
    }

    return 0;
#else
    const char *dir = dirPath.c_str();
    int ret = 0;
    FTS *ftsp = NULL;
    FTSENT *curr;

    // Cast needed (in C) because fts_open() takes a "char * const *", instead
    // of a "const char * const *", which is only allowed in C++. fts_open()
    // does not modify the argument.
    char *files[] = {(char *)dir, NULL};

    // FTS_NOCHDIR  - Avoid changing cwd, which could cause unexpected behavior
    //                in multithreaded programs
    // FTS_PHYSICAL - Don't follow symlinks. Prevents deletion of files outside
    //                of the specified directory
    // FTS_XDEV     - Don't cross filesystem boundaries
    ftsp = fts_open(files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
    if (!ftsp)
    {
        fprintf(stderr, "%s: fts_open failed: %s\n", dir, strerror(errno));
        ret = -1;
        goto finish;
    }

    while ((curr = fts_read(ftsp)))
    {
        switch (curr->fts_info)
        {
        case FTS_NS:
        case FTS_DNR:
        case FTS_ERR:
            fprintf(stderr, "%s: fts_read error: %s\n",
                    curr->fts_accpath, strerror(curr->fts_errno));
            break;

        case FTS_DC:
        case FTS_DOT:
        case FTS_NSOK:
            // Not reached unless FTS_LOGICAL, FTS_SEEDOT, or FTS_NOSTAT were
            // passed to fts_open()
            break;

        case FTS_D:
            // Do nothing. Need depth-first search, so directories are deleted
            // in FTS_DP
            break;

        case FTS_DP:
        case FTS_F:
        case FTS_SL:
        case FTS_SLNONE:
        case FTS_DEFAULT:
            if (remove(curr->fts_accpath) < 0)
            {
                fprintf(stderr, "%s: Failed to remove: %s\n",
                        curr->fts_path, strerror(errno));
                ret = -1;
            }
            break;
        }
    }

finish:
    if (ftsp)
    {
        fts_close(ftsp);
    }

    return ret;

#endif
}

bool FileUtils::isDirectoryWritable(const std::string &directoryPath)
{
    std::string file = buildFilePath(directoryPath, ".writable.temp");
    std::ofstream out(file.c_str(), std::ios::out);
    bool writable = out.is_open();
    out.close();
    if (writable)
    {
        deleteFile(file);
    }
    return writable;
}

std::string FileUtils::getSanitizedPathName(const std::string &input_path_string,
                                            bool use_underscore)
{
    std::string pathName = input_path_string;
    for (int a1 = static_cast<int>(pathName.size() - 1); a1 >= 0; --a1)
    {
        if (pathName[a1] == '<' ||
            pathName[a1] == '>' ||
            pathName[a1] == '|' ||
            pathName[a1] == '\"' ||
            pathName[a1] == '\b' ||
            pathName[a1] == '\0' ||
            pathName[a1] == '\t' ||
            pathName[a1] < 32)
        {
            if (use_underscore)
            {
                pathName[a1] = '_';
            }
            else
            {
                pathName.erase(pathName.begin() + a1);
            }
        }
    }
    return pathName;
}

std::vector<std::string> FileUtils::getFolderEntries(const std::string &dirPath,
                                                     bool include_full_path)
{
    std::vector<std::string> folders;
    getFileEntries(dirPath, include_full_path, {""}, folders, true);
    for (int a1 = folders.size() - 1; a1 >= 0; --a1)
    {
        if (!isDirectory(folders[a1]))
        {
            folders.erase(folders.begin() + a1);
        }
    }
    return folders;
}

std::vector<std::string> FileUtils::getFileEntries(const std::string &dirPath,
                                                   const std::string &filter,
                                                   bool include_full_path)
{
    std::vector<std::string> entries;
    std::vector<std::string> filters;
    if (!StringUtils::isEmpty(filter))
    {
        filters = StringUtils::split(filter, ",");
    }
    getFileEntries(dirPath, include_full_path, filters, entries);
    return entries;
}

std::string FileUtils::createTemporaryFilePath(const std::string &customExtension)
{
    std::string tmpfolder;
#ifdef _WIN32
    char dir[c_MAX_PATH];

    GetTempPathA(c_MAX_PATH, dir);
    GetLongPathNameA(dir, dir, c_MAX_PATH);

    std::string dirStr(dir);
    StringUtils::replaceInPlace(dirStr, "\\", "/");
    tmpfolder = dirStr;
#else
    tmpfolder = std::string("/tmp");
#endif
    return FileUtils::buildFilePath(tmpfolder, StringUtils::createUUID() + customExtension);
}

std::string FileUtils::getAbsolutePath(const std::string &input_path_string)
{
#ifdef _WIN32
#define max_path 4096
    char resolved_path[max_path] = {0};
    _fullpath(resolved_path, input_path_string.c_str(), max_path);
#else
#define max_path 40960
    if (input_path_string.front() == '/')
    {
        return input_path_string;
    }
    char resolved_path[max_path] = {0};
    if (!realpath(input_path_string.c_str(), resolved_path))
    {
    }
#endif
    return std::string(resolved_path);
}
