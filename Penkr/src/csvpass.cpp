#include "pch.hpp"
#include "csvpass.hpp"

namespace Penkr
{
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

	CsvPass::CsvPass(const std::filesystem::path& filename)
		: m_FileName(filename), m_Index(0)
	{
		if (std::filesystem::exists(filename)) std::filesystem::remove(filename);

		m_Stream.open(filename.string(), std::ios::out | std::ios::binary);
		m_Stream << "index,url,username,password\n";
	}

	static std::string Encode(const std::string& str)
	{
		std::string result = str;
		strreplace(result, ",", "%2C");
		return result;
	}

	void CsvPass::Next(const std::string& url, const std::string& username, const std::string& pass)
	{
		m_Stream << m_Index << "," << Encode(url) << "," << username << "," << pass << "\n";
		m_Index++;
	}
}