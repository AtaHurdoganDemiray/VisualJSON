// TestRunner.cpp

#include "stdafx.h"
#include "TestRunner.h"
#include "jsmn_test.h"
#include "VJSONElementTest.h"
#include "VJSONBasicValTest.h"
#include "VJSONStringTest.h"
#include "VJSONArrayTest.h"
#include "VJSONObjectTest.h"
#include "VJSONObjectTemplateTest.h"

void TestRunner::RunAllTests(MilIFStream &inf, MilOFStream &of)
{
	std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
	std::cin.rdbuf(inf.rdbuf()); //redirect std::cin to in.txt!
	
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(of.rdbuf()); //redirect std::cout to out.txt!

	JsmnTest::TestAll();
	VJSONObjectTemplateTest::TestAll();
	VJSONObjectTest::TestAll();

	std::cin.rdbuf(cinbuf);   //reset to standard input again
	std::cout.rdbuf(coutbuf); //reset to standard output again
}