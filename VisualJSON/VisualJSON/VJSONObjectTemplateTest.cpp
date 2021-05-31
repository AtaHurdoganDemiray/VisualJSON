// VJSONObjectTemplateTest.cpp

#include "stdafx.h"
#include "VJSONObjectTemplateTest.h"
#include "jsmn_test.h"
#include "VJSONObjectTemplate.h"
#include "VJSONObject.h"

MilString VJSONTESTSTR = "{\
									\"glossary\": {					\
													\"title\": \"example glossary\",\
													\"GlossDiv\" : {\
																	\"title\": \"S\",\
																	\"GlossList\" : {\
																						\"GlossEntry\": {\
																										\"ID\": \"SGML\",\
																										\"SortAs\" : \"SGML\",\
																										\"GlossTerm\" : \"Standard Generalized Markup Language\",\
																										\"Acronym\" : \"SGML\",\
																										\"Abbrev\" : \"ISO 8879:1986\",\
																										\"GlossDef\" : {\
																														\"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",\
																														\"GlossSeeAlso\" : [\"GML\", \"XML\"]\
																														},\
																										\"GlossSee\" : \"markup\"\
																										}\
																					}\
																	}\
												}\
								}";

#ifdef _DEBUG
	std::map<VJSONElement *,VJSONElement *> VJSONObjectTemplateTest::m_memoryLeakCheckMap = std::map<VJSONElement *, VJSONElement *>();
	void VJSONObjectTemplateTest::AddToMemoryLeakCheckMap(VJSONElement *el)
	{
		m_memoryLeakCheckMap[el] = el;
	}
#endif
VJSONObjectTemplate * VJSONObjectTemplateTest::MakeVJSONObjectTemplateByString(MilString *str, const MilString &templateName , const MilString &fileName) // str should stay on memory , it will be used during life time of VJSONObjectTemplate
{
	VJSONObjectTemplate *newTemp = new VJSONObjectTemplate();
	VJSONObject * templateRootJSONObj = new VJSONObject(newTemp);
	newTemp->ConstructAndParse(templateRootJSONObj, templateName, fileName, str);
	return newTemp;
}

VJSONObjectTemplate * VJSONObjectTemplateTest::MakeEmptyVJSONObjectTemplate(const MilString &templateName)
{
	VJSONObjectTemplate * tEmpty = new VJSONObjectTemplate();
	VJSONObject * templateRootJSONObj = new VJSONObject(tEmpty);
	//MilString fileName("");// dummy file name , which means it is not read from file but constructed manually
	tEmpty->Construct(templateRootJSONObj, templateName);
	return tEmpty;
}

int VJSONObjectTemplateTest::TestConstructAndParse()
{
	//check()
	MilString templateName("GlossaryTemplate1");
	MilString fileName("");// file name for the source , it should be read before and loaded to string (here VJSONTESTSTR is used for construction)
	VJSONObjectTemplate *newTemp = MakeVJSONObjectTemplateByString(&VJSONTESTSTR, templateName, fileName);// parser is using VJSONTESTSTR which is already filled with information,filename is dummy
	VJSONObject *templateRootJSONObj = newTemp->GetTemplateRootJSONObject();
	// iterate for all elements in the template

	VJSONElement * firstChild = templateRootJSONObj->GetFirstChild();
	check(firstChild != NULL);
	
	vector<VJSONElement *> &allElements = newTemp->GetAllElements();
	for (size_t i = 0; i < allElements.size(); ++i)
	{
		VJSONElement * curElement = allElements.at(i);
		MilString curStr = curElement->GetJSONString();
		MilString curNameStr = curElement->GetName();
		//CString toShow(curNameStr.c_str());
		//toShow += _T(" : ");
		//toShow += curStr.c_str();
		//AfxMessageBox(toShow);
	}
	return 0;
}

int VJSONObjectTemplateTest::TestAll()
{
	JsmnTest::test_passed = 0;
	JsmnTest::test_failed = 0;
	cout << "VJSONObjectTemplateTest >>\n";
	JsmnTest::test(TestConstructAndParse, "VJSONObjectTemplate::ConstructAndParse");
	cout << "PASSED: " << JsmnTest::test_passed << "\n" << "FAILED: " << JsmnTest::test_failed << "\n";
	cout << "<< VJSONObjectTemplateTest\n";
	return 0;
}