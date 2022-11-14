#pragma once

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

namespace se
{
    namespace fs = std::filesystem;

    class File
    {
    public:
        /* data */
        File() = default;
        ~File() = default;

        File(const File &) = delete;
        File &operator=(const File &) = delete;
        File(File &&) = delete;
        File &operator=(File &&) = delete;

        static std::string Read(const std::string &path)
        {
            std::ifstream file(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            return stream.str();
        }

        static void Write(const std::string &path, const std::string &content)
        {
            std::ofstream file(path);
            file << content;
            file.close();
        }

        static bool Exists(const std::string &path)
        {
            return fs::exists(path);
        }

        static bool IsFile(const std::string &path)
        {
            return fs::is_regular_file(path);
        }

        static bool IsDirectory(const std::string &path)
        {
            return fs::is_directory(path);
        }

        static std::string GetFileName(const std::string &path)
        {
            return fs::path(path).filename().string();
        }

        static std::string GetFileNameWithoutExtension(const std::string &path)
        {
            return fs::path(path).stem().string();
        }

        static std::string GetFileExtension(const std::string &path)
        {
            return fs::path(path).extension().string();
        }

        static std::string GetDirectory(const std::string &path)
        {
            return fs::path(path).parent_path().string();
        }

        static std::string GetAbsolutePath(const std::string &path)
        {
            return fs::absolute(path).string();
        }

        static std::string GetRelativePath(const std::string &path)
        {
            return fs::relative(path).string();
        }

        static std::string GetWorkingDirectory()
        {
            return fs::current_path().string();
        }

        static void SetWorkingDirectory(const std::string &path)
        {
            fs::current_path(path);
        }

        static const char *GetLogDirectory()
        {
            return __LOG__;
        }

        static std::string GetTempDirectory()
        {
            return fs::temp_directory_path().string();
        }
    };

} // namespace se
