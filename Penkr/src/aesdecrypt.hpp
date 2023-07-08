#pragma once

#include <string>

namespace Penkr::AES
{
	std::string DecryptAES(const std::string& cipher, const std::string& key);
	std::string Base64Decode(const std::string& str);
	std::string Unprotect(const std::string& str);
}