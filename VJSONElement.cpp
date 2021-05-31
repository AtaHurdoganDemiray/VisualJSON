// VJSONElement.cpp

#include "stdafx.h"
#include "VJSONElement.h"
#include "VJSONObjectTemplate.h"
#include "VJSONObjectTemplateTest.h"

VJSONElement::VJSONElement(VJSONObjectTemplate *root)// used only for VJSONObjectTemplate::m_templateRootJSONObject
	:m_nameTokeni(-1)
	, m_parent(NULL)
	, m_firstChild(NULL)
	, m_next(NULL)
	, m_prev(NULL)
	, m_tokeni(-1)
	, m_root(root)
{
#ifdef _DEBUG
	VJSONObjectTemplateTest::AddToMemoryLeakCheckMap(this);
#endif
}
VJSONElement::VJSONElement(int nameTokeni , VJSONObjectTemplate *root , int tokeni)
	:m_nameTokeni(nameTokeni)
	, m_parent(NULL)
	, m_firstChild(NULL)
	, m_next(NULL)
	, m_prev(NULL)
	, m_tokeni(tokeni)
	, m_root(root)
{
	//if (m_tokeni != -1)
	//{
		jsmntok_t &t = m_root->GetAllTokens()[m_tokeni];
		//SetParentForConstructor(t);
		if (this != ((VJSONElement *)m_root->GetTemplateRootJSONObject()))
			m_root->GetAllElements().push_back(this);

		if (t.parent != -1)
		{
			VJSONElement *parentElement = m_root->GetAllElements().at(t.parent);
			parentElement->AddChild(this);
			this->SetParent(parentElement);
		}
		else
		{
			VJSONElement *tempRoot = (VJSONElement *)m_root->GetTemplateRootJSONObject();
			this->SetParent(tempRoot);
			tempRoot->AddChild(this);
		}
	//}
#ifdef _DEBUG
		VJSONObjectTemplateTest::AddToMemoryLeakCheckMap(this);
#endif
}
VJSONElement::VJSONElement(int nameTokeni, VJSONObjectTemplate *root,const MilString &jsonStr, jsmntype_t tokenType , int parenti)// not from parsed file but made manually by GUI 
	:m_nameTokeni(nameTokeni)
	, m_parent(NULL)
	, m_firstChild(NULL)
	, m_next(NULL)
	, m_prev(NULL)
	, m_tokeni(-1)
	, m_root(root)
{
	std::vector<VJSONElement *> &allel = m_root->GetAllElements();
	int elSize = (int)allel.size();
	m_tokeni = elSize;
	m_root->EnlargeCapacityIf(m_tokeni);
	jsmntok_t t;
	t.parent = parenti;
	t.type = tokenType;
	MilString &bigStr = *m_root->GetStr();
	if (tokenType == JSMN_STRING)
		bigStr += '\"';
	t.start = bigStr.length();
	bigStr += jsonStr;
	t.end = bigStr.length();
	if (tokenType == JSMN_STRING)
		bigStr += '\"';
	if (m_nameTokeni == -2)
	{
		bigStr += ':';
	}
	m_root->GetAllTokens()[m_tokeni] = t;
	m_root->IncrementNumTokens();
	//SetParentForConstructor(t);
	if (this != ((VJSONElement *)m_root->GetTemplateRootJSONObject()))
		m_root->GetAllElements().push_back(this);
	if (t.parent != -1)
	{
		VJSONElement *parentElement = m_root->GetAllElements().at(t.parent);
		parentElement->AddChild(this);
		this->SetParent(parentElement);
		jsmntok_t &parentTok = m_root->GetAllTokens()[t.parent];
		parentTok.size++;
	}
	else
	{
		VJSONElement *tempRoot = (VJSONElement *)m_root->GetTemplateRootJSONObject();
		this->SetParent(tempRoot);
		tempRoot->AddChild(this);
	}
#ifdef _DEBUG
	VJSONObjectTemplateTest::AddToMemoryLeakCheckMap(this);
#endif
}
VJSONElement::~VJSONElement()
{
#ifdef _DEBUG
	std::map<VJSONElement *, VJSONElement *>::iterator fi = VJSONObjectTemplateTest::m_memoryLeakCheckMap.find(this);
	if (fi == VJSONObjectTemplateTest::m_memoryLeakCheckMap.end())
	{
		throw std::exception("VJSONElement::destructor exception , this object was deleted before or not added to check map");
	}
	else
	{
		VJSONObjectTemplateTest::m_memoryLeakCheckMap.erase(fi);
		
	}
#endif
}
//VJSONElement::VJSONElement (const VJSONElement &toCopy)
//	:m_nameTokeni(-1)
//	, m_parent(toCopy.m_parent)
//	, m_firstChild(NULL)
//	, m_next(NULL)
//	, m_prev(NULL)
//	, m_tokeni(-1)
//	, m_root(toCopy.m_root)
//{
//	if (toCopy.m_nameTokeni != -1)
//	{
//		// Make same name but different element
//
//	}
//}
//const VJSONElement & VJSONElement::operator = (const VJSONElement &toCopy)
//{
//
//	return *this;
//}
//void VJSONElement::SetParentForConstructor(const jsmntok_t &t)
//{
//	if (this != ((VJSONElement *)m_root->GetTemplateRootJSONObject()))
//		m_root->GetAllElements().push_back(this);
//
//	if (t.parent != -1)
//	{
//		VJSONElement *parentElement = m_root->GetAllElements().at(t.parent);
//		parentElement->AddChild(this);
//		this->SetParent(parentElement);
//	}
//	else
//	{
//		this->SetParent((VJSONElement *)m_root->GetTemplateRootJSONObject());
//	}
//}
VJSONElement * VJSONElement::GetParent()
{
	return m_parent;
}
VJSONElement * VJSONElement::GetFirstChild()
{
	return m_firstChild;
}
VJSONElement * VJSONElement::GetNext()
{
	return m_next;
}
VJSONElement * VJSONElement::GetPrev()
{
	return m_prev;
}
void VJSONElement::SetParent(VJSONElement * parent)
{
	m_parent = parent;
}
void VJSONElement::SetFirstChild(VJSONElement *child)
{
	m_firstChild = child;
}
void VJSONElement::SetNext(VJSONElement * next)
{
	m_next = next;
}
void VJSONElement::SetPrev(VJSONElement *prev)
{
	m_prev = prev;
}
MilString VJSONElement::GetName()const
{
	if (m_nameTokeni != -1 && m_root != NULL)
	{
		return m_root->GetElementJSONString(m_nameTokeni);
	}
	else
	{
		return "";
	}
}
//void VJSONElement::AddNameToJSONString(MilString &str)const
//{
//	if (m_name != _T(""))
//	{
//		str += m_name;
//		str += _T(": ");
//	}
//}

void VJSONElement::AddChild(VJSONElement *child)
{
	if (GetFirstChild() == NULL)
	{
		SetFirstChild(child);
	}
	else
	{
		VJSONElement *mychild = GetFirstChild();
		while (mychild->GetNext() != NULL)
			mychild = mychild->GetNext();
		mychild->SetNext(child);
		child->SetPrev(mychild);
	}
}
//void VJSONElement::AddToJSONStringWithNameQuotAndColons(MilString &str)const
//{
//	//MilString nameStr = GetName();
//	//if (nameStr != "")
//	//{
//	//	str += nameStr;
//	//	str += " : ";
//	//}
//	m_root->AddToJSONStringWithNameQuotAndColons(str);
//
//}

MilString VJSONElement::GetJSONString()const
{
	if (m_root != NULL && m_tokeni != -1)
	{
		return m_root->GetElementJSONString(m_tokeni);
	}
	else
	{
		return "";
	}
}

int VJSONElement::GetTokeni()const
{
	return m_tokeni;
}

int VJSONElement::GetNameTokeni()const
{
	return m_nameTokeni;
}

MilString VJSONElement::GetElTypeName()const
{
	ElType eltype = GetElType();
	MilString temp("Undefined");
	switch (eltype)
	{
	case ElTypeObject:
		temp = "Object";
		break;
	case ElTypeArray:
		temp = "Array";
		break;
	case ElTypeString:
		temp = "String";
		break;
	case ElTypeBasic:
		temp = "Basic";
		break;
	};
	return temp;
}

VJSONElement::CompareResult VJSONElement::Compare(const VJSONElement *other)const
{
	CompareResult myck;
	myck.m_keyComp = KeyCompareResult::DifferentKey;
	myck.m_locComp = LocationCompareResult::DifferentLocation;
	myck.m_valComp = ValueCompareResult::DifferentTypes;
	return myck;
}

MilString VJSONElement::GetJSONStringWithoutWhiteSpaceIf()const
{
	MilString jstr = GetJSONString();
	//MilString jstrNoWhite;
	//jstrNoWhite.reserve(jstr.size());
	//for (int i = 0; i < jstr.length(); ++i)
	//{
	//	char curc = jstr[i];
	//	if (curc != ' ' && curc != '\n' && curc != '\r' && curc != '\t')
	//		jstrNoWhite += curc;
	//}
	//return jstrNoWhite;
	return RemoveWhiteSpaceOutOfQuotations(jstr);
}

MilString VJSONElement::RemoveWhiteSpaceOutOfQuotations(const MilString &toClean)
{
	MilString jstrNoWhite;
	jstrNoWhite.reserve(toClean.size());
	int quotCounter = -1;
	for (unsigned int i = 0; i < toClean.length(); ++i)
	{
		char curc = toClean[i];
		if (curc == '\"')
			++quotCounter;
		if (quotCounter % 2 == 0)
			jstrNoWhite += curc;
		else if (curc != ' ' && curc != '\n' && curc != '\r' && curc != '\t')
			jstrNoWhite += curc;
	}
	return jstrNoWhite;
}

int VJSONElement::GetNameCountForFirstLevelChildren(const MilString &newKeyName)
{
	int counter = 0;
	if (GetElType() == ElTypeObject || GetElType() == ElTypeArray)
	{
		VJSONElement *curChild = GetFirstChild();
		while (curChild != NULL)
		{
			if (m_root->IsElementKeyNameStrDuplicate(curChild, newKeyName))
				++counter;
			curChild = curChild->GetNext();
		}
	}
	return counter;
}

VJSONElement * VJSONElement::GetLastChild()
{
	VJSONElement *curChild = GetFirstChild();
	if (curChild == NULL)
		return curChild;
	while (curChild->GetNext() != NULL)
	{
		curChild = curChild->GetNext();
	}
	return curChild;
}

void VJSONElement::CollectAllChildrenRecursive(std::list<VJSONElement *> &allChildren)
{
	if (GetFirstChild() != NULL)
	{
		VJSONElement *curChild = GetFirstChild();
		while (curChild != NULL)
		{
			allChildren.push_back(curChild);
			curChild->CollectAllChildrenRecursive(allChildren);
			curChild = curChild->GetNext();
		}
	}
}
