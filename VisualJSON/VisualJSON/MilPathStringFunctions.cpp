// MilPathStringFunctions.cpp

#include "stdafx.h"
#include "MilPathStringFunctions.h"

const char MilPathStringFunctions::dirDel = '\\';
const char MilPathStringFunctions::dirDelRev = '\\';

//std::vector<char> MilPathStringFunctions::m_unallowedChars = { '*', '.', '"', '/', '\\', '[', ']', ':', ';', '|', ',', '<', '>' };
//std::map<MilString, MilPathStringFunctions::PathExistData> PathStringFunctions::m_pathExistData;
char * MilPathStringFunctions::MIL_LINE_READER = new char[MAX_CHR_IN_ONE_LINE];
size_t MilPathStringFunctions::MIL_LINE_READERSIZE = MAX_CHR_IN_ONE_LINE;

MilString MilPathStringFunctions::TrimExtension(const MilString & filename)
{
	if (filename == "")
		throw exception("MilPathStringFunction::TrimExtension exception , filename is empty");
	if (filename.size() >= MIL_LINE_READERSIZE)
		throw exception("MilPathStringFunctions::TrimExtension exception , filedirectoryname length too long");
	const char * mydata = filename.data();
	char * result = MIL_LINE_READER;
	for (unsigned int i = 0; i < filename.length(); i++)
	{
		if (mydata[i] == '.')
		{
			result[i] = '\0';
			break;
		}
		result[i] = mydata[i];
		result[i + 1] = '\0';
	}

	MilString resultstring(result);
	return resultstring;
}

MilString MilPathStringFunctions::GetDirectoryName(MilString filepath)
{
	if (filepath == "")
		throw exception("MilPathStringFunctions::GetDirectoryName exception filepath is empty");
	MilString s1 = filepath.substr(0, filepath.rfind(dirDel));//file.length() - file.rfind(dirDel) + 2 );
	MilString s2 = filepath.substr(0, filepath.rfind(dirDelRev));//file.length() - file.rfind(dirDel) + 2 );
	if (s1.length() < s2.length())
		return s1;
	else
		return s2;
}

MilString MilPathStringFunctions::GetFileExtension(MilString filename)
{
	if (filename == "")
		throw exception("MilPathStringFunction::GetFileExtension exception , filename is empty");
	char myext[20];
	myext[0] = 0;

	unsigned int i = 0;
	for (i; i < filename.length(); i++)
	{
		char temp = filename[i];
		if (temp == '.')
			break;
	}

	if (i < filename.length())
	{
		unsigned int offset = i + 1;
		for (unsigned int j = 0; j < (filename.length() - offset); j++)
		{
			myext[j] = filename[j + offset];
		}
		// VC8 change
		myext[/*j*/(filename.length() - offset)] = 0;
	}

	MilString retval(myext);

	return retval;
}

MilString MilPathStringFunctions::GetJustFileName(const MilString & fullfilepath)
{
	if (fullfilepath == "")
		throw exception("MilPathStringFunction::GetJustFileName exception , path string is empty");
	MilString::const_iterator myit = fullfilepath.end();

	while (--myit != fullfilepath.begin())
	{
		if (*myit == '//' || *myit == '\\')
			break;
	};

	MilString myresult;

	if (*myit == dirDel || *myit == dirDelRev)
	{
		while (++myit != fullfilepath.end())
		{
			myresult += (*myit);
		};
	}
	else
	{
		// no seperators
		myresult = fullfilepath;
	}

	return myresult;
}
bool MilPathStringFunctions::DetectPlaces(const MilString &str, int bi, int ei, std::map<char, std::vector<int>>&charPlaceIndexMap)
{
	try
	{
		for (int i = bi; i <= ei; ++i)
		{
			char ci = str.at(i);
			std::map<char, std::vector<int>>::iterator fi = charPlaceIndexMap.find(ci);
			if (fi != charPlaceIndexMap.end())
			{
				fi->second.push_back(i);
			}
		}
		return true;
	}
	catch (std::exception &e)
	{
		throw e;
	}
	catch (...)
	{
		throw exception("MilPathStringFunctions::DetectPlaces");
	}
}
std::vector<MilString> MilPathStringFunctions::ParseVJSONCommandLine()
{
	std::vector<MilString> parsedsolidcnccmdline;
	MilString solidcnccmdline = GetCommandLine();
	//TypeConverters::SplitString (solidcnccmdline , ' ' , parsedsolidcnccmdline);
	std::map<char, std::vector<int>> cmp;
	cmp.insert(std::pair<char, std::vector<int>>('"', std::vector<int>()));
	DetectPlaces(solidcnccmdline, 0, (int)solidcnccmdline.length() - 1, cmp);
	if (cmp.empty())
		throw exception("MilPathStringFunctions::ParseSolidCncCommandLin. cmp empty");
	std::vector<int> &ivec = cmp.begin()->second;
	int mysize = (int)ivec.size();
	if (mysize % 2 != 0)
		throw exception("MilPathStringFunctions::ParseSolidCncCommandLin. mysize not even");
	for (int i = 0; i<mysize; i += 2)
	{
		parsedsolidcnccmdline.push_back(GetSubSet(solidcnccmdline, ivec.at(i), ivec.at(i + 1)));
	}
	return parsedsolidcnccmdline;
}

MilString MilPathStringFunctions::GetVJSONExeFolder()
{
	std::vector<MilString> parsedsolidcnccmdline = ParseVJSONCommandLine();
	if (parsedsolidcnccmdline.size())
	{
		MilString fname = parsedsolidcnccmdline.front();
		fname = fname.substr(1, fname.rfind('\\'));
		return fname;
	}
	else
		throw exception("MilPathStringFunctions::GetVJSONExeFolderOld. size is 0");
}

MilString MilPathStringFunctions::GetSubSet(const MilString &str, int bi, int ei)
{
	try
	{
		MilString substr;
		for (int i = bi; i <= ei; ++i)
		{
			substr += str.at(i);
		}
		return substr;
	}
	catch (exception &e)
	{
		//MilString dummystr;
		//LogFileWriter::WriteExceptionLog(e, IDS_TypGetVpE);
		//return dummystr;
		throw (e);
	}
	catch (...)
	{
		//MilString dummystr;
		//LogFileWriter::WriteUnknownExceptionLog(IDS_UnkExc1, IDS_TypUnkE);
		//return dummystr;
		throw(exception("MilPathStringFunctions::GetSubSet unknown exception"));
	}
}
