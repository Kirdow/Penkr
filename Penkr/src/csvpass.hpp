#pragma once

namespace Penkr
{
	// Used to easily save URL, Username and Password columns as a CSV file.
	class CsvPass
	{
	public:
		CsvPass(const std::filesystem::path& filename);

		bool Good() const { return m_Stream.is_open(); }
		const std::filesystem::path& GetFileName() const { return m_FileName; }

		void Next(const std::string& url, const std::string& userName, const std::string& pass);

		friend CsvPass& operator<<(CsvPass& csv, const std::string& value);
	private:
		std::filesystem::path m_FileName;
		std::ofstream m_Stream;

		std::vector<std::string> m_Queue;
		int m_Index;
	};

	// Used to pass URL, Username and Password to CsvPass one at a time.
	CsvPass& operator<<(CsvPass& csv, const std::string& value);
}