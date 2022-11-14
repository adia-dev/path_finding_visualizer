#pragma once

#include "File.h"
#include "Colors.h"

namespace se
{
    class Logger
    {
    public:
        /* data */
        Logger() = default;
        ~Logger() = default;

        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;
        Logger(Logger &&) = delete;
        Logger &operator=(Logger &&) = delete;

        static Logger &Get()
        {
            static Logger instance;
            return instance;
        }

        static void Log(const std::string &message, const Colors &color = Colors::White)
        {
            std::cout
                << "\033"
                << COLORS_CODE[color]
                << message
                << "\033"
                << RESET;
        }

        static void Logln(const std::string &message, const Colors &color = Colors::White)
        {

            std::cout
                << "\033"
                << COLORS_CODE[color]
                << message
                << "\033"
                << RESET
                << std::endl;
        }

        static void LogWait(const std::string &message, const Colors &color = Colors::White, const std::string &waitInstructions = "Press any key to continue...")
        {
            std::cout
                << "\033"
                << COLORS_CODE[color]
                << message
                << "\033"
                << RESET
                << std::endl;

            std::cout
                << "\033"
                << COLORS_CODE[Colors::Grey]
                << waitInstructions
                << "\033"
                << RESET
                << std::endl;

            std::cin.get();
        }

        static void LogFile(const std::string &message, const std::string &outFile = __LOG__, bool append = true)
        {

            std::ofstream file;
            if (append)
                file.open(outFile, std::ofstream::out | std::ios::app);
            else
                file.open(outFile);

            if (file.is_open())
            {
                file << message << std::endl;
                file.close();
            }
            else
            {
                std::cout
                    << "\033"
                    << COLORS_CODE[Colors::Red]
                    << "Error: Could not open file: "
                    << outFile
                    << "\033"
                    << RESET
                    << std::endl;
            }
        }
    };

} // namespace se
