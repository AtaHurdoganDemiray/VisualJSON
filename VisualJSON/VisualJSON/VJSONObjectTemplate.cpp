// VJSONObjectTemplate.cpp

#include "stdafx.h"
#include  "VJSONObjectTemplate.h"
#include "VJSONObject.h"
#include "VJSONArray.h"
#include "VJSONSingleKeyVal.h"
#include "VJSONString.h"
#include "ExportAsciiTextJSON.h"

VJSONObjectTemplate::CoreData::CoreData()
{
	m_numTokens = 0;
	m_capacityTokens = 100;
	m_str = NULL;
	m_statusParse = JSMN_ERROR_NOMEM;
	m_templateRootJSONObject = NULL;
	m_allTokens = new jsmntok_t[m_capacityTokens];
}
VJSONObjectTemplate::CoreData::~CoreData()
{
	if (m_allTokens != NULL)
		delete[]m_allTokens;
	delete m_str;
	std::vector<VJSONElement *>::iterator eli = m_allElements.begin();
	std::vector<VJSONElement *>::iterator ele = m_allElements.end();
	for (eli; eli != ele; ++eli)
	{
		delete (*eli);
	}
	if (m_templateRootJSONObject != NULL)
		delete m_templateRootJSONObject;
}
VJSONObjectTemplate::VJSONObjectTemplate()
{
	m_historyMaxLimit = 5;
	m_cd = new CoreData();
}
VJSONObjectTemplate::~VJSONObjectTemplate()
{
	if (m_cd != NULL)
		delete m_cd;
	std::list<CoreData *>::iterator ci = m_history.begin();
	std::list<CoreData *>::iterator ce = m_history.end();
	for (ci; ci != ce; ++ci)
		delete *ci;
	m_history.clear();
}
void VJSONObjectTemplate::ConstructAndParse(VJSONObject *templateRootJSONObject , const MilString &templateName, const MilString &fileName , MilString *str)// str comes from a file
{
	m_cd->m_templateRootJSONObject = templateRootJSONObject;
	m_cd->m_templateName = templateName;
	m_cd->m_fileName = fileName;
	m_cd->m_str = str;
	//m_capacityTokens = m_str->length();
	Parse();
}
void VJSONObjectTemplate::Construct(VJSONObject *templateRootJSONObject, const MilString &templateName) // just construct. No string to parse.
{
	m_cd->m_templateRootJSONObject = templateRootJSONObject;
	m_cd->m_templateName = templateName;
	// m_fileName = "";
	m_cd->m_str = new MilString();
}
std::pair<VJSONElement *, VJSONElement *> VJSONObjectTemplate::ReconstructStr(VJSONElement *oldParent , VJSONElement *oldPrevBrother)
{
	//m_history.push_back(m_cd);
	AddToHistory(m_cd);
	MilString *newStr = new MilString(*m_cd->m_str);
	//ExportAsciiTextJSON *myexport = new ExportAsciiTextJSON(this,newStr);
	CoreData *emptyCoreData = new CoreData();
	emptyCoreData->m_str = newStr;
	emptyCoreData->m_templateRootJSONObject = new VJSONObject(this);//m_templateRootJSONObject;
	emptyCoreData->m_fileName = m_cd->m_fileName;
	emptyCoreData->m_templateName = m_cd->m_templateName;
	//emptyCoreData.m_allElements = m_cd.m_allElements;
		
	m_cd = emptyCoreData;
	Parse();
	//VJSONObjectTemplate *newTemp = new VJSONObjectTemplate();
	//VJSONObject * templateRootJSONObj = new VJSONObject(newTemp);
	//MilString templateName = GetTemplateName();
	//MilString fileName(GetFileName());
	//newTemp->ConstructAndParse(templateRootJSONObj, templateName, fileName, &myexport->GetStr());
	//m_curObjTemplate = newTemp;
	//m_allObjectTemplates.push_back(m_curObjTemplate);

	VJSONElement * newPar = m_cd->m_allElements.at(oldParent->GetTokeni());
	VJSONElement * newPrevBro = oldPrevBrother == NULL ? NULL : m_cd->m_allElements.at(oldPrevBrother->GetTokeni());
	std::pair<VJSONElement *, VJSONElement *> newParents(newPar,newPrevBro);
	return newParents;
}
std::pair<VJSONElement *, VJSONElement *> VJSONObjectTemplate::ReconstructStr(MilString *newStr,VJSONElement *oldParent , VJSONElement *oldPrevBrother)
{
	//m_history.push_back(m_cd);
	AddToHistory(m_cd);
	//MilString *newStr = new MilString(*m_cd->m_str);
	//ExportAsciiTextJSON *myexport = new ExportAsciiTextJSON(this,newStr);
	CoreData *emptyCoreData = new CoreData();
	emptyCoreData->m_str = newStr;
	emptyCoreData->m_templateRootJSONObject = new VJSONObject(this);//m_templateRootJSONObject;
	emptyCoreData->m_fileName = m_cd->m_fileName;
	emptyCoreData->m_templateName = m_cd->m_templateName;
	//emptyCoreData.m_allElements = m_cd.m_allElements;
	m_cd = emptyCoreData;
	Parse();
	//VJSONObjectTemplate *newTemp = new VJSONObjectTemplate();
	//VJSONObject * templateRootJSONObj = new VJSONObject(newTemp);
	//MilString templateName = GetTemplateName();
	//MilString fileName(GetFileName());
	//newTemp->ConstructAndParse(templateRootJSONObj, templateName, fileName, &myexport->GetStr());
	//m_curObjTemplate = newTemp;
	//m_allObjectTemplates.push_back(m_curObjTemplate);
	VJSONElement * newPar = m_cd->m_allElements.at(oldParent->GetTokeni());
	VJSONElement * newPrevBro = oldPrevBrother == NULL ? NULL : m_cd->m_allElements.at(oldPrevBrother->GetTokeni());
	std::pair<VJSONElement *, VJSONElement *> newParents(newPar, newPrevBro);
	return newParents;
}
VJSONObject * VJSONObjectTemplate::GetTemplateRootJSONObject()
{
	return m_cd->m_templateRootJSONObject;
}
const MilString & VJSONObjectTemplate::GetTemplateName()const
{
	return m_cd->m_templateName;
}
const MilString & VJSONObjectTemplate::GetFileName()const
{
	return m_cd->m_fileName;
}
jsmntok_t * VJSONObjectTemplate::GetAllTokens()
{
	return m_cd->m_allTokens;
}
jsmn_parser & VJSONObjectTemplate::GetParser()
{
	return m_cd->m_parser;
}
jsmnerr_t VJSONObjectTemplate::GetStatusParse()const
{
	return m_cd->m_statusParse;
}
MilString * VJSONObjectTemplate::GetStr()
{
	return m_cd->m_str;
}
int VJSONObjectTemplate::GetNumTokens()const
{
	return m_cd->m_numTokens;
}
MilString VJSONObjectTemplate::GetStringFromJSONToken(jsmntok_t &curToken, const char *  mystr)
{
	MilString temp;
	for (int j = curToken.start; j <= curToken.end; ++j)
	{
		temp += mystr[j];
	}
	return temp;
}
void VJSONObjectTemplate::Parse()
{
	try
	{
		//m_cd->m_capacityTokens = (int)m_cd->m_str->length();
		//m_cd->m_allTokens = new jsmntok_t[m_cd->m_capacityTokens];
		if (m_cd == NULL)
			throw exception("VJSONObjectTemplate::Parse exception , coredata m_cd is NULL");
		int capacityNeededMax = (int)m_cd->m_str->length();
		EnlargeCapacityIf(capacityNeededMax);

		jsmn_init(&m_cd->m_parser);
		m_cd->m_statusParse = jsmn_parse(&m_cd->m_parser, m_cd->m_str->c_str(), m_cd->m_allTokens, m_cd->m_capacityTokens);
		MilString key;
		if (m_cd->m_statusParse == JSMN_SUCCESS)
		{
			// Make root object for the file 
			//m_obj = new VJSONObject(this);
			//m_parent = m_obj;
			//vector<VJSONElement *> allelements;
			m_cd->m_numTokens = m_cd->m_parser.toknext;
			if (m_cd->m_numTokens >= m_cd->m_capacityTokens)
				throw std::exception(_T("VJSONObjectTemplate::Parse m_numTokens >= m_capacity"));
			//int lastObjectIndex = -1;
			//int lastArrayIndex = -2;
			//int lastStringIndex = -3;
			int curNameTokeni = -1;
			for (int i = 0; i < m_cd->m_numTokens; ++i)
			{
				jsmntok_t &t = m_cd->m_allTokens[i];
				VJSONElement *curElement = NULL;
				VJSONElement * strParent = NULL;
				bool isKeyNameStr(false);
				switch (t.type)
				{
				case JSMN_ARRAY:
					//curNameTokeni = -1;
					//if (lastStringIndex > lastObjectIndex && lastStringIndex > lastArrayIndex && lastObjectIndex > lastArrayIndex)
					//{
					//	if ((lastStringIndex-lastObjectIndex)%2 == 1 && lastStringIndex == i -1)
					//		curNameTokeni = lastStringIndex;
					//}
					curNameTokeni = GetKeyNameTokeniIf(i);
					curElement = new VJSONArray(curNameTokeni, this, i);
					//lastArrayIndex = i;
					break;
				case JSMN_STRING:
					//curNameTokeni = -1;
					//isKeyNameStr = false;
					//if (lastObjectIndex > lastArrayIndex)
					//{
					//	if ((i - lastObjectIndex) % 2 == 0)
					//	{
					//		curNameTokeni = i - 1;
					//	}
					//	else
					//	{
					//		isKeyNameStr = true;
					//	}
					//}
					curNameTokeni = GetKeyNameTokeniIf(i);
					curElement = new VJSONString(curNameTokeni, this, i);
					//  for debug 
					key = curElement->GetJSONString();
					// for debug
					strParent = curElement->GetParent();
					if (strParent != NULL && strParent->GetElType() != VJSONElement::ElTypeArray)
					{
						if (curNameTokeni == -1)
							isKeyNameStr = true;
						else
							isKeyNameStr = false;
					}
					else
						isKeyNameStr = false;
					((VJSONString *)curElement)->SetIsKeyNameStr(isKeyNameStr);
					//lastStringIndex = i;
					break;
				case JSMN_OBJECT:
					//curNameTokeni = -1;
					//if (lastStringIndex > lastObjectIndex && lastStringIndex > lastArrayIndex && lastStringIndex == (i - 1))
					//{
					//	curNameTokeni = i - 1;
					//}
					curNameTokeni = GetKeyNameTokeniIf(i);
					curElement = new VJSONObject(curNameTokeni, this, i);
					//lastObjectIndex = i;
					break;
				case JSMN_PRIMITIVE:
					//curNameTokeni = -1;
					//if (lastObjectIndex > lastArrayIndex)
					//{
					//	if ((i - lastObjectIndex) % 2 == 0)
					//	{
					//		curNameTokeni = i - 1;
					//	}
					//}
					curNameTokeni = GetKeyNameTokeniIf(i);
					curElement = new VJSONBasicVal(curNameTokeni, this, i);
					break;
				};
			}
			
			m_cd->m_importResult = _T("Successfull");
			CString typeStr(_T("info"));
			CString messageStr;
			messageStr.Format(_T("%s successfully parsed"), m_cd->m_templateName.c_str());
			PrintMessageManager::AddMessage(typeStr, messageStr);
			return;
		}
		else if (m_cd->m_statusParse == JSMN_ERROR_NOMEM)
		{
			m_cd->m_importResult = _T("Not enough tokens were provided");
		}
		else if (m_cd->m_statusParse == JSMN_ERROR_PART)
		{
			m_cd->m_importResult = _T("The string is not a full JSON packet, more bytes expected");
		}
		else if (m_cd->m_statusParse == JSMN_ERROR_INVAL)
		{
			m_cd->m_importResult = _T("Invalid character inside JSON string");
		}
		CString typeStr(_T("info"));
		CString messageStr;
		messageStr.Format(_T("%s could not be parsed , %s"), m_cd->m_templateName.c_str(), m_cd->m_importResult.c_str());
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
	catch (...)
	{
		m_cd->m_importResult = _T("Exception occured");
		CString typeStr(_T("info"));
		CString messageStr;
		messageStr.Format(_T("%s could not be imported because of exception!"), m_cd->m_fileName.c_str());
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

vector<VJSONElement *> & VJSONObjectTemplate::GetAllElements()
{
	return m_cd->m_allElements;
}
const vector<VJSONElement *> & VJSONObjectTemplate::GetAllElements()const
{
	return m_cd->m_allElements;
}
//void VJSONObjectTemplate::AddChild(VJSONElement *child)
//{
//	if (m_templateRootJSONObject != NULL)
//		m_templateRootJSONObject->AddChild(child);
//	else
//		throw exception("VJSONObjectTemplate::AddChild exception , no root constructed");
//}

const MilString & VJSONObjectTemplate::GetImportResult()const
{
	return m_cd->m_importResult;
}

void VJSONObjectTemplate::EnlargeCapacityIf(int nextTokeni)
{
	if (m_cd->m_capacityTokens <= nextTokeni)
	{
		m_cd->m_capacityTokens = nextTokeni;//*= 2;
		jsmntok_t * newAllTokens = new jsmntok_t[m_cd->m_capacityTokens];
		for (int i = 0; i < m_cd->m_numTokens; ++i)
		{
			newAllTokens[i] = m_cd->m_allTokens[i];
		}
		if (m_cd->m_allTokens != NULL)
			delete[] m_cd->m_allTokens;
		m_cd->m_allTokens = newAllTokens;
	}
}

MilString VJSONObjectTemplate::GetElementJSONString(int elementi)const
{
	const jsmntok_t &curTok = m_cd->m_allTokens[elementi];
	MilString temp;
	for (int i = curTok.start; i != curTok.end; ++i)
	{
		temp += (*m_cd->m_str)[i];
	}
	return temp;
}

//int VJSONObjectTemplate::GetNameCountSoFar(const MilString &name)
//{
//	map<MilString, int>::iterator fi = m_cd->m_mapNameCount.find(name);
//	int countSoFar = 1;
//	if (fi != m_cd->m_mapNameCount.end())
//	{
//		countSoFar = ++(fi->second);
//	}
//	else
//	{
//		m_cd->m_mapNameCount.insert(pair<MilString, int>(name, countSoFar));
//	}
//	return countSoFar;
//}

int VJSONObjectTemplate::GetKeyNameTokeniIf(int tokeni)const
{
	int nameTokeni = tokeni - 1;
	int myresult = -1;
	if (nameTokeni >= 0 && nameTokeni < m_cd->m_capacityTokens)
	{
		VJSONElement *namTokEl = m_cd->m_allElements.at(nameTokeni);
		if (namTokEl->GetElType() == VJSONElement::ElTypeString)
		{
			int strLen = m_cd->m_str->length();
			jsmntok_t &namtok = m_cd->m_allTokens[nameTokeni];
			int si = namtok.end + 1;
			char curc = m_cd->m_str->at(si);
			while (curc != '}' && curc != ']' && curc != ',' && curc != '{' && curc != '[' && curc != '\"' &&si < strLen)
			{
				if (curc == ':')
				{
					myresult = nameTokeni;
					break;
				}
				++si;
				curc = m_cd->m_str->at(si);
			}
		}
	}	
	return myresult;
}

map<MilString, int> VJSONObjectTemplate::m_mapNameCountGlobal = map<MilString, int>();
int VJSONObjectTemplate::GetNameCountSoFarGlobal(const MilString &name)
{
	map<MilString, int>::iterator fi = m_mapNameCountGlobal.find(name);
	int countSoFar = 1;
	if (fi != m_mapNameCountGlobal.end())
	{
		countSoFar = ++(fi->second);
	}
	else
	{
		m_mapNameCountGlobal.insert(pair<MilString, int>(name, countSoFar));
	}
	return countSoFar;
}

void VJSONObjectTemplate::SetTemplateName(const MilString &templateName)
{
	m_cd->m_templateName = templateName;
}

void VJSONObjectTemplate::SetFileName(const MilString &fileName)
{
	m_cd->m_fileName = fileName;
}

void VJSONObjectTemplate::AddToHistory(VJSONObjectTemplate::CoreData *cd)
{
	if (m_history.size() == m_historyMaxLimit)
	{
		CoreData *todel = m_history.front();
		m_history.pop_front();
		delete todel;
	}
	m_history.push_back(cd);
}

void VJSONObjectTemplate::InsertRawJsonStringAsFirstChild(VJSONElement *parent, const MilString &toInsert)
{
	if (parent != NULL && m_cd != NULL && m_cd->m_allTokens != NULL && (parent->GetElType() == VJSONElement::ElTypeObject  || parent->GetElType() == VJSONElement::ElTypeArray))
	{
		//VJSONElement::ElType parentType = parent->GetElType();
		int partokeni = parent->GetTokeni();
		if (partokeni == -1)
		{
			if (parent == m_cd->m_templateRootJSONObject)
			{
				VJSONElement *child = m_cd->m_templateRootJSONObject->GetFirstChild();
				if (child == NULL)
					throw exception("VJSONObjectTemplate::InsertRawJsonStringAsLastChild template root have no child object");
				InsertRawJsonStringAsPrevSibling(child, toInsert);
			}
			else
				throw exception("VJSONObjectTemplate::InsertRawJSonStringAsLastChild parent does not have token index");
		}
		else if (m_cd->m_numTokens > partokeni)
		{
			jsmntok_t partok = m_cd->m_allTokens[partokeni];
			int sti = partok.start +1;
			MilString::iterator si = m_cd->m_str->begin();
			for (int i = 0; i < sti; ++i)
				++si;
			m_cd->m_str->insert(si, toInsert.begin(), toInsert.end());
		}
	}
}
void VJSONObjectTemplate::InsertRawJsonStringAsLastChild(VJSONElement *parent, const MilString &toInsert)
{
	if (parent == NULL)
		throw exception("VJSONObjectTemplate::InsertRawJsonStringAsLastChild exception null parent");
	if (parent->GetElType() == VJSONElement::ElTypeString && ((VJSONString *)parent)->GetIsKeyNameStr())
	{
		InsertRawJsonStringAsLastChild(parent->GetNext(), toInsert);
		return;
	}
	if (m_cd != NULL && m_cd->m_allTokens != NULL && (parent->GetElType() == VJSONElement::ElTypeObject || parent->GetElType() == VJSONElement::ElTypeArray))
	{
		int partokeni = parent->GetTokeni();
		if (partokeni == -1)
		{
			//if (parent == m_cd->m_templateRootJSONObject)
			//{
			//	VJSONElement *child = m_cd->m_templateRootJSONObject->GetFirstChild();
			//	if (child != NULL)
			//	{
			//		while (child->GetNext() != NULL)
			//		{
			//			child = child->GetNext();
			//		}
			//	}
			//	if (child == NULL)
			//		throw exception("VJSONObjectTemplate::InsertRawJsonStringAsLastChild template root have no child object");
			//	InsertRawJsonStringAsNextSibling(child, toInsert);
			//}
			//else
				throw exception("VJSONObjectTemplate::InsertRawJSonStringAsLastChild parent does not have token index");
		}
		else if (m_cd->m_numTokens > partokeni)
		{
			jsmntok_t partok = m_cd->m_allTokens[partokeni];
			int sti = partok.end - 1;
			MilString::iterator si = m_cd->m_str->begin();
			for (int i = 0; i < sti; ++i)
				++si;
			m_cd->m_str->insert(si, toInsert.begin(), toInsert.end());
		}
	}
}
void VJSONObjectTemplate::InsertRawJsonStringAsNextSibling(VJSONElement *brother, const MilString &toInsert)
{
	if (brother == NULL)
		throw exception("VJSONObjectTemplate::InsertRawJsonStringAsNextSibling exception , brother is NULL");
	if (m_cd != NULL && m_cd->m_allTokens != NULL)
	{
		if (brother != this->m_cd->m_templateRootJSONObject)
		{
			VJSONElement *usedBro = brother;
			int brotokeni = brother->GetTokeni();
			if (brother->GetElType() == VJSONElement::ElTypeString && ((VJSONString *)brother)->GetIsKeyNameStr() == true)
			{
				usedBro = brother->GetNext();
				if (usedBro != NULL)
				{
					brotokeni = usedBro->GetTokeni();
				}
				else
					throw exception("VJSONObjectTemplate::InsertRawJsonStringAsNextSibling exception brother next is null even though brother is key name");
			}
			if (usedBro->GetNext() == NULL)
			{
				VJSONElement *bigParent = usedBro->GetParent();
				if (bigParent != NULL && bigParent != m_cd->m_templateRootJSONObject)
				{
					InsertRawJsonStringAsLastChild(bigParent, toInsert);
					return;
				}
				else
					throw exception("VJSONObjectTemplate::InsertRawJsonStringAsNextSibling exception , adding next tail to template root is not allowed");
				//else if (bigParent == m_cd->m_templateRootJSONObject)
				//{
				//	if (bigParent->GetFirstChild() != NULL)
				//		InsertRawJsonStringAsLastChild(bigParent->GetFirstChild(), toInsert);
				//	else
				//		throw exception("VJSONObjectTemplate::InsertRawJsonStringAsNextSibling unexpected tree form");
				//	return;
				//}
				//else
				//	throw exception("VJSONObjectTemplate::InsertRawJsonStringAsNextSibling exception , now next item and bigParent is NULL");
			}

			if (brotokeni < 0)
				throw exception("VJSONObjectTemplate::InsertRawJSonStringAsNextSibling brother does not have token index");
			VJSONElement::ElType broType = usedBro->GetElType();
			if (m_cd->m_numTokens > brotokeni)
			{
				jsmntok_t partok = m_cd->m_allTokens[brotokeni];
				int sti = partok.end;
				if (broType != VJSONElement::ElTypeObject && broType != VJSONElement::ElTypeArray)
					++sti;
				MilString::iterator si = m_cd->m_str->begin();
				for (int i = 0; i < sti; ++i)
					++si;
				m_cd->m_str->insert(si, toInsert.begin(), toInsert.end());
			}
		}
	}
}
void VJSONObjectTemplate::InsertRawJsonStringAsPrevSibling(VJSONElement *brother, const MilString &toInsert)
{
	if (brother == NULL)
		throw exception("VJSONObjectTemplate::InsertRawJsonStringAsPrevSibling exception brother is NULL");
	if (m_cd != NULL && m_cd->m_allTokens != NULL)
	{
		if (brother != this->m_cd->m_templateRootJSONObject)
		{
			VJSONElement *usedBro = brother->GetPrev();
			VJSONElement::ElType broType;
			int brotokeni;
			if (usedBro != NULL && usedBro->GetElType() == VJSONElement::ElTypeString && ((VJSONString *)usedBro)->GetIsKeyNameStr() == true)
			{
				broType = usedBro->GetElType();
				brotokeni = usedBro->GetTokeni();
			}
			else
			{
				// previous element is not the name of the brother , so stick to brother it self to insert before
				usedBro = brother;
				broType = brother->GetElType();
				brotokeni = brother->GetTokeni();
			}
			if (usedBro->GetPrev() == NULL)
			{
				VJSONElement *bigParent = usedBro->GetParent();
				if (bigParent != NULL && bigParent != m_cd->m_templateRootJSONObject)
				{
					InsertRawJsonStringAsFirstChild(bigParent, toInsert);
					return;
				}
				//else if (bigParent == m_cd->m_templateRootJSONObject)
				//{
				//	if (bigParent->GetFirstChild() != NULL)
				//		InsertRawJsonStringAsFirstChild(bigParent->GetFirstChild(), toInsert);
				//	else
				//		throw exception("VJSONObjectTemplate::InsertRawJsonStringAsPrevSibling unexpected tree form");
				//	return;
				//}
				else
				{
					throw exception("VJSONObjectTemplate::InsertRawJsonStringAsPrevSibling exception , brother element has no previous and parent seems to be template root");
				}
			}

			// first try the chance with previous element assuming it is a name for the brother

			if (brotokeni < 0)
				throw exception("VJSONObjectTemplate::InsertRawJSonStringAsPrevSibling brother does not have token index");
			if (m_cd->m_numTokens > brotokeni)
			{
				jsmntok_t partok = m_cd->m_allTokens[brotokeni];
				int sti = partok.start-1;
				if (broType != VJSONElement::ElTypeObject && broType != VJSONElement::ElTypeArray)
					--sti;
				MilString::iterator si = m_cd->m_str->begin();
				for (int i = 0; i < sti; ++i)
					++si;
				m_cd->m_str->insert(si, toInsert.begin(), toInsert.end());
			}
		}
	}
}
void VJSONObjectTemplate::IncrementNumTokens()
{
	if (m_cd != NULL)
	{
		m_cd->m_numTokens++;
	}
	else
		throw exception("VJSONObjectTemplate::IncrementNumTokens CoreData instance of template is null");
}

bool VJSONObjectTemplate::IsElementKeyNameStrDuplicate(VJSONElement *childElement, const MilString &newKeyName)const
{
	if (childElement->GetElType() == VJSONElement::ElTypeString && ((VJSONString *)childElement)->GetJSONString() == newKeyName)
		return true;
	else
		return false;
}

void VJSONObjectTemplate::DeleteElement(VJSONElement *selElem)
{
	bool removed(false);
	VJSONElement *nameStrEl(NULL);
	VJSONElement *prevEl = selElem->GetPrev();
	VJSONElement *nextEl = selElem->GetNext();
	if (selElem->GetPrev() != NULL)
	{
		if (prevEl->GetElType() == VJSONElement::ElTypeString && ((VJSONString *)prevEl)->GetIsKeyNameStr() == true)
		{
			nameStrEl = prevEl;
			prevEl = nameStrEl->GetPrev();
		}
		if (prevEl != NULL)
		{
			if (selElem->GetNext() != NULL)
			{
				prevEl->SetNext(nextEl);
				if (nextEl != NULL)
					nextEl->SetPrev(prevEl);
				removed = true;
			}
			else
			{
				prevEl->SetNext(NULL);
				removed = true;
			}
		}
	}
	if (removed == false)
	{
		// this must be first element of its parent
		VJSONElement *parentEl = selElem->GetParent();
		if (parentEl == NULL || parentEl == GetTemplateRootJSONObject())
			throw exception("CVisualJSONDlg::OnBnClickedDeleteElement exception , sel item has no parent");
		parentEl->SetFirstChild(nextEl);
		if (nextEl != NULL)
		{
			nextEl->SetPrev(NULL);
		}
		removed = true;
	}
	if (removed)
	{
		MilString *exportedStr = new MilString();
		ExportAsciiTextJSON(this, exportedStr);
		ReconstructStr(exportedStr,selElem->GetParent(),selElem->GetPrev());
	}
}
std::pair<VJSONElement *,VJSONElement *> VJSONObjectTemplate::DoInsertStringAndReconstruct(const MilString &name, const MilString &toInsert, VJSONElement::ElType valType, VJSONElement *selElem, int insertLocationOpt)
{
	VJSONElement::ElType selType = selElem->GetElType();
	//VJSONElement::ElType valType = m_dialKeyVal->GetSelectedElType();
	VJSONElement::ElType parentType = selType;
	MilString keyName(name);
	MilString defaultValue(toInsert);
	BOOL isChild = insertLocationOpt == 0 || insertLocationOpt == 2 ? TRUE : FALSE;
	std::pair<VJSONElement *, VJSONElement *> ancesters(NULL,NULL);
	if (isChild)
	{
		if (selType != VJSONElement::ElTypeArray && selType != VJSONElement::ElTypeObject)
		{
			//isChild = FALSE;
			throw exception("You should select either an object or array element to insert a child item!");
		}

		if (insertLocationOpt == 0)
		{
			bool addCommaAtStart = true;
			bool addCommaAtEnd = false;
			VJSONElement *prevEl = selElem->GetLastChild();
			if (prevEl == NULL)
				addCommaAtStart = false;
			MilString strToInsert = VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd);
			InsertRawJsonStringAsLastChild(selElem, strToInsert);
			ancesters = ReconstructStr(selElem, prevEl);
		}
		else if (insertLocationOpt == 2)
		{
			bool addCommaAtStart = false;
			bool addCommaAtEnd = true;
			if (selElem->GetFirstChild() == NULL)
				addCommaAtEnd = false;
			MilString strToInsert = VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd);
			InsertRawJsonStringAsFirstChild(selElem, strToInsert);
			ancesters = ReconstructStr(selElem, selElem->GetFirstChild());
		}
	}
	else
	{
		if (insertLocationOpt == 1)
		{
			VJSONElement *bigParent = selElem->GetParent();
			if (bigParent == NULL)
				throw exception("CVisualJSONDlg::OnBnClickedMakeKeyValue selected element is root! No next element can be added");
			parentType = bigParent->GetElType();
			bool addCommaAtStart = true;
			bool addCommaAtEnd = selElem->GetNext() != NULL ? true : false;
			MilString strToInsert = VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd);
			InsertRawJsonStringAsNextSibling(selElem, strToInsert);
			ancesters = ReconstructStr(bigParent, selElem);
		}
		else if (insertLocationOpt == 3)
		{
			VJSONElement *bigParent = selElem->GetParent();
			if (bigParent == NULL)
				throw exception("CVisualJSONDlg::OnBnClickedMakeKeyValue selected element is root! No previous element can be added");
			parentType = bigParent->GetElType();
			bool addCommaAtStart = false;
			bool addCommaAtEnd = true;
			MilString strToInsert = VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd);
			InsertRawJsonStringAsPrevSibling(selElem, strToInsert);
			ancesters = ReconstructStr(bigParent, selElem->GetPrev());
		}
	}
	if (ancesters.first == NULL)
		throw exception("VJSONObjectTemplate::DoInsertStringAndReconstruct exception , ancester parent is NULL");
	return ancesters;
}

MilString VJSONObjectTemplate::GetRawJsonString(const MilString &keyName, const MilString &defaultValue, VJSONElement::ElType parentElType, VJSONElement::ElType myElType, bool addCommaAtStart, bool addCommaAtEnd)
{
	MilStringStream mystr;
	if (parentElType == VJSONElement::ElTypeArray || parentElType == VJSONElement::ElTypeObject)
	{
		if (addCommaAtStart == true)// it seems to be inserted as last child 
		{
			mystr << ",";
		}
	}
	else
	{
		throw exception("DialogKeyValue::GetRawJsonString exception , parent is not object or array!");
	}
	if (parentElType != VJSONElement::ElTypeArray && keyName != "")
		mystr << "\"" << keyName << "\":";
	switch (myElType)
	{
	case VJSONElement::ElTypeBasic:
		// double int 
		mystr << defaultValue;
		break;
	case VJSONElement::ElTypeString:
		// string
		mystr << "\"" << defaultValue << "\"";
		break;
	case VJSONElement::ElTypeArray:
		mystr << defaultValue;
		break;
	case VJSONElement::ElTypeObject:
		mystr << defaultValue;
		break;
		//case 4:
		//
		//break;
	}
	if (addCommaAtEnd)
	{
		mystr << ",";
	}
	MilString rawJsonString(mystr.str());
	return rawJsonString;
}

std::list<VJSONObject *> VJSONObjectTemplate::SearchObjectsByName(const MilString &keyName)const
{
	std::list<VJSONObject *> objectsWithGivenName;
	std::vector<VJSONElement *>::const_iterator i = m_cd->m_allElements.begin();
	std::vector<VJSONElement *>::const_iterator e = m_cd->m_allElements.end();
	for (i; i != e; ++i)
	{
		VJSONElement *curElement = (*i);
		if (curElement->GetElType() == VJSONElement::ElTypeObject)
		{
			if (curElement->GetName() == keyName)
			{
				objectsWithGivenName.push_back((VJSONObject *)curElement);
			}
		}
	}
	return objectsWithGivenName;
}
