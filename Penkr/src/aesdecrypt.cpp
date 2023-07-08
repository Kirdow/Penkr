#include "pch.hpp"
#include "aesdecrypt.hpp"

#include <Windows.h>

#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "aes.h"
#include "gcm.h"
#include "filters.h"
#include "base64.h"

#ifndef KTN_DIST
#define AES_ASSERT(check, msg) { if (!(check)) { std::cout << "Assertion Failed: " << msg << "\n"; __debugbreak(); }}
#else
#define AES_ASSERT(...)
#endif


namespace Penkr::AES
{
	std::string DecryptAES(const std::string& cipher, const std::string& key)
	{
		if (cipher.empty()) return "";

		std::string initvec(cipher.begin() + 3, cipher.begin() + 15);
		std::string pass(cipher.begin() + 15, cipher.end() - 4);

		CryptoPP::SecByteBlock sbbKey((CryptoPP::byte*)key.c_str(), key.size());
		CryptoPP::byte* iv = (CryptoPP::byte*)initvec.c_str();

		std::string decrypted;
		decrypted.resize(0);
		try
		{
			CryptoPP::GCM<CryptoPP::AES>::Decryption aes;
			aes.SetKeyWithIV(sbbKey, sbbKey.size(), iv, initvec.size());

			CryptoPP::AuthenticatedDecryptionFilter df(aes, new CryptoPP::StringSink(decrypted), 16, 12);

			CryptoPP::StringSource ss(pass, true, new CryptoPP::Redirector(df));

			if (true != df.GetLastResult())
			{
				std::cerr << "Failed to decrypt!\n";
			}
		}
		catch (CryptoPP::Exception& e)
		{
			std::cerr << e.what() << "\n";
		}

		return decrypted;
	}

	std::string Base64Decode(const std::string& str)
	{
		std::string decoded;
		try
		{
			CryptoPP::Base64Decoder decoder;
			decoder.Put((CryptoPP::byte*)str.data(), str.size());
			decoder.MessageEnd();

			CryptoPP::word64 size = decoder.MaxRetrievable();
			if (size && size <= SIZE_MAX)
			{
				decoded.resize(size);
				decoder.Get((CryptoPP::byte*)&decoded[0], decoded.size());
			}
		}
		catch (CryptoPP::Exception& e)
		{
			std::cerr << e.what() << "\n";
		}

		return decoded;
	}

	std::string Unprotect(const std::string& str)
	{
		DATA_BLOB cipher = { (DWORD)str.size(), (BYTE*)str.data() };
		DATA_BLOB plain;
		if (!CryptUnprotectData(&cipher, 0, 0, 0, 0, 0, &plain))
		{
			std::cerr << "ERROR: CryptUnprotectData() failed.\n";
		}

		return std::string(&plain.pbData[0], &plain.pbData[plain.cbData]);
	}
}