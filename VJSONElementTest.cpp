// VJSONElementTest.cpp

#include "stdafx.h"
#include "VJSONElementTest.h"
#include "VJSONElement.h"
#include "VJSONObjectTemplate.h"
#include "VJSONBasicVal.h"
#include "VJSONString.h"
#include "jsmn_test.h"
#include "VJSONObject.h"
#include "VJSONObjectTemplateTest.h"

//bool VJSONElementTest::Test(MilIFStream &inf, MilOFStream &of)
//{
//	VJSONElement(VJSONObjectTemplate *root);// used only for VJSONObjectTemplate::m_templateRootJSONObject
//	VJSONElement(int nameTokeni, VJSONObjectTemplate *root, int tokeni);
//	VJSONElement(int nameTokeni, VJSONObjectTemplate *root, const MilString &jsonStr, jsmntype_t tokenType, int parenti);// not from parsed file but made manually by GUI 
//	//VJSONElement(const VJSONElement &toCopy);
//	//const VJSONElement & operator = (const VJSONElement &toCopy);
//	void AddToJSONStringWithNameQuotAndColons(MilString &str)const;
//	MilString GetJSONString()const;
//	MilString GetName()const;
//protected:
//	VJSONObjectTemplate *m_root;
//	int m_nameTokeni;// -1 means Element does not have a key name , -2 means Element it self is a key name string , 0 or positive integer are index to VJSONString element for key name string
//	VJSONElement * m_parent;
//	VJSONElement * m_firstChild;
//	VJSONElement * m_next;
//	VJSONElement * m_prev;
//	int m_tokeni;
//	//void VJSONElement::SetParentForConstructor(const jsmntok_t &t);
//public:
//	VJSONElement * GetParent();
//	VJSONElement * GetFirstChild();
//	VJSONElement * GetNext();
//	VJSONElement * GetPrev();
//	int GetTokeni()const;
//	int GetNameTokeni()const;
//protected:
//	void SetParent(VJSONElement * parent);
//	void SetFirstChild(VJSONElement *child);
//	void SetNext(VJSONElement * next);
//	void SetPrev(VJSONElement *prev);
//	//void AddNameToJSONString(MilString &str)const;
//	void AddChild(VJSONElement *child);

	// Test constructors 




//	return true;
//}

int VJSONElementTest::TestAll()
{
	JsmnTest::test_passed = 0;
	JsmnTest::test_failed = 0;
	cout << "VJSONObjectTemplateTest >>\n";
	JsmnTest::test(TestCompare, "VJSONObjectTemplate::ConstructAndParse");
	cout << "PASSED: " << JsmnTest::test_passed << "\n" << "FAILED: " << JsmnTest::test_failed << "\n";
	cout << "<< VJSONObjectTemplateTest\n";
	return 0;
}

int VJSONElementTest::TestCompare()
{
	// Make some elements in different object templates
	MilString templateName("emptyAtFirstTemplate1");
	VJSONObjectTemplate * t1 = VJSONObjectTemplateTest::MakeEmptyVJSONObjectTemplate(templateName);
	
	MilString templateName2("emptyAtFirstTemplate2");
	VJSONObjectTemplate * t2 = VJSONObjectTemplateTest::MakeEmptyVJSONObjectTemplate(templateName2);

	// object templates are constructed 
	// now making some elements manually by strings

	
	int nameTokeni = -2; // if this is a name string it self , we give -2 for nameTokeni (index to a name string)
	MilString jsonString = "";
	jsmntype_t tokenTypeStr = jsmntype_t::JSMN_STRING;
	int parentiStr = 0;
	VJSONString * s1 = new VJSONString(nameTokeni, t1, jsonString, tokenTypeStr, parentiStr);
	


	// first make 


	// compare them an make sure intended results happen



	return 0;
}