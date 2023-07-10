#pragma once

namespace Penkr
{
	// Row data when reading from PenkrDB.
	struct PenkrValue
	{
		std::string Url;
		std::string Username;
		std::string Password;

		PenkrValue() = default;
		PenkrValue(const std::string& url, const std::string& username, const std::string& password)
			: Url(url), Username(username), Password(password) {}

		operator bool() const { return !Password.empty(); }
	};

	// Iterator for PenkrDB to support for each loops.
	class PenkrIterator
	{
	public:
		using ValueType = PenkrValue;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;
	public:
		PenkrIterator(PointerType ptr)
			: m_Ptr(ptr) {}

		PenkrIterator& operator++()
		{
			m_Ptr++;
			return *this;
		}

		PenkrIterator operator++(int)
		{
			PenkrIterator iterator = *this;
			++(*this);
			return iterator;
		}

		PenkrIterator& operator--()
		{
			m_Ptr--;
			return *this;
		}

		PenkrIterator operator--(int)
		{
			PenkrIterator iterator = *this;
			--(*this);
			return iterator;
		}

		ReferenceType operator[](int index)
		{
			return *(m_Ptr + index);
		}

		PointerType operator->()
		{
			return m_Ptr;
		}

		ReferenceType operator*()
		{
			return *m_Ptr;
		}

		bool operator==(const PenkrIterator& other) const
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const PenkrIterator& other) const
		{
			return !(*this == other);
		}
	private:
		PointerType m_Ptr;
	};

	// Database wrapper for loading user passwords.
	class PenkrDB
	{
	public:
		using ValueType = PenkrValue;
		using Iterator = PenkrIterator;
	public:
		PenkrDB(const std::filesystem::path& filepath, const std::string& key);
		~PenkrDB();

		PenkrIterator begin() { return m_Values; }
		PenkrIterator end() { return m_Values + m_Size; }
	private:
		PenkrValue* m_Values;
		size_t m_Size;
	};
}