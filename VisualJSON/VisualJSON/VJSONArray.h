// VJSONArray.h

#ifndef __VJSONArray_h__
#define __VJSONArray_h__

#include "VJSONElement.h"

class VJSONArray : public VJSONElement
{
public:
	VJSONArray(int nameTokeni , VJSONObjectTemplate *root , int tokeni);
	VJSONArray(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti);
	virtual ~VJSONArray();
	//void AddToJSONString(MilString &str)const;
	void AddElement(VJSONElement * element);
	virtual ElType GetElType()const;
};

#endif