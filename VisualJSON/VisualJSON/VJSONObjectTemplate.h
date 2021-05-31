// VJSONObjectTemplate.h

#ifndef __VJSONObjectTemplate_h__
#define __VJSONObjectTemplate_h__

#include "jsmn.h"
#include "VJSONElement.h"

class VJSONObject;

class VJSONObjectTemplate
{
public:
	VJSONObjectTemplate();
	~VJSONObjectTemplate();
	void ConstructAndParse(VJSONObject *templateRootJSONObject , const MilString &templateName, const MilString &fileName , MilString *mystr);// mystr is allocated and read from file
	void Construct(VJSONObject *templateRootJSONObject, const MilString &templateName); // just construct no string to parse.
	std::pair<VJSONElement *,VJSONElement *> ReconstructStr(VJSONElement *oldParent , VJSONElement *oldPrevBrother);// This is done for changing JSON string for a single element in template and re-making entire elements from new JSON string. Our changed elements parents stay with same token index. OldPrev is NULL if changed element is first child of parent. New parent and new prev brother is returned. Parent should not be NULL but new brother may be NULL
	std::pair<VJSONElement *,VJSONElement *> ReconstructStr(MilString *newStr,VJSONElement *oldParent , VJSONElement *oldPrevBrother);
	VJSONObject * GetTemplateRootJSONObject();
	const MilString & GetTemplateName()const;
	const MilString & GetFileName()const;
	jsmntok_t * GetAllTokens();
	jsmn_parser & GetParser();
	jsmnerr_t GetStatusParse()const;
	MilString * GetStr();
	int GetNumTokens()const;
	MilString GetStringFromJSONToken(jsmntok_t &curToken, const char * mystr);
	vector<VJSONElement *> & GetAllElements();
	const vector<VJSONElement *> & GetAllElements()const;
	//void AddChild(VJSONElement *child);
	const MilString & GetImportResult()const;
	void EnlargeCapacityIf(int nextTokeni);
	MilString GetElementJSONString(int elementi)const;

	class CoreData
	{
	public:
		CoreData();
		~CoreData();
		MilString m_templateName;
		MilString m_fileName;
		jsmntok_t * m_allTokens;
		jsmn_parser m_parser;
		jsmnerr_t m_statusParse;
		MilString * m_str;
		int m_numTokens;
		int m_capacityTokens;
		vector<VJSONElement *> m_allElements;
		MilString m_importResult;
		map<MilString, int> m_mapNameCount;
		VJSONObject *m_templateRootJSONObject;
	};
protected:
	CoreData *m_cd;
	std::list<CoreData *> m_history;
	int m_historyMaxLimit;
	void AddToHistory(CoreData *cd);
	int GetKeyNameTokeniIf(int tokeni)const;
	void Parse();
public:
	//int GetNameCountSoFar(const MilString &name);
	static map<MilString, int> m_mapNameCountGlobal;
	static int GetNameCountSoFarGlobal(const MilString &name);
	void SetTemplateName(const MilString &templateName);
	void SetFileName(const MilString &fileName);
	void InsertRawJsonStringAsFirstChild(VJSONElement *parent, const MilString &toInsert);
	void InsertRawJsonStringAsLastChild(VJSONElement *parent, const MilString &toInsert);
	void InsertRawJsonStringAsNextSibling(VJSONElement *brother, const MilString &toInsert);
	void InsertRawJsonStringAsPrevSibling(VJSONElement *brother, const MilString &toInsert);
	void IncrementNumTokens();
	bool IsElementKeyNameStrDuplicate(VJSONElement *childElement , const MilString &newKeyName)const;
	void DeleteElement(VJSONElement *selElem);
	std::pair<VJSONElement *,VJSONElement *> DoInsertStringAndReconstruct(const MilString &name, const MilString &toInsert, VJSONElement::ElType valType, VJSONElement *selElem, int insertLocationOpt);
	static MilString GetRawJsonString(const MilString &keyName, const MilString &defaultValue, VJSONElement::ElType parentElType, VJSONElement::ElType myElType, bool addCommaAtStart, bool addCommaAtEnd);
	std::list<VJSONObject *> SearchObjectsByName(const MilString &keyName)const;
};

#endif