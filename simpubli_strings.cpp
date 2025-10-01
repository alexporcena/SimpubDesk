#include "simpubli_strings.h"
#include <algorithm>
#include <iostream>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
#include <unicode/translit.h>

std::string SimpubliString::AnsiToUtf8(const std::string& ansiString, const char* sourceCharset) {
    UErrorCode status = U_ZERO_ERROR;
    UConverter* conv = ucnv_open(sourceCharset, &status);
    if (U_FAILURE(status)) {
        
        return "";
    }

    int32_t utf8Len = ucnv_toUChars(conv, nullptr, 0, ansiString.c_str(), ansiString.length(), &status);
    if (status == U_BUFFER_OVERFLOW_ERROR) {
        status = U_ZERO_ERROR;
    }
    else if (U_FAILURE(status)) {
        ucnv_close(conv);
        return "";
    }

    std::vector<UChar> ucharBuffer(utf8Len + 1);
    ucnv_toUChars(conv, ucharBuffer.data(), ucharBuffer.size(), ansiString.c_str(), ansiString.length(), &status);
    if (U_FAILURE(status)) {
        ucnv_close(conv);
        return "";
    }

    UConverter* utf8Conv = ucnv_open("UTF-8", &status);
    if (U_FAILURE(status)) {
        ucnv_close(conv);
        return "";
    }

    int32_t outputLen = ucnv_fromUChars(utf8Conv, nullptr, 0, ucharBuffer.data(), utf8Len, &status);
    if (status == U_BUFFER_OVERFLOW_ERROR) {
        status = U_ZERO_ERROR;
    }
    else if (U_FAILURE(status)) {
        ucnv_close(conv);
        ucnv_close(utf8Conv);
        return "";
    }

    std::string utf8String(outputLen, '\0');
    ucnv_fromUChars(utf8Conv, &utf8String[0], outputLen + 1, ucharBuffer.data(), utf8Len, &status);
    if (U_FAILURE(status)) {
        ucnv_close(conv);
        ucnv_close(utf8Conv);
        return "";
    }

    ucnv_close(conv);
    ucnv_close(utf8Conv);
    return utf8String;
}

//Sem gambiarra
std::string SimpubliString::RemoveDiacritcs(const std::string& str)
{
	icu::UnicodeString myUnicodeString = icu::UnicodeString::fromUTF8(str);

	UErrorCode status = U_ZERO_ERROR;

	icu::Transliterator* tr = icu::Transliterator::createInstance(
		"Any-Latin; Latin-ASCII", UTRANS_FORWARD, status);
	if (U_FAILURE(status)) {
		return "";
	}

	tr->transliterate(myUnicodeString);

	std::string finalStdString;
	myUnicodeString.toUTF8String(finalStdString);

	delete tr;

	return finalStdString;
}

std::vector<std::string> SimpubliString::Split(const std::string& str, char delimiter)
{
	std::vector<std::string> res;
	std::stringstream str_stream(str);
	std::string seg_string;

	while (std::getline(str_stream, seg_string, delimiter)) {
		res.push_back(seg_string);
	}

	return res;
}

void SimpubliString::ReplaceAll(std::string& in_str, std::string to_search, std::string replace_str)
{
	size_t pos = in_str.find(to_search);

	while (pos != std::string::npos)
	{
		in_str.replace(pos, to_search.size(), replace_str);
		pos = in_str.find(to_search, pos + replace_str.size());
	}
}

std::string SimpubliString::ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	return str;
}

std::string SimpubliString::Encrypt(const std::string& str, int key) {
    std::string res = "";
    
    for (char c : str) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            res += static_cast<char>((c - base + key) % 26 + base);
        }
        else {
            res += c;
        }
    }

    return res;
}

std::string SimpubliString::Decrypt(const std::string& str, int key) {
    return Encrypt(str, 26 - key);
}