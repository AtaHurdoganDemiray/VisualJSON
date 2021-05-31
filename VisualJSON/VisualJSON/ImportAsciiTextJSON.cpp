//  ImportAsciiTextJSON.cpp

#include "stdafx.h"
#include "ImportAsciiTextJSON.h"
#include "VJSONObject.h"
#include "VJSONArray.h"
#include "VJSONSingleKeyVal.h"
#include "VJSONBasicVal.h"
#include "VJSONObjectTemplate.h"
#include "MilPathStringFunctions.h"

ImportAsciiTextJSON::ImportAsciiTextJSON(const MilString &fileName)//,const MilString &rootObjectName)
	:m_importResult(NULL)
{
	try
	{
		MilIFStream f(fileName);
		MilString templateName = MilPathStringFunctions::GetJustFileName(fileName);
		templateName = MilPathStringFunctions::TrimExtension(templateName);
		//int myCounter = VJSONObjectTemplate::GetNameCountSoFarGlobal(templateName);
		//MilStringStream strstr;
		//strstr << templateName << '(' << myCounter << ')';
		//templateName = strstr.str();

		MilString * mystr = new MilString;
		char buff[VJSONMAXCHARSIZEFORLINE];
		if (f.is_open())
		{
			while (f.eof() == false)
			{
				f.getline(buff, VJSONMAXCHARSIZEFORLINE);
				(*mystr) += buff;
			}
		}
		//m_importResult = _T("Successfull");
		CString typeStr(_T("info"));
		CString messageStr;
		messageStr.Format(_T("%s successfully read from file"), fileName.c_str());
		PrintMessageManager::AddMessage(typeStr, messageStr);

		VJSONObjectTemplate *newObjTemplate = new VJSONObjectTemplate();
		VJSONObject *templateRoot = new VJSONObject(newObjTemplate);
		newObjTemplate->ConstructAndParse(templateRoot, templateName , fileName, mystr);
		m_importResult = newObjTemplate;
		f.close();
	}
	catch (...)
	{
		CString info(_T("error"));
		CString message(_T("ImportAsciiTextJSON unknown exception!"));
		PrintMessageManager::AddMessage(info, message);
	}
}

VJSONObjectTemplate * ImportAsciiTextJSON::GetImportResult()const
{
	return m_importResult;
}