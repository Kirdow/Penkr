#pragma once

namespace Penkr
{
	class CsvPass
	{
	public:
		CsvPass(const std::filesystem::path& filename);

		bool Good() const { return m_Stream.is_open(); }
		const std::filesystem::path& GetFileName() const { return m_FileName; }

		void Next(const std::string& url, const std::string& userName, const std::string& pass);
	private:
		std::filesystem::path m_FileName;
		std::ofstream m_Stream;
		int m_Index;
	};
}