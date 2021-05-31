// VJSONObject.h

#ifndef __VJSONObject_h__
#define __VJSONObject_h__

#include "VJSONElement.h"

class VJSONObject : public VJSONElement
{
public:
	VJSONObject(VJSONObjectTemplate *root);// only for Template Root Object
	VJSONObject(int nameTokeni , VJSONObjectTemplate *root , int tokeni);
	VJSONObject(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti);
	virtual ~VJSONObject();
	//void AddToJSONString(MilString &str)const;
	virtual ElType GetElType()const;
private:
};

#endif