#pragma once

namespace Penkr::Utils
{
	// Get the name for the password file.
	const char* GetPassFile();

	// Get the absolute path for %localappdata%.
	std::filesystem::path GetDataDirectory();

	// Get the absolute paths for `User Data` and `Default` for
	// a browser's local cache path.
	bool GetAbsolutePath(const std::filesystem::path& directory, std::filesystem::path& userDataResult, std::filesystem::path& defaultResult);

	// Read the entire content of a file on disk.
	std::string ReadFile(const std::filesystem::path& filepath);

	// Get/decode the key used for the AES decryption.
	std::string GetKey(const std::string& data);
}