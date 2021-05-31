// DialogSelectObjectTemplate.cpp

#include "stdafx.h"
#include "DialogSelectObjectTemplate.h"
#include "VJSONObjectTemplate.h"
#include "VisualJSONDlg.h"

DialogSelectObjectTemplate::DialogSelectObjectTemplate(std::vector<VJSONObjectTemplate *> *allObjects, CVisualJSONDlg* pParent)	// standard constructor
	:CDialogEx(DialogSelectObjectTemplate::IDD, pParent), m_allObjects(allObjects), m_objecti(0), m_parentJSONDlg(pParent)
{

}

void DialogSelectObjectTemplate::DoDataExchange(CDataExchange* pDX)	// DDX/DDV support
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ALLOBJECTS, m_comboAllObjects);
}

BOOL DialogSelectObjectTemplate::OnInitDialog()
{
	try
	{
		BOOL ck = CDialogEx::OnInitDialog();
		RefillComboAllObjects();
		return ck;
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogSelectObjectTemplate::OnInitDialog exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
		return FALSE;
	}
}

void DialogSelectObjectTemplate::OnBnClickedOK()
{
	try
	{
		m_parentJSONDlg->OnCompleteInsertObjectTemplate();
		CDialogEx::OnOK();
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogSelectObjectTemplate::OnBnClickedOK exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

void DialogSelectObjectTemplate::OnBnClickedCancel()
{
	try
	{
		CDialogEx::OnCancel();
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogSelectObjectTemplate::OnBnClickedCancel exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

void DialogSelectObjectTemplate::OnCmbObjectSelChange()
{
	try
	{
		UpdateData();
		int curSel = m_comboAllObjects.GetCurSel();
		if (curSel != LB_ERR)
		{
			m_objecti = curSel;
		}
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogSelectObjectTemplate::OnCmbObjectSelChange exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

BEGIN_MESSAGE_MAP(DialogSelectObjectTemplate , CDialogEx)
	ON_BN_CLICKED(IDOK, OnBnClickedOK)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_ALLOBJECTS, OnCmbObjectSelChange)
END_MESSAGE_MAP()
	

void DialogSelectObjectTemplate::RefillComboAllObjects()
{ 
	try
	{
		m_comboAllObjects.ResetContent();
		size_t objSize = m_allObjects->size();
		for (size_t i = 0; i < objSize; ++i)
		{
			VJSONObjectTemplate *curObject = m_allObjects->at(i);
			m_comboAllObjects.InsertString(i, curObject->GetTemplateName().c_str());
		}
		if (objSize > m_objecti)
			m_comboAllObjects.SetCurSel(m_objecti);
		else
		{
			m_objecti = 0;
			m_comboAllObjects.SetCurSel(0);
		}
	}
	catch (exception &e)
	{
		CString typeStr("error");
		CString msgStr(e.what());
		PrintMessageManager::AddMessage(typeStr, msgStr);

	}
	catch (...)
	{
		CString typeStr("error");
		CString msgStr("DialogSelectObjectTemplate::RefillComboAllObjects");
		PrintMessageManager::AddMessage(typeStr, msgStr);
	}
}

BOOL DialogSelectObjectTemplate::ShowWindow(int nCmdShow)
{
	try
	{
		BOOL ck = CDialogEx::ShowWindow(nCmdShow);
		if (nCmdShow == SW_SHOW)
		{
			RefillComboAllObjects();
		}
		return ck;
	}
	catch (exception &e)
	{
		CString info("error");
		CString message(e.what());
		PrintMessageManager::AddMessage(info, message);
		return FALSE;
	}
	catch (...)
	{
		CString info("error");
		CString message("DialogSelectObjectTemplate::ShowWindow unknown exception");
		return FALSE;
	}
}

VJSONObjectTemplate * DialogSelectObjectTemplate::GetSelectedObject()
{
	try
	{
		VJSONObjectTemplate *selectedTemplate = NULL;
		if (m_allObjects->size() > m_objecti)
		{
			selectedTemplate = m_allObjects->at(m_objecti);
		}
		return selectedTemplate;
	}
	catch (exception &e)
	{
		CString typeStr("error");
		CString messageStr(e.what());
		PrintMessageManager::AddMessage(typeStr, messageStr);
		return NULL;
	}
}
