// ExportAsciiTextJSON.cpp

#include "stdafx.h"
#include "ExportAsciiTextJSON.h"
#include "VJSONObjectTemplate.h"
#include "VJSONObject.h"
#include "VJSONArray.h"
#include "VJSONString.h"
#include "VJSONBasicVal.h"

ExportAsciiTextJSON::ExportAsciiTextJSON(VJSONObjectTemplate *toExport, const MilString &fullFilePath)
{
	m_tabCounter = 0;
	m_tabChar = char(9);
	m_fullFilePath = fullFilePath;
	VJSONObject *templateRoot = toExport->GetTemplateRootJSONObject();
	VJSONElement *firstChild = templateRoot->GetFirstChild();
	MilString *mystr = new MilString();
	if (firstChild != NULL)
	{
		AddToStrRecursive(templateRoot, firstChild, *mystr);
	}

	MilOFStream f(m_fullFilePath);
	if (f.is_open())
	{
		size_t starti = 0;
		size_t strlen = mystr->length();
		if (strlen > 0)
		{
			if (mystr->operator[](0) == '\n')
				++starti;
			for (size_t i = 0; i < strlen; ++i)
				f.put(mystr->operator[](i));
		}
		f.close();
	}
	else
		throw exception("ExportAsciiTextJSON::ExportAsciiTextJSON file could not be opened");
}
ExportAsciiTextJSON::ExportAsciiTextJSON(VJSONObjectTemplate *toExport, MilString * exportedStr)// exportedStr must no be NULL , user is responsible to delete that
{
	m_tabCounter = 0;
	m_tabChar = char(9);
	VJSONObject *templateRoot = toExport->GetTemplateRootJSONObject();
	VJSONElement *firstChild = templateRoot->GetFirstChild();
	if (firstChild != NULL)
	{
		AddToStrRecursive(templateRoot, firstChild, *exportedStr);
	}
}
bool ExportAsciiTextJSON::ShouldIPrintComaBefore(VJSONElement *parent , VJSONElement *toPrint)
{
	VJSONElement *firstChild = parent->GetFirstChild();
	if (toPrint == firstChild)
		return false;
	else
		return true;
}
VJSONElement * ExportAsciiTextJSON::AddToStrRecursiveKeyValPair(VJSONElement *parent, VJSONString *keyToAdd, VJSONElement *valToAdd, MilString &str)
{
	//if (m_tabCounter != 0)
	str += '\n';
	for (int ti = 0; ti < m_tabCounter; ++ti)
		str += m_tabChar;
	if (ShouldIPrintComaBefore(parent,keyToAdd))
		str += ',';
	keyToAdd->AddToJSONStrWithQuotAndColonsIf(str);
	//m_isPrevBasicTypeOrValStr = false;
	VJSONElement::ElType valType = valToAdd->GetElType();
	VJSONString *valJSONString = NULL;

	switch (valType)
	{
	case VJSONElement::ElTypeObject:
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		str += '{';
		++m_tabCounter;
		//m_isPrevBasicTypeOrValStr = false;
		break;
	case VJSONElement::ElTypeArray:
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		str += '[';
		++m_tabCounter;
		//m_isPrevBasicTypeOrValStr = false;
		break;
	case VJSONElement::ElTypeString:
		//if (m_isPrevBasicTypeOrValStr)
		//	str += " , ";
		valJSONString = (VJSONString *)valToAdd;
		//m_isPrevBasicTypeOrValStr = true;
		valJSONString->AddToJSONStrWithQuotAndColonsIf(str);
		break;
	case VJSONElement::ElTypeBasic:
		//if (m_isPrevBasicTypeOrValStr)
		//	str += " , ";
		//m_isPrevBasicTypeOrValStr = true;
		str += valToAdd->GetJSONString();
		break;
	};

	VJSONElement *firstChild = valToAdd->GetFirstChild();
	VJSONElement *nextEl = valToAdd->GetNext();
	
	if (firstChild != NULL)
		AddToStrRecursive(valToAdd, firstChild, str);


	switch (valType)
	{
	case VJSONElement::ElTypeObject:
		--m_tabCounter;
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		str += '}';
		//--m_tabCounter;
		break;
	case VJSONElement::ElTypeArray:
		--m_tabCounter;
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		str += ']';
		//--m_tabCounter;
		break;
	case VJSONElement::ElTypeString:
		
		break;
	case VJSONElement::ElTypeBasic:
		
		break;
	};
	if (nextEl != NULL)
		AddToStrRecursive(parent, nextEl, str);
	return valToAdd;
}
VJSONElement * ExportAsciiTextJSON::AddToStrRecursiveNoKeyOnlyVal(VJSONElement *parent, VJSONElement *valToAdd, MilString &str)
{
	VJSONElement::ElType valType = valToAdd->GetElType();
	VJSONString *valJSONString = NULL;
	switch (valType)
	{
	case VJSONElement::ElTypeObject:
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		if (ShouldIPrintComaBefore(parent, valToAdd))
			str += ',';
		str += '{';
		++m_tabCounter;
		//m_isPrevBasicTypeOrValStr = false;
		break;
	case VJSONElement::ElTypeArray:
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		if (ShouldIPrintComaBefore(parent, valToAdd))
			str += ',';
		str += '[';
		++m_tabCounter;
		//m_isPrevBasicTypeOrValStr = false;
		break;
	case VJSONElement::ElTypeString:
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		if (ShouldIPrintComaBefore(parent, valToAdd))
			str += " , ";
		//m_isPrevBasicTypeOrValStr = true;
		valJSONString = (VJSONString *)valToAdd;
		valJSONString->AddToJSONStrWithQuotAndColonsIf(str);
		break;
	case VJSONElement::ElTypeBasic:
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		if (ShouldIPrintComaBefore(parent,valToAdd))
			str += " , ";
		//m_isPrevBasicTypeOrValStr = true;
		str += valToAdd->GetJSONString();
		break;
	};
	VJSONElement *firstChild = valToAdd->GetFirstChild();
	VJSONElement *nextEl = valToAdd->GetNext();

	if (firstChild != NULL)
		AddToStrRecursive(valToAdd, firstChild, str);


	switch (valType)
	{
	case VJSONElement::ElTypeObject:
		--m_tabCounter;
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		str += '}';
		//--m_tabCounter;
		break;
	case VJSONElement::ElTypeArray:
		--m_tabCounter;
		str += '\n';
		for (int ti = 0; ti < m_tabCounter; ++ti)
			str += m_tabChar;
		str += ']';
		//--m_tabCounter;
		break;
	case VJSONElement::ElTypeString:

		break;
	case VJSONElement::ElTypeBasic:

		break;
	};
	if (nextEl != NULL)
		AddToStrRecursive(parent, nextEl, str);
	return valToAdd;
}
VJSONElement * ExportAsciiTextJSON::AddToStrRecursive(VJSONElement *parent, VJSONElement *toAdd, MilString &str)
{
	if (parent == NULL)
		return NULL;
	VJSONString *curstring = NULL;
	VJSONElement::ElType etype = toAdd->GetElType();
	switch (etype)
	{
	case VJSONElement::ElTypeString:
		curstring = (VJSONString *)toAdd;
		if (curstring->GetIsKeyNameStr() == true)
		{
			VJSONElement *nextItem = curstring->GetNext();
			if (nextItem == NULL)
				throw exception("CVisualJSONDlg::AddToTreeRecursive exception , next element should not be NULL");
			return AddToStrRecursiveKeyValPair(parent, curstring, nextItem, str);
		}
		else
		{
			return AddToStrRecursiveNoKeyOnlyVal(parent, toAdd, str);
		}

		break;
	case VJSONElement::ElTypeBasic:
		return AddToStrRecursiveNoKeyOnlyVal(parent, toAdd, str);
		break;
	case VJSONElement::ElTypeArray:
		return AddToStrRecursiveNoKeyOnlyVal(parent, toAdd, str);
		break;
	case VJSONElement::ElTypeObject:
		return AddToStrRecursiveNoKeyOnlyVal(parent, toAdd, str);
		break;
	default:
		break;
	};
	return NULL;
}

//MilString & ExportAsciiTextJSON::GetStr()
//{
//	return m_str;
//}