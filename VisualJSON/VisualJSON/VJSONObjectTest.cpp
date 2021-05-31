// VJSONObjectTest.cpp

#include "stdafx.h"
#include "VJSONObjectTest.h"
#include "VJSONObject.h"
#include "VJSONObjectTemplate.h"
#include "VJSONObjectTemplateTest.h"
#include "jsmn_test.h"
#include "VJSONString.h"
#include "VJSONArray.h"
#include "VJSONStringTest.h"

VJSONObjectTemplate * VJSONObjectTest::m_testProduct = NULL;

int VJSONObjectTest::TestAll()
{
	JsmnTest::test_passed = 0;
	JsmnTest::test_failed = 0;
	cout << "VJSONObjectTest >>\n";
	JsmnTest::test(TestObj, "VJSONObject::TestObj");
	cout << "PASSED: " << JsmnTest::test_passed << "\n" << "FAILED: " << JsmnTest::test_failed << "\n";
	cout << "<< VJSONObjectTest\n";
	return 0;
}

#define VJSONOBJECTTEST_finalJsonShouldBe  "{		\
		[											\
			14.4									\
			, 1.3									\
			, 3.14									\
			, \"merhaba\"							\
			, {										\
				\"pi\" : 3.14						\
			}										\
		]											\
		, \"AnameStr\" : \"Avalue\"					\
		, [											\
			14.4									\
			, 1.3									\
			, 3.14									\
			, \"merhaba\"							\
			, {										\
				\"pi\" : 3.14						\
			}										\
		]											\
}"

int VJSONObjectTest::TestObj()
{
	MilString templateName("TestContainer");
	VJSONObjectTemplate *myContainer = VJSONObjectTemplateTest::MakeEmptyVJSONObjectTemplate(templateName);
	VJSONObject * firstEmptyGrandObject = MakeFirstEmptyGrandObject(myContainer);
	check(myContainer->GetTemplateRootJSONObject() != NULL);
	check(firstEmptyGrandObject == myContainer->GetTemplateRootJSONObject()->GetFirstChild());
	check(firstEmptyGrandObject->GetJSONStringWithoutWhiteSpaceIf() == "{}");

	// Add a string 
	int nameTokeni = -2;
	MilString keyStr = "AnameStr";
	MilString valStr = "Avalue";	// string initial definition without quotations
	bool addCommaAtStart0(false);
	bool addCommaAtEnd0(false);
	MilString rawJsonStrKeyVal = VJSONObjectTemplate::GetRawJsonString(keyStr, valStr, firstEmptyGrandObject->GetElType(), VJSONElement::ElTypeString, addCommaAtStart0, addCommaAtEnd0);
	// rawString will have quotations , commas and column marks as needed
	int parenti = 0;
	myContainer->InsertRawJsonStringAsFirstChild(firstEmptyGrandObject, rawJsonStrKeyVal);
	std::pair<VJSONElement*, VJSONElement*> parPair1 =	myContainer->ReconstructStr(firstEmptyGrandObject,NULL);
	

	VJSONElement *tmp1 = myContainer->GetTemplateRootJSONObject();
	check(tmp1 != NULL);
	VJSONElement *tmp2 = tmp1->GetFirstChild();
	check(tmp2 != NULL);
	VJSONElement *tmp3 = tmp2->GetFirstChild();
	check(tmp3 != NULL);
	VJSONString *toTest1 = (VJSONString *)tmp3;
	VJSONElement *newAddedKeyStr = parPair1.first->GetFirstChild();
	check(tmp3 == newAddedKeyStr);
	VJSONElement *tmp4 = tmp3->GetNext();
	check(tmp4 != NULL);
	VJSONElement *newAddedValStr = newAddedKeyStr->GetNext();
	check(tmp4 == newAddedValStr);
	VJSONString *toTest2 = (VJSONString *)tmp4;
	VJSONStringTest::Test(toTest1 , keyStr , true);
	VJSONStringTest::Test(toTest2, valStr, false);

	//  Add an array 
	//jsmntype_t tokTyp2 = jsmntype_t::JSMN_ARRAY;
	MilString unnamedTempStr("");
	MilString arrStr = "[14.4,1.3,3.14,\"merhaba\",{\"pi\":3.14}]";		// if a string is inside array or object its quotation marks should be added
	VJSONElement *reFirstEmptyGrandObject = myContainer->GetAllElements().at(0); // ReconstructStr delete all old elements , reparse and make brand new elements
	bool addCommaAtStart1(false);
	bool addCommaAtEnd1(true);
	MilString rawjsonStrArr1 = VJSONObjectTemplate::GetRawJsonString(unnamedTempStr, arrStr, reFirstEmptyGrandObject->GetElType(),VJSONElement::ElTypeArray,addCommaAtStart1,addCommaAtEnd1);

	check(firstEmptyGrandObject != reFirstEmptyGrandObject);
	VJSONElement *firstChildInserted = reFirstEmptyGrandObject->GetFirstChild();
	check(firstChildInserted != NULL);

	myContainer->InsertRawJsonStringAsFirstChild(reFirstEmptyGrandObject , rawjsonStrArr1);
	myContainer->ReconstructStr(reFirstEmptyGrandObject,NULL);
	
	VJSONElement *rereGrandObject = myContainer->GetAllElements().at(0);
	bool addCommaAtStart2(true);
	bool addCommaAtEnd2(false);
	MilString rawjsonStrArr2 = VJSONObjectTemplate::GetRawJsonString(unnamedTempStr, arrStr, rereGrandObject->GetElType(), VJSONElement::ElTypeArray, addCommaAtStart2, addCommaAtEnd2);
	myContainer->InsertRawJsonStringAsLastChild(rereGrandObject,rawjsonStrArr2);
	std::pair<VJSONElement *,VJSONElement *> parPair2 = myContainer->ReconstructStr(rereGrandObject,rereGrandObject->GetLastChild());

	VJSONElement *rerereGrandObject = myContainer->GetAllElements().at(0); // be careful first element change again 
	check(rereGrandObject != rerereGrandObject);
	check(rerereGrandObject == parPair2.first);
	MilString tock1(VJSONElement::RemoveWhiteSpaceOutOfQuotations(MilString(VJSONOBJECTTEST_finalJsonShouldBe)));
	//tock1 += "\n****\n";
	MilString tock2 (VJSONElement::RemoveWhiteSpaceOutOfQuotations(*myContainer->GetStr()));//rerereGrandObject->GetJSONStringWithoutWhiteSpaceIf();
	//AfxMessageBox((tock1+tock2).c_str());
	check(tock1 == tock2);

	m_testProduct = myContainer;
	return 0;
}
VJSONObject * VJSONObjectTest::MakeFirstEmptyGrandObject(VJSONObjectTemplate *container)
{
	int nameTokeniObj = -1;
	int parentiObj = -1;
	MilString jsonStringObj = "{}";
	jsmntype_t tokenTypeObj = jsmntype_t::JSMN_OBJECT;
	VJSONObject *myobj = new VJSONObject(nameTokeniObj, container , jsonStringObj, tokenTypeObj, parentiObj);
	return myobj;
}
