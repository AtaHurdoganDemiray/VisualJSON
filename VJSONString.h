// VJSONString.h

#ifndef __VJSONString_h__
#define __VJSONString_h__

#include"VJSONElement.h"

class VJSONString : public VJSONElement
{
public:
	VJSONString(int nameTokeni ,VJSONObjectTemplate *root , int tokeni);
	VJSONString(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti);
	//void AddToJSONString(MilString &str)const;
	virtual ElType GetElType()const;
private:	
	bool m_isKeyNameStr;
public:
	bool GetIsKeyNameStr()const;
	void SetIsKeyNameStr(bool isKeyNameStr);
	void AddToJSONStrWithQuotAndColonsIf(MilString &str)const;
};

#endif