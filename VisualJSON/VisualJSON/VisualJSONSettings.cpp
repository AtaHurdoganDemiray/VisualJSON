// VisualJSONSettings.cpp

#include "stdafx.h"
#include "VisualJSONSettings.h"
#include "VJSONObjectTemplate.h"
#include "VJSONElement.h"
#include "VJSONObject.h"
#include "VJSONString.h"

VisualJSONSettings::VisualJSONSettings(VJSONObjectTemplate *settingsObject)
	:m_settingsObject(settingsObject)
{

}
void VisualJSONSettings::SetObject(VJSONObjectTemplate *settingsObject)
{
	m_settingsObject = settingsObject;
}
VJSONObject * VisualJSONSettings::SearchParameterSet(const MilString &categoryName)const
{
	if (m_settingsObject == NULL)
		throw exception("VisualJSONSettings settings object is NULL");
	list<VJSONObject *> objectsByName = m_settingsObject->SearchObjectsByName(categoryName);
	if (objectsByName.size() != 1)
	{
		MilStringStream mystr;
		mystr << "VisualJSONSettings::SearchParameterSet exception,category name:" <<categoryName<<" object count:"<<objectsByName.size();
		MilString myerr(mystr.str());
		throw exception(myerr.c_str());
	}
	else
		return *objectsByName.begin();
}

VJSONElement * VisualJSONSettings::SearchParameter(VJSONObject *parameterSet, const MilString &parameterKey)const
{
	if (m_settingsObject == NULL)
		throw exception("VisualJSONSettings settings object is NULL");
	std::list<VJSONElement *> allChildren;
	parameterSet->CollectAllChildrenRecursive(allChildren);
	std::list<VJSONElement *> resultEnt;
	std::list<VJSONElement *>::iterator i = allChildren.begin();
	std::list<VJSONElement *>::iterator e = allChildren.end();
	for (i; i != e; ++i)
	{
		VJSONElement *curEl = (*i);
		if (curEl->GetName() == parameterKey)
		{
			resultEnt.push_back(curEl);
		}
	}
	if (resultEnt.size() != 1)
	{
		MilStringStream mystr;
		mystr << "VisualJSONSettings::SearchParameter exception,parameter name:"<<parameterKey<<" parameter count:"<<resultEnt.size();
		MilString myerr(mystr.str());
		throw exception(myerr.c_str());
	}
	else
		return *resultEnt.begin();

}

void VisualJSONSettings::OpenFileWithEditor(const MilString &filePathToOpen)const
{
	MilString editorPath("C:\\Windows\\Notepad.exe");
	try
	{
		MilString category("General");
		VJSONObject * generalParametersObj = SearchParameterSet(category);
		if (generalParametersObj != NULL)
		{
			MilString parKey("DefaultEditorExePath");
			VJSONElement *defaultEditorExePathElement = SearchParameter(generalParametersObj, parKey);
			editorPath = defaultEditorExePathElement->GetJSONString();
		}
	}
	catch (exception &e)
	{
		PrintMessageManager::AddMessage(CString("error"), CString(e.what()));
	}
	catch (...)
	{
		PrintMessageManager::AddMessage(CString("error"),CString("VisualJSONSettings::OpenFileWithEditor unknown exception"));
	}
	HINSTANCE ck = ShellExecute(0, 0, editorPath.c_str(), filePathToOpen.c_str(), 0, SW_SHOW);
}

MilString VisualJSONSettings::GetAutomatedTestOutputFolder()const
{
	MilString outFolder("C:\\Test\\");
	try
	{
		MilString category("AutomatedTestParameters");
		VJSONObject * testParObj = SearchParameterSet(category);
		if (testParObj != NULL)
		{
			MilString parKey("TestBaseFolder");
			VJSONElement *foldEl = SearchParameter(testParObj, parKey);
			if (foldEl != NULL)
				outFolder = foldEl->GetJSONString();
		}
	}
	catch (exception &e)
	{
		PrintMessageManager::AddMessage(CString("error"), CString(e.what()));
	}
	catch (...)
	{
		PrintMessageManager::AddMessage(CString("error"), CString("VisualJSONSettings::GetAutomatedTestOutputFolder unknown exception"));
	}
	return outFolder;
}
