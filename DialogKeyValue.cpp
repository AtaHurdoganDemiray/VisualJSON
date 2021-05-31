// DialogKeyValue.cpp

#include "stdafx.h"
#include "DialogKeyValue.h"
#include "resource.h"
#include "VisualJSONDlg.h"
#include "VJSONString.h"

DialogKeyValue::DialogKeyValue(CVisualJSONDlg* pParent)	// standard constructor
	:CDialogEx(DialogKeyValue::IDD, pParent), m_parentJSONDlg(pParent), m_selEl(NULL)
{
	m_valueTypei = 0;
	m_keyName = _T("mykey");
	m_defaultValue = _T("0.0");
	m_minValue = _T("0.0");
	m_maxValue = _T("1000000");
	m_enumListStr = _T("linear;rotary");
}

void DialogKeyValue::DoDataExchange(CDataExchange* pDX)	// DDX/DDV support
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEYNAME, m_keyName);
	DDX_Text(pDX, IDC_EDIT_DEFAULTVALUE, m_defaultValue);
	DDX_Text(pDX, IDC_EDIT_MINVALUE, m_minValue);
	DDX_Text(pDX, IDC_EDIT_MAXVALUE, m_maxValue);
	DDX_Control(pDX, IDC_EDIT_ENUMSET, m_enumSetEdit);
	DDX_Control(pDX, IDC_COMBO_VALUETYPE, m_comboValueTypes);
}

BEGIN_MESSAGE_MAP(DialogKeyValue, CDialogEx)
	ON_BN_CLICKED(IDOK , OnBnClickedOK)
	ON_BN_CLICKED(IDCANCEL , OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_VALUETYPE , OnCmbValueTypesSelChange)
END_MESSAGE_MAP()

BOOL DialogKeyValue::OnInitDialog()
{
	try
	{
		BOOL ck = CDialogEx::OnInitDialog();
		RefillComboValueTypes();
		return ck;
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogKeyValue::OnInitDialog exception"));
		PrintMessageManager::AddMessage(typeStr,messageStr);
		return FALSE;
	}
}

void DialogKeyValue::OnBnClickedOK()
{
	try
	{
		UpdateData();
		if (m_selEl == NULL)
			m_parentJSONDlg->OnCompleteAddElement();
		else
			m_parentJSONDlg->OnCompleteEditElement();
		CDialogEx::OnOK();
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogKeyValue::OnBnClickedOK exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

void DialogKeyValue::OnBnClickedCancel()
{
	try
	{
		m_selEl = NULL;
		CDialogEx::OnCancel();
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogKeyValue::OnBnClickedCancel exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

void DialogKeyValue::OnCmbValueTypesSelChange()
{
	try
	{
		UpdateData();
		int curSel = m_comboValueTypes.GetCurSel();
		if (curSel != LB_ERR)
			m_valueTypei = curSel;
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("DialogKeyValue::OnCmbValueTypesSelChange exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}
	
void DialogKeyValue::RefillComboValueTypes()
{
	m_comboValueTypes.ResetContent();
	m_comboValueTypes.AddString(_T("double"));
	m_comboValueTypes.AddString(_T("int"));
	m_comboValueTypes.AddString(_T("string"));
	m_comboValueTypes.AddString(_T("array"));
	m_comboValueTypes.AddString(_T("object"));
	m_comboValueTypes.SetCurSel(0);
}
VJSONElement::ElType DialogKeyValue::GetSelectedElType()const
{
	VJSONElement::ElType mytyp(VJSONElement::ElTypeBasic);
	//if (comboSel == LB_ERR)
	//{
	//	CString info("error");
	//	CString mess("DialogKeyValue::GetSelectedElType error , combo cur sel is lb_err");
	//	PrintMessageManager::AddMessage(info, mess);
	//}
	switch (m_valueTypei)
	{
	case 2:
		mytyp = VJSONElement::ElTypeString;
		break;
	case 3:
		mytyp = VJSONElement::ElTypeArray;
		break;
	case 4:
		mytyp = VJSONElement::ElTypeObject;
		break;
	}
	return mytyp;
}
void DialogKeyValue::ShowOrHideEnumSetEdit()
{

}

void DialogKeyValue::ReinitForEditElement(VJSONElement *editKeyStr, VJSONElement * toEdit)
{
	try
	{
		m_selEl = toEdit;
		UpdateData();
		MilString keyName("");
		if (editKeyStr != NULL)
		{
			keyName = ((VJSONString *)editKeyStr)->GetJSONString();
		}
		m_keyName = keyName.c_str();
		MilString defaultValue = VJSONElement::RemoveWhiteSpaceOutOfQuotations(toEdit->GetJSONString());
		m_defaultValue = defaultValue.c_str();
		UpdateData(FALSE);
		switch (toEdit->GetElType())
		{
		case VJSONElement::ElTypeObject:
			m_comboValueTypes.SetCurSel(4);
			break;
		case VJSONElement::ElTypeArray:
			m_comboValueTypes.SetCurSel(3);
			break;
		case VJSONElement::ElTypeString:
			m_comboValueTypes.SetCurSel(2);
			break;
		default:
			m_comboValueTypes.SetCurSel(0);
			break;
		}
	}
	catch (exception &e)
	{
		CString info("error");
		CString messageStr(e.what());
		PrintMessageManager::AddMessage(info, messageStr);
		
	}
	catch (...)
	{
		CString info("error");
		CString messageStr("DialogKeyValue::ReinitForEditElement unknown exception");
		PrintMessageManager::AddMessage(info, messageStr);
	}
}

VJSONElement * DialogKeyValue::GetSelElAndMakeItNull()
{
	VJSONElement *tmp = m_selEl;
	m_selEl = NULL;
	return tmp;
}
