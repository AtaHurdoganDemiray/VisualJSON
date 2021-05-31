// MilPathStringFunctions.h

#ifndef __MilPathStringFunctions_h__
#define __MilPathStringFunctions_h__

#define MAX_CHR_IN_ONE_LINE	16384//2048

class MilPathStringFunctions
{
public:
	static MilString MilPathStringFunctions::TrimExtension(const MilString & filename);
	static MilString MilPathStringFunctions::GetDirectoryName(MilString filepath);
	static MilString MilPathStringFunctions::GetFileExtension(MilString filename);
	static MilString MilPathStringFunctions::GetJustFileName(const MilString & fullfilepath);
	static bool MilPathStringFunctions::DetectPlaces(const MilString &str, int bi, int ei, std::map<char, std::vector<int>>&charPlaceIndexMap);
	static std::vector<MilString> MilPathStringFunctions::ParseSolidCncCommandLine();
	static MilString MilPathStringFunctions::GetVJSONExeFolder();
	static MilString MilPathStringFunctions::GetSubSet(const MilString &str, int bi, int ei);

	static const char dirDel;
	static const char dirDelRev;

	//std::vector<char> MilPathStringFunctions::m_unallowedChars = { '*', '.', '"', '/', '\\', '[', ']', ':', ';', '|', ',', '<', '>' };
	//std::map<MilString, MilPathStringFunctions::PathExistData> PathStringFunctions::m_pathExistData;
	static char * MilPathStringFunctions::MIL_LINE_READER;
	static size_t MIL_LINE_READERSIZE;
};

#endif