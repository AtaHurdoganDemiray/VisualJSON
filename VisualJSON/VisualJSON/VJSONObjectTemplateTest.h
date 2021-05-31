// VJSONObjectTemplateTest.h

#ifndef __VJSONObjectTemplateTest_h__
#define __VJSONObjectTemplateTest_h__

class VJSONObjectTemplate;
class VJSONElement;

class VJSONObjectTemplateTest
{
public:
	static int TestAll();
	static int TestConstructAndParse();
	static VJSONObjectTemplate * VJSONObjectTemplateTest::MakeVJSONObjectTemplateByString(MilString *str, const MilString &templateName , const MilString &fileName);// str should stay on memory , it will be used during life time of VJSONObjectTemplate
	static VJSONObjectTemplate * VJSONObjectTemplateTest::MakeEmptyVJSONObjectTemplate(const MilString &templateName);
#ifdef _DEBUG
	static std::map<VJSONElement *, VJSONElement *> m_memoryLeakCheckMap;
	static void AddToMemoryLeakCheckMap(VJSONElement *el);
#endif
};

#endif