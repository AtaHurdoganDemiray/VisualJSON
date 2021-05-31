// VJSONObjectTest.h

#ifndef __VJSONObjectTest_h__
#define __VJSONObjectTest_h__

#include "VJSONElement.h"

class VJSONObject;
class VJSONObjectTemplate;

class VJSONObjectTest
{
public:
	static int TestAll();
	static VJSONObject * MakeFirstEmptyGrandObject(VJSONObjectTemplate *container);
	static int TestObj();
	static VJSONObjectTemplate * m_testProduct;
};

#endif