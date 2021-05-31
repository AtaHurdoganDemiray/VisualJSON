// MilString.h

#ifndef __MilString_h__
#define __MilString_h__

#ifdef _UNICODE
typedef std::wstring MilString;
typedef std::wifstream MilIFStream;
typedef std::wofstream MilOFStream;
typedef std::wstringstream MilStringStream;
#else
typedef std::string MilString;
typedef std::ifstream MilIFStream;
typedef std::ofstream MilOFStream;
typedef std::stringstream MilStringStream;
#endif //  VJSON_UNICODE


#endif