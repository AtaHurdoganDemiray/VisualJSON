// VJSONBasicVal.cpp

#include "stdafx.h"
#include "VJSONBasicVal.h"
#include "FloatingPointNumberToStr.h"
#include "VJSONObjectTemplate.h"

VJSONBasicVal::VJSONBasicVal(int nameTokeni , VJSONObjectTemplate *root , int tokeni)
	:VJSONElement(nameTokeni,root,tokeni)
{
	//jsmntok_t & curToken = m_root->GetAllTokens()[tokeni];
	//const char * mystr = m_root->GetStr()->c_str();
	//MilString temp = m_root->GetStringFromJSONToken(curToken, mystr);
	//(*this) =  VJSONBasicVal::MakeFromJSONString(temp);
}
VJSONBasicVal::VJSONBasicVal(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti)// not from a parsed file but constructed from GUI
	:VJSONElement(nameTokeni,root,jsonStr,tokenType,parenti)
{
	//m_root->AddItem();
	//m_root->ReconstructStr();
}
//VJSONBasicVal::VJSONBasicVal(double num)
//	:m_type(VJSONBasicVal::floatingPointNumberDbl)
//{
//	m_val = convert<double>(num);
//}
//VJSONBasicVal::VJSONBasicVal(float num)
//	: m_type(VJSONBasicVal::floatingPointNumber)
//{
//	m_val = convert<float>(num);
//}
//VJSONBasicVal::VJSONBasicVal(const MilString &s)
//	: m_type(VJSONBasicVal::stringType)
//{
//	m_val = s;
//}
//VJSONBasicVal::VJSONBasicVal(int num)
//	:m_type(VJSONBasicVal::integerNumber)
//{
//	MilStringStream mystr;
//	mystr << num;
//	mystr >> m_val;
//}
//VJSONBasicVal::VJSONBasicVal(bool b)
//	: m_type(VJSONBasicVal::booleanType)
//{
//	if (b)
//	{
//		m_val = _T("true");
//	}
//	else
//	{
//		m_val = _T("false");
//	}
//}
//VJSONBasicVal::VJSONBasicVal()
//	:m_type(VJSONBasicVal::nullType)
//{
//	m_val = _T("null";)
//}
//void VJSONBasicVal::AddToJSONString(MilString &str)const
//{
	//str += m_val;
//}
//VJSONBasicVal * VJSONBasicVal::MakeDouble(const wstring &str)
//{
//	VJSONBasicVal *val = new VJSONBasicVal(wconvertFrom<double>(str));
//	return val;
//}
//VJSONBasicVal * VJSONBasicVal::MakeFloat(const wstring &str)
//{
//	VJSONBasicVal *val = new VJSONBasicVal(wconvertFrom<float>(str));
//	return val;
//}
//VJSONBasicVal * VJSONBasicVal::MakeString(const wstring &str)
//{
//	VJSONBasicVal *val = new VJSONBasicVal(str);
//	return val;
//}
//VJSONBasicVal * VJSONBasicVal::MakeInt(const wstring &str)
//{
//	wstringstream mystr;
//	mystr << str;
//	int num;
//	mystr >> num;
//	VJSONBasicVal *val = new VJSONBasicVal(num);
//	return val;
//}
bool VJSONBasicVal::IsStrBooleanTrue(const MilString &str)
{
	if (str.length() == 4 && str.at(0) == 't' && str.at(1) == 'r' && str.at(2) == 'u' && str.at(3) == 'e')
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool VJSONBasicVal::IsStrBooleanFalse(const MilString &str)
{
	if (str.length() == 5 && str.at(0) == 'f' && str.at(1) == 'a' && str.at(2) == 'l' && str.at(3) == 's' && str.at(4) == 'e')
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool VJSONBasicVal::IsStrPrimitiveNull(const MilString &str)
{
	if (str.length() == 4 && str.at(0) == 'n' && str.at(1) == 'u' && str.at(2) == 'l' && str.at(3) == 'l')
	{
		return true;
	}
	else
	{
		return false;
	}
}
//VJSONBasicVal * VJSONBasicVal::MakeBool(const wstring &str)
//{
//	if (IsStrBooleanTrue(str))
//	{
//		VJSONBasicVal *myval = new VJSONBasicVal(true);
//		return myval;
//	}
//	else if (IsStrBooleanFalse(str))
//	{
//		VJSONBasicVal *myval = new VJSONBasicVal(false);
//		return myval;
//	}
//	throw std::exception("VJSONBasicVal::MakeBool string is not true or false");
//}
bool VJSONBasicVal::operator == (const VJSONBasicVal &other)const
{
	if (m_type == other.m_type && VJSONElement::GetName() == other.GetName() && VJSONElement::GetJSONString() == VJSONElement::GetJSONString())
		return true;
	else
		return false;
}
bool VJSONBasicVal::operator != (const VJSONBasicVal &other)const
{
	return !(this->operator==(other));
}
VJSONBasicVal & VJSONBasicVal::operator = (const VJSONBasicVal &tocopy)
{
	//m_type = tocopy.m_type;
	//m_val = tocopy.m_val;
	return *this;
}
VJSONBasicVal::VJSONBasicVal(const VJSONBasicVal &tocopy)
	:VJSONElement(tocopy.GetNameTokeni(), tocopy.m_root, tocopy.GetTokeni())
{
	//m_type = tocopy.m_type;
	//m_val = tocopy.m_val;
}

VJSONBasicVal::Type VJSONBasicVal::GetTypeForJSONString(const MilString &jstr)
{
	//wstring wstr(jstr.begin(), jstr.end());
	if (jstr.find('.') != string::npos)
	{
		// this is a floating point number
		return Type::floatingPointNumberDbl;
	}
	else if (IsStrBooleanTrue(jstr) || IsStrBooleanFalse(jstr))
	{
		return Type::booleanType;
	}
	else if (IsStrPrimitiveNull(jstr))
	{
		return Type::nullType;
	}
	else
	{
		return Type::undefinedType;
	}
}

MilString VJSONBasicVal::DoubleToJSONString(double num)
{
	return MilConvert<double>(num);
}

MilString VJSONBasicVal::FloatToJSONString(float num)
{
	return MilConvert<float>(num);
}

MilString VJSONBasicVal::IntToJSONString(int num)
{
	MilStringStream mystr;
	mystr << num;
	MilString temp;
	mystr >> temp;
	return temp;
}

MilString VJSONBasicVal::BooleanToJSONString(bool b)
{
	MilString temp;
	if (b)
	{
		temp = _T("true");
	}
	else
	{
		temp = _T("false");
	}
	return temp;
}

MilString VJSONBasicVal::NullToJSONString()
{
	MilString temp = "null";
	return temp;
}

VJSONElement::ElType VJSONBasicVal::GetElType()const
{
	return ElTypeBasic;
}