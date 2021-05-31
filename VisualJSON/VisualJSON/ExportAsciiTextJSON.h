// ExportAsciiTextJSON.h

#ifndef __ExportAsciiTextJSON_h__
#define __ExportAsciiTextJSON_h__

class VJSONObjectTemplate;
class VJSONElement;
class VJSONString;

class ExportAsciiTextJSON
{
public:
	ExportAsciiTextJSON(VJSONObjectTemplate *toExport, const MilString &fullFilePath);
	ExportAsciiTextJSON(VJSONObjectTemplate *toExport,MilString *exportedStr);// exportedStr must not be null , user is responsible to delete it 
	MilString & GetStr();
private:
	VJSONElement * AddToStrRecursiveKeyValPair(VJSONElement *parent, VJSONString *keyToAdd, VJSONElement *valToAdd, MilString &str);
	VJSONElement * AddToStrRecursiveNoKeyOnlyVal(VJSONElement *parent, VJSONElement *valToAdd, MilString &str);
	VJSONElement * AddToStrRecursive(VJSONElement *parent, VJSONElement *toAdd, MilString &str);
	//MilString m_str;
	MilString m_fullFilePath;
	int m_tabCounter;
	char m_tabChar;
	bool m_isPrevBasicTypeOrValStr;
	bool ShouldIPrintComaBefore(VJSONElement *parent , VJSONElement *toPrint);
};

#endif