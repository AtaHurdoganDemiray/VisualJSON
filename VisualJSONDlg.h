
// VisualJSONDlg.h : header file
//

#pragma once

#include  "VJSONElement.h"
#include "VisualJSONSettings.h"

class DialogKeyValue;
class VJSONObjectTemplate;
class VJSONString;
class DialogSelectObjectTemplate;
class DialogKeyValue;
// CVisualJSONDlg dialog

class CVisualJSONDlg : public CDialogEx
{
// Construction
public:
	CVisualJSONDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VISUALJSON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedMakeObject();
	afx_msg void OnBnClickedDeleteObject();
	afx_msg void OnBnClickedAddExistingObjectTemplate();
	afx_msg void OnBnClickedMakeKeyValue();
	afx_msg void OnBnClickedExportJSON();
	afx_msg void OnBnClickedImportJSON();
	afx_msg void OnBnClickedTest();
	afx_msg void OnCbnSelChangeCurObjTemplate();
	DECLARE_MESSAGE_MAP()

	CTreeCtrl m_treeForObject;
	CListCtrl m_listMessages;
	CComboBox m_comboObjects;
	VJSONObjectTemplate * m_curObjTemplate;
	int m_radioAddUnderOrAfter;
	vector<VJSONObjectTemplate *> m_allObjectTemplates;
	DialogSelectObjectTemplate *m_dialSelectObjTemplate;
	DialogKeyValue *m_dialKeyVal;
	CString m_importFilePathStr;
	CString m_exportFilePathStr;
public:
	CListCtrl & GetListMessages();
	void AddObjectToTree(DialogKeyValue &dialKeyVal);
	void MakeNewObject(const MilString &keyStr, MilString &valStr);
	void RefillTreeJSONObjectTemplate();
	void RefillComboObjects();
private:
	HTREEITEM AddToTreeRecursive(VJSONElement *parent, VJSONElement *toAdd, HTREEITEM &parentHti);
	HTREEITEM AddToTreeRecursiveKeyValPair(VJSONElement *parent, VJSONString *keyToAdd, VJSONElement *valToAdd, HTREEITEM &parentHti);
	HTREEITEM AddToTreeRecursiveNoKeyOnlyVal(VJSONElement *parent, VJSONElement *valToAdd, HTREEITEM &parentHti);
	afx_msg void OnBnClickedDeleteElement();
	afx_msg void OnBnClickedMoveUp();
	afx_msg void OnBnClickedMoveDown();
	afx_msg void OnBnClickedSelectImportFilePath();
	afx_msg void OnBnClickedSelectExportFilePath();
	afx_msg void OnBnClickedEditSelectedElement();
	afx_msg void OnBnClickedSettings();
public:
	void OnCompleteInsertObjectTemplate();
	void OnCompleteAddElement();
	void DoInsertStringToCurTemplate(const MilString &name, const MilString &toInsert, VJSONElement::ElType valType);
	void DoInsertStringToCurTemplateFromAnotherTemplate(const MilString &name, const MilString &toInsert, VJSONElement::ElType valType);
	void PreventDublicateTemplateNameIf();
	void UpdateKeyNameIf(VJSONElement *bigParent, MilString &newKeyName);
	void OnCompleteEditElement();
	void LoadSettingsIf();
	VisualJSONSettings m_settings;
};
