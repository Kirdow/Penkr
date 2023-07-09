#pragma once

namespace Penkr::Utils
{
	const char* GetPassFile();
	std::filesystem::path GetDataDirectory();
	bool GetAbsolutePath(const std::filesystem::path& directory, std::filesystem::path& userDataResult, std::filesystem::path& defaultResult);
	std::string ReadFile(const std::filesystem::path& filepath);
	std::string GetKey(const std::string& data);
}