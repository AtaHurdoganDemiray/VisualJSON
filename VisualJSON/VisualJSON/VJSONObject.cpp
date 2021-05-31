// VJSONObject.cpp

#include"stdafx.h"
#include "VJSONObject.h"
#include "VJSONBasicVal.h"
#include "VJSONObjectTemplate.h"

VJSONObject::VJSONObject(VJSONObjectTemplate *root)
	:VJSONElement(root)
{

}
VJSONObject::VJSONObject(int nameTokeni, VJSONObjectTemplate *root, int tokeni)
: VJSONElement (nameTokeni,root,tokeni)
{

}
VJSONObject::VJSONObject(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti)
	: VJSONElement(nameTokeni, root, jsonStr, tokenType, parenti)
{
	//m_root->ReconstructStr();
}
VJSONObject::~VJSONObject()
{

}
//void VJSONObject::AddToJSONString(MilString &str)const
//{
//	str += m_root->GetElementJSONString(m_tokeni);
//}
VJSONElement::ElType VJSONObject::GetElType()const
{
	return ElTypeObject;
}