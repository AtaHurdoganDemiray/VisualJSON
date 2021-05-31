// DialogSelectObjectTemplate.h

#ifndef __DialogSelectObjectTemplate_h__
#define __DialogSelectObjectTemplate_h__

#include "resource.h"
class VJSONObjectTemplate;
class CVisualJSONDlg;

class DialogSelectObjectTemplate : public CDialogEx
{
	// Construction
public:
	DialogSelectObjectTemplate(std::vector<VJSONObjectTemplate *> *allObjects , CVisualJSONDlg *parent);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_DIALOG_SELECTOBJECTTEMPLATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCmbObjectSelChange();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboAllObjects;
	size_t m_objecti;
	std::vector<VJSONObjectTemplate *> *m_allObjects;
	CVisualJSONDlg *m_parentJSONDlg;
public:
	void RefillComboAllObjects();
	VJSONObjectTemplate * GetSelectedObject();
	virtual BOOL ShowWindow(int nCmdShow);
};

#endif