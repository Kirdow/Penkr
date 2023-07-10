#include "pch.hpp"
#include "csvpass.hpp"

namespace Penkr
{
	// Util function to replace all occurrences of a string in a string with another string.
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

	// First open the CSV file and create the columns.
	CsvPass::CsvPass(const std::filesystem::path& filename)
		: m_FileName(filename), m_Index(0)
	{
		if (std::filesystem::exists(filename)) std::filesystem::remove(filename);

		m_Stream.open(filename.string(), std::ios::out | std::ios::binary);
		m_Stream << "index,url,username,password\n";
	}

	// Util function to encode commas with a URL encoded variant.
	static std::string Encode(const std::string& str)
	{
		std::string result = str;
		strreplace(result, ",", "%2C");
		return result;
	}

	// Save one row onto the CSV file.
	void CsvPass::Next(const std::string& url, const std::string& username, const std::string& pass)
	{
		m_Stream << m_Index << "," << Encode(url) << "," << username << "," << pass << "\n";
		m_Index++;
	}

	// Accept one single column item, and when we have 3, save them as a row.
	CsvPass& operator<<(CsvPass& csv, const std::string& value)
	{
		csv.m_Queue.push_back(value);
		while (csv.m_Queue.size() >= 3)
		{
			csv.Next(csv.m_Queue.at(0), csv.m_Queue.at(1), csv.m_Queue.at(2));
			csv.m_Queue.erase(csv.m_Queue.begin(), csv.m_Queue.begin() + 2);
		}
		return csv;
	}
}