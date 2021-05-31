// VJSONArray.cpp

#include "stdafx.h"
#include "VJSONArray.h"
#include "VJSONObjectTemplate.h"

VJSONArray::VJSONArray(int nameTokeni , VJSONObjectTemplate *root , int tokeni)
	:VJSONElement(nameTokeni,root,tokeni)
{

}
VJSONArray::VJSONArray(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti)
	: VJSONElement(nameTokeni, root, jsonStr, tokenType, parenti)
{
	//m_root->ReconstructStr();
}
VJSONArray::~VJSONArray()
{

}
//void VJSONArray::AddToJSONString(MilString &str)const
//{
//	str += m_root->GetElementJSONString(m_tokeni);
//}

void VJSONArray::AddElement(VJSONElement * element)
{

}

VJSONElement::ElType VJSONArray::GetElType()const
{
	return ElTypeArray;
}