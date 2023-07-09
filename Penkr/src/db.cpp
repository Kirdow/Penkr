#include "pch.hpp"
#include "db.hpp"
#include "aesdecrypt.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

namespace Penkr
{
	PenkrDB::PenkrDB(const std::filesystem::path& filepath, const std::string& key)
		: m_Values(nullptr), m_Size(0)
	{
		SQLite::Database db(filepath.string());

		std::vector<PenkrValue> data;

		try
		{
			SQLite::Statement query(db, "SELECT action_url, username_value, password_value FROM logins");

			while (query.executeStep())
			{
				const char* url = query.getColumn(0);
				const char* username = query.getColumn(1);
				const char* cipher = query.getColumn(2);

				std::string password = AES::DecryptAES(cipher, key);

				data.emplace_back(url, username, password);
			}
		}
		catch (std::exception& e)
		{
			std::cout << "SQL Error: " << e.what() << "\n";
		}

		m_Size = data.size();
		m_Values = new PenkrValue[m_Size];
		PenkrValue* ptr = m_Values;
		for (const auto& value : data)
		{
			*ptr = value;
			++ptr;
		}
	}

	PenkrDB::~PenkrDB()
	{
		if (m_Values)
		{
			delete[] m_Values;
			m_Values = nullptr;
			m_Size = 0;
		}
	}
}