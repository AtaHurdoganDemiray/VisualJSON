// DialogKeyValue.h

#ifndef __DialogKeyValue_h__
#define __DialogKeyValue_h__

#include "resource.h"
#include "VJSONElement.h"
class CVisualJSONDlg;

class DialogKeyValue : public CDialogEx
{
	// Construction
public:
	DialogKeyValue(CVisualJSONDlg* pParent);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_DIALOG_KEYVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCmbValueTypesSelChange();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboValueTypes;
	int m_valueTypei;
	CString m_keyName;
	CString m_defaultValue;
	CString m_minValue;
	CString m_maxValue;
	CString m_enumListStr;
	CEdit m_enumSetEdit;
	CVisualJSONDlg *m_parentJSONDlg;
public:
	void RefillComboValueTypes();
	void ShowOrHideEnumSetEdit();
	VJSONElement::ElType GetSelectedElType()const;
	void ReinitForEditElement(VJSONElement *editKeyStr, VJSONElement * toEdit);
private:	
	VJSONElement *m_selEl;
public:
	VJSONElement * GetSelElAndMakeItNull();
};

#endif