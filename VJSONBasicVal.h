// VJSONBasicVal.h

#ifndef __VJSONBasicVal_h__
#define __VJSONBasicVal_h__

#include "VJSONElement.h"

class VJSONBasicVal : public VJSONElement
{
public:
	VJSONBasicVal(int nameTokeni , VJSONObjectTemplate *root,int tokeni);
	VJSONBasicVal(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti);// not from a parsed file but constructed from GUI
	enum Type {floatingPointNumber, floatingPointNumberDbl, integerNumber, booleanType, nullType , undefinedType};
	bool operator == (const VJSONBasicVal &other)const;
	bool operator != (const VJSONBasicVal &other)const;
	//virtual void AddToJSONString(MilString &str)const;// with quotes and colon if needed
	//MilString GetJSONString()const;// with quotes and colon if needed
	VJSONBasicVal(const VJSONBasicVal &tocopy);
	VJSONBasicVal & operator = (const VJSONBasicVal &tocopy);
	Type GetType()const;
private:
	//VJSONBasicVal(double num);
	//VJSONBasicVal(float num);
	//VJSONBasicVal(int num);
	//VJSONBasicVal(bool b);
	//VJSONBasicVal();
	//static VJSONBasicVal * MakeDouble(const MilString &str);
	//static VJSONBasicVal * MakeFloat(const MilString &str);
	//static VJSONBasicVal * MakeString(const MilString &str);
	//static VJSONBasicVal * MakeInt(const MilString &str);
	//static VJSONBasicVal * MakeBool(const MilString &str);
	//static VJSONBasicVal * MakeFromJSONString(const MilString &jstr);
	//static VJSONBasicVal * MakeNull(const MilString &str);
public:
	static bool IsStrBooleanTrue(const MilString &str);
	static bool IsStrBooleanFalse(const MilString &str);
	static bool IsStrPrimitiveNull(const MilString &str);
	static MilString IntToJSONString(int num);
	static MilString DoubleToJSONString(double num);
	static MilString FloatToJSONString(float num);
	static MilString BooleanToJSONString(bool b);
	static MilString NullToJSONString();
	static Type GetTypeForJSONString(const MilString &str);
	//void AddToJSONString(MilString &str)const;
	virtual ElType GetElType()const;
private:
	Type m_type;
};

#endif