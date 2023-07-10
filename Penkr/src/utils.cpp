#include "pch.hpp"
#include "utils.hpp"
#include "aesdecrypt.hpp"

#ifdef KTN_PLATFORM_WINDOWS
#include <ShlObj.h>
#endif

namespace Penkr::Utils
{
	// This is a constant getter for the Pass File name.
	// Could have made this a const variable but it doesn't matter
	// really since it's only used once.
	const char* GetPassFile()
	{
		return "pass.csv";
	}

	// Get the data directory. This is for %localappdata% which evaluates to
	// C:\Users\<username>\AppData\Local
	// 
	// std::filesystem::path doesn't actually support the environment variables
	// so we need a platform specific conversion for this.
	std::filesystem::path GetDataDirectory()
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

	// Get the absolute path of `User Data` and `Default` for the specific browser.
	// This returns false if either of them does not exist.
	bool GetAbsolutePath(const std::filesystem::path& directory, std::filesystem::path& userDataResult, std::filesystem::path& defaultResult)
	{
		auto local = GetDataDirectory();
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

	// This reads the full content of a file depending on the path input.
	// This is such a trivial thing to have easily available.
	std::string ReadFile(const std::filesystem::path& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
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
			std::cout << "file does not exist: " << filepath << "\n";
		}

		return result;
	}

	// This decodes the key used for decrypting the database.
	std::string GetKey(const std::string& data)
	{
		std::string result = data;

		result = AES::Base64Decode(result);
		result = result.substr(5);
		result = AES::Unprotect(result);

		return result;
	}
}