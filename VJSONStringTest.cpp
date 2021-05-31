// VJSONStringTest.cpp

#include "stdafx.h"
#include "VJSONStringTest.h"
#include"jsmn_test.h"
#include "VJSONString.h"

int VJSONStringTest::Test(VJSONString *toTest , const MilString &strContent , bool isKey)
{
	check(toTest->GetJSONString() == strContent);
	check(toTest->GetIsKeyNameStr() == isKey);
	return 0;
}