// VJSONSingleKeyVal.h

#ifndef __VJSONSingleKeyVal_h__
#define __VJSONSingleKeyVal_h__

#include "VJSONElement.h"
#include "VJSONBasicVal.h"

class VJSONSingleKeyVal : public VJSONElement
{
public:
	VJSONSingleKeyVal(const MilString &name, VJSONObjectTemplate *root, int tokeni);
	
	VJSONSingleKeyVal(const MilString &name , const VJSONBasicVal &val , VJSONObjectTemplate *root);
	const VJSONBasicVal & GetVal()const;
	void SetVal(const VJSONBasicVal &val);
	virtual void AddToJSONString(MilString &str)const;
protected:
	VJSONBasicVal m_val;
};

#endif