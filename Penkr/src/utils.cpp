#include "pch.hpp"
#include "utils.hpp"
#include "aesdecrypt.hpp"

#ifdef KTN_PLATFORM_WINDOWS
#include <ShlObj.h>
#endif

namespace Penkr::Utils
{
	const char* GetPassFile()
	{
		return "pass.csv";
	}

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

	std::string GetKey(const std::string& data)
	{
		std::string result = data;

		result = AES::Base64Decode(result);
		result = result.substr(5);
		result = AES::Unprotect(result);

		return result;
	}
}