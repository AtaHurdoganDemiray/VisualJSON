// VJSONString.cpp

#include "stdafx.h"
#include "VJSONString.h"
#include "VJSONObjectTemplate.h"

VJSONString::VJSONString(int nameTokeni, VJSONObjectTemplate *root, int tokeni)// here -1 or an index to string object is given to nameTokeni , VJSONObjectTemplate::Parse call this constructor with suitable nameTokeni
	:VJSONElement(nameTokeni, root, tokeni)
{

}

VJSONString::VJSONString(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti)// give name tokeni -2 if this it self is a name for some other element
	: VJSONElement(nameTokeni, root, jsonStr, tokenType , parenti)
{
	if (nameTokeni == -2)
	{
		SetIsKeyNameStr(true);
		m_nameTokeni = -1;
	}
	//m_root->ReconstructStr();
}

VJSONElement::ElType VJSONString::GetElType()const
{
	return ElType::ElTypeString;
}

bool VJSONString::GetIsKeyNameStr()const
{
	return m_isKeyNameStr;
}

void VJSONString::SetIsKeyNameStr(bool isKeyNameStr)
{
	m_isKeyNameStr = isKeyNameStr;
}

void VJSONString::AddToJSONStrWithQuotAndColonsIf(MilString &str)const
{
	str += '\"';
	str += GetJSONString();
	str += '\"';
	if (m_isKeyNameStr)
		str += " : ";
}
