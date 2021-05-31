
// VisualJSONDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisualJSON.h"
#include "VisualJSONDlg.h"
#include "afxdialogex.h"
#include "DialogKeyValue.h"
#include "SimpleInputDialog.h"
#include "VJSONBasicValTest.h"
#include "ImportAsciiTextJSON.h"
#include "TestRunner.h"
#include "VJSONObjectTemplate.h"
#include "VJSONObject.h"
#include "VJSONArray.h"
#include "VJSONString.h"
#include "VJSONBasicVal.h"
#include "ExportAsciiTextJSON.h"
#include "VJSONObjectTest.h"
#include "DialogSelectObjectTemplate.h"
#include "VJSONObjectTemplateTest.h"
#include "MilPathStringFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVisualJSONDlg dialog



CVisualJSONDlg::CVisualJSONDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisualJSONDlg::IDD, pParent), m_settings(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_curObjTemplate = NULL;
	m_radioAddUnderOrAfter = 0;
	m_dialSelectObjTemplate = new DialogSelectObjectTemplate(&m_allObjectTemplates, this);
	m_dialSelectObjTemplate->Create(DialogSelectObjectTemplate::IDD,this);
	m_dialKeyVal = new DialogKeyValue(this);
	m_dialKeyVal->Create(DialogKeyValue::IDD, this);
}

void CVisualJSONDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OBJECT, m_treeForObject);
	DDX_Control(pDX, IDC_COMBO_OBJECTS, m_comboObjects);
	DDX_Control(pDX, IDC_LIST_MESSAGES, m_listMessages);
	DDX_Radio(pDX, IDC_RADIO_ADDASCHILD, m_radioAddUnderOrAfter);
	DDX_Text(pDX, IDC_EDIT_IMPORTFILEPATH, m_importFilePathStr);
	DDX_Text(pDX, IDC_EDIT_EXPORTFILEPATH, m_exportFilePathStr);
}

BEGIN_MESSAGE_MAP(CVisualJSONDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MAKENEWOBJECT, OnBnClickedMakeObject)
	ON_BN_CLICKED(IDC_BUTTON_MAKEKEYVALUE, OnBnClickedMakeKeyValue)
	ON_BN_CLICKED(IDC_BUTTON_DELETEOBJECT, OnBnClickedDeleteObject)
	ON_BN_CLICKED(IDC_BUTTON_ADDEXISTINGOBJECT, OnBnClickedAddExistingObjectTemplate)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTJSON, OnBnClickedImportJSON)
	ON_BN_CLICKED(IDC_BUTTON_EXPORTOBJECTASJSON, OnBnClickedExportJSON)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnBnClickedTest)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJECTS, OnCbnSelChangeCurObjTemplate)
	ON_BN_CLICKED(IDC_BUTTON_DELETEELEMENT , OnBnClickedDeleteElement)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUPP , OnBnClickedMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN , OnBnClickedMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_SELECTIMPORTFILEPATH,OnBnClickedSelectImportFilePath)
	ON_BN_CLICKED(IDC_BUTTON_SELECTEXPORTFILEPATH , OnBnClickedSelectExportFilePath)
	ON_BN_CLICKED(IDC_BUTTON_EDITELEMENT , OnBnClickedEditSelectedElement)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS , OnBnClickedSettings)
END_MESSAGE_MAP()

// CVisualJSONDlg message handlers

BOOL CVisualJSONDlg::OnInitDialog()
{
	try
	{
		CDialogEx::OnInitDialog();

		// Add "About..." menu item to system menu.

		// IDM_ABOUTBOX must be in the system command range.
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			BOOL bNameValid;
			CString strAboutMenu;
			bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
			ASSERT(bNameValid);
			if (!strAboutMenu.IsEmpty())
			{
				pSysMenu->AppendMenu(MF_SEPARATOR);
				pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			}
		}

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here

		m_listMessages.SetExtendedStyle(/*LVS_EX_CHECKBOXES |*/m_listMessages.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		//m_listParts.ModifyStyle  (0,LVS_EX_CHECKBOXES);
		m_listMessages.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 100);
		m_listMessages.InsertColumn(1, _T("Message"), LVCFMT_LEFT, 500);
		m_listMessages.InsertColumn(2, _T("Time"), LVCFMT_LEFT, 100);

		PrintMessageManager::m_listMessages = (&m_listMessages);
		CString typeStr(_T("info"));
		CString messageStr(_T("Page initialised"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
		//m_treeForObject.InsertItem(_T("Active object"));
		// Object Tree Style
		m_treeForObject.ModifyStyle(0, WS_CHILD | WS_VISIBLE |
			TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT |
			TVS_EDITLABELS | TVS_SHOWSELALWAYS); //| TVS_EX_MULTISELECT);  not supported don't use
		m_treeForObject.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		m_treeForObject.ModifyStyle(0, TVS_CHECKBOXES);

		// Load settings if exist
		LoadSettingsIf();
		//

		return TRUE;  // return TRUE  unless you set the focus to a control
	}
	catch (exception &e)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnInitDialog exception catch : "));
		messageStr += e.what();
		PrintMessageManager::AddMessage(typeStr, messageStr);
		return TRUE;
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnInitDialog unknown exception"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
		return TRUE;
	}
}

void CVisualJSONDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisualJSONDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisualJSONDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVisualJSONDlg::OnBnClickedMakeObject()
{
	try
	{
		CString keyStr(_T("Object name"));
		CString defaultValStr(_T("obj1"));
		SimpleInputDialog mysimpleDial(keyStr, defaultValStr, this);
		if (mysimpleDial.DoModal() == IDOK)
		{
			MilString *dummyObjStr = new MilString("{\"dummyParameter\":\"abc\"}");
			MakeNewObject(MilString(mysimpleDial.m_valStr), *dummyObjStr);
		}
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedMakeObject exception catch"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}
void CVisualJSONDlg::OnBnClickedDeleteObject()
{
	try
	{
		if (m_curObjTemplate == NULL)
			throw exception("CVisualJSONDlg::OnBnClickedDeleteObject exception , no object template is selected");
		if (AfxMessageBox("Are you sure to delete ?", MB_YESNO) == IDYES)
		{
			std::vector<VJSONObjectTemplate *> newVec;
			size_t mysize = m_allObjectTemplates.size();
			for (size_t i = 0; i < mysize; ++i)
			{
				VJSONObjectTemplate *curTemplate = m_allObjectTemplates.at(i);
				if (curTemplate != m_curObjTemplate)
					newVec.push_back(curTemplate);
			}
			m_allObjectTemplates = newVec;
			RefillComboObjects();
			RefillTreeJSONObjectTemplate();
		}
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedDeleteObject exception catch"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}
void CVisualJSONDlg::OnBnClickedAddExistingObjectTemplate()
{
	try
	{
		//AfxMessageBox(_T("Not implemented yet"));
		m_dialSelectObjTemplate->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedAddExistingObjectTemplate exception catch"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}
void CVisualJSONDlg::OnBnClickedMakeKeyValue()
{
	try
	{
		m_dialKeyVal->ShowWindow(SW_SHOW);
	}
	catch (exception &e)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T(e.what()));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedMakeCategory exception catch"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}
void CVisualJSONDlg::OnBnClickedExportJSON()
{
	try
	{
		if (m_curObjTemplate != NULL)
		{
			//CFileDialog mydial(FALSE);
			//if (mydial.DoModal() == IDOK)
			//{
			MilString fileName = m_exportFilePathStr;//mydial.m_ofn.lpstrFile;
			ExportAsciiTextJSON(m_curObjTemplate,fileName);
			//}
			//MilString editorPath("C:\\Windows\\Notepad.exe");
			//HINSTANCE ck = ShellExecute(0, 0, editorPath.c_str(), fileName.c_str(), 0, SW_SHOW);
			m_settings.OpenFileWithEditor(fileName);
		}
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedExportJSON exception catch"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}
void CVisualJSONDlg::PreventDublicateTemplateNameIf()
{
	int mycount = VJSONObjectTemplate::GetNameCountSoFarGlobal(m_curObjTemplate->GetTemplateName());
	MilString templateName = m_curObjTemplate->GetTemplateName();

	while (mycount > 1)
	{
		MilStringStream strstr;
		strstr << templateName << '(' << mycount << ')';
		templateName = strstr.str();
		CString arecomendation(templateName.c_str());
		SimpleInputDialog mydial(CString("There is name collision. Please rename:"),arecomendation);
		if (mydial.DoModal() == IDCANCEL)
			return;
		templateName = mydial.m_valStr;
		if (templateName != "")
		{
			m_curObjTemplate->SetTemplateName(templateName);
			mycount = VJSONObjectTemplate::GetNameCountSoFarGlobal(templateName);
		}
		else
		{
			templateName = arecomendation;
		}
	}
}
void CVisualJSONDlg::OnBnClickedImportJSON()
{
	try
	{
		//CFileDialog mydial(TRUE);
		//if (mydial.DoModal() == IDOK)
		//{
			MilString fileName = m_importFilePathStr;//mydial.m_ofn.lpstrFile;
			if (fileName == "")
				throw exception("CVisualJSONDlg::OnBnClickedImportJSON exception , filename is empty");
			//MilString objName = fileName;
			ImportAsciiTextJSON myparser(fileName);// , objName);
			m_curObjTemplate = myparser.GetImportResult();
			if (m_curObjTemplate == NULL)
				throw exception("CVisualJSONDlg::OnBnClickedImportJSON exception , null import result");
			PreventDublicateTemplateNameIf();
			m_allObjectTemplates.push_back(m_curObjTemplate);
			RefillComboObjects();
			RefillTreeJSONObjectTemplate();
		//}
	}
	catch (exception &e)
	{
		CString typeStr(_T("error"));
		CString messageStr(e.what());
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedImportJSON exception catch"));
		PrintMessageManager::AddMessage(typeStr, messageStr);
	}
}

CListCtrl & CVisualJSONDlg::GetListMessages()
{
	return m_listMessages;
}

void CVisualJSONDlg::AddObjectToTree(DialogKeyValue &dialKeyVal)
{
	CString itemNameStr = dialKeyVal.m_keyName;

	VJSONObjectTemplate *newObjTemplate = new VJSONObjectTemplate();
	VJSONObject *templateRoot = new VJSONObject(newObjTemplate);
	newObjTemplate->SetTemplateName(_T("123"));
	m_curObjTemplate = newObjTemplate;
	m_allObjectTemplates.push_back(newObjTemplate);
	RefillComboObjects();
	RefillTreeJSONObjectTemplate();
	
}

void CVisualJSONDlg::MakeNewObject(const MilString &keyStr, MilString &valStr)
{
	MilString templateName(keyStr);
	MilString fileName("");// file name for the source , it should be read before and loaded to string (here VJSONTESTSTR is used for construction)
	VJSONObjectTemplate *newTemp = VJSONObjectTemplateTest::MakeVJSONObjectTemplateByString(&valStr ,keyStr, fileName);
	m_curObjTemplate = newTemp;
	if (m_curObjTemplate == NULL)
		throw exception("CVisualJSONDlg::MakeNewObject exception , dummy template is null");
	PreventDublicateTemplateNameIf();
	m_allObjectTemplates.push_back(m_curObjTemplate);
	RefillComboObjects();
	RefillTreeJSONObjectTemplate();
}

void CVisualJSONDlg::OnBnClickedTest()
{
	try
	{
		MilStringStream timestampstr;
		CTime mytimer = CTime::GetCurrentTime();
		timestampstr << mytimer.GetYear() << "_" << mytimer.GetMonth() << "_" << mytimer.GetDay() << "_" << mytimer.GetHour() << "_" << mytimer.GetMinute();
		MilString timestr(timestampstr.str());
		MilString instr(m_settings.GetAutomatedTestOutputFolder()+MilString("in_"));
		MilString outstr(m_settings.GetAutomatedTestOutputFolder()+"out_");
		MilString extstr(".txt");
		MilIFStream in(instr + timestr + extstr);
		MilString outStrAppended(outstr + timestr + extstr);
		MilOFStream out( outStrAppended , std::ios::app);
		TestRunner::RunAllTests(in,out);
		in.close();
		out.close();
#ifdef _DEBUG
		if (VJSONObjectTest::m_testProduct != NULL)
		{
			m_curObjTemplate = VJSONObjectTest::m_testProduct;
			m_allObjectTemplates.push_back(m_curObjTemplate);
			RefillComboObjects();
			RefillTreeJSONObjectTemplate();
		}
#endif

		//MilString editorPath("C:\\Windows\\Notepad.exe");
		//HINSTANCE ck = ShellExecute(0, 0, editorPath.c_str() , outStrAppended.c_str() , 0 , SW_SHOW);
		m_settings.OpenFileWithEditor(outStrAppended);
	}
	catch (...)
	{
		CString typeStr(_T("error"));
		CString messageStr(_T("CVisualJSONDialog::OnBnClickedTest exception catch"));
		PrintMessageManager::AddMessage(typeStr,messageStr);
	}

}

void CVisualJSONDlg::RefillTreeJSONObjectTemplate()
{
	m_treeForObject.DeleteAllItems();
	if (m_curObjTemplate != NULL)
	{
		VJSONObject *templateRoot = m_curObjTemplate->GetTemplateRootJSONObject();
		VJSONElement *firstChild = templateRoot->GetFirstChild();
		if (firstChild != NULL)
		{
			CString itemNameStr = m_curObjTemplate->GetTemplateName().c_str();
			HTREEITEM parentHti = m_treeForObject.GetFirstVisibleItem();
			parentHti = m_treeForObject.InsertItem(itemNameStr, parentHti);//, brotherHtri);
			m_treeForObject.SetItemData(parentHti, (DWORD_PTR)templateRoot);
			HTREEITEM curhti = AddToTreeRecursive(templateRoot, firstChild, parentHti);
		}
	}
}
HTREEITEM CVisualJSONDlg::AddToTreeRecursiveKeyValPair(VJSONElement *parent , VJSONString *keyToAdd , VJSONElement *valToAdd , HTREEITEM &parentHti)
{
	MilString keystr = keyToAdd->GetJSONString();
	VJSONElement::ElType valType = valToAdd->GetElType();
	VJSONElement *firstChild = valToAdd->GetFirstChild();
	VJSONElement *nextEl = valToAdd->GetNext();
	switch (valType)
	{
	case VJSONElement::ElTypeObject:
		keystr += "(Object)";
		break;
	case VJSONElement::ElTypeArray:
		keystr += "(Array)";
		break;
	case VJSONElement::ElTypeString:
		keystr += " : ";
		keystr += valToAdd->GetJSONString();
		break;
	case VJSONElement::ElTypeBasic:
		keystr += " : ";
		keystr += valToAdd->GetJSONString();
		break;
	}
	HTREEITEM newHti = m_treeForObject.InsertItem(keystr.c_str(), parentHti);
	m_treeForObject.SetItemData(newHti, (DWORD_PTR)valToAdd);
	if (firstChild != NULL)
		AddToTreeRecursive(valToAdd, firstChild, newHti);
	if (nextEl != NULL)
		AddToTreeRecursive(parent, nextEl, parentHti);
	return newHti;
}
HTREEITEM CVisualJSONDlg::AddToTreeRecursiveNoKeyOnlyVal(VJSONElement *parent, VJSONElement *valToAdd, HTREEITEM &parentHti)
{
	MilString keystr;
	VJSONElement::ElType valType = valToAdd->GetElType();
	VJSONElement *firstChild = valToAdd->GetFirstChild();
	VJSONElement *nextEl = valToAdd->GetNext();
	switch (valType)
	{
	case VJSONElement::ElTypeObject:
		keystr += "Untitled(Object)";
		break;
	case VJSONElement::ElTypeArray:
		keystr += "Untitled(Array)";
		break;
	case VJSONElement::ElTypeString:
		keystr += valToAdd->GetJSONString();
		break;
	case VJSONElement::ElTypeBasic:
		keystr += valToAdd->GetJSONString();
		break;
	}
	HTREEITEM newHti = m_treeForObject.InsertItem(keystr.c_str(), parentHti);
	m_treeForObject.SetItemData(newHti, (DWORD_PTR)valToAdd);
	if (firstChild != NULL)
		AddToTreeRecursive(valToAdd, firstChild, newHti);
	if (nextEl != NULL)
		AddToTreeRecursive(parent, nextEl, parentHti);
	return newHti;
}
HTREEITEM CVisualJSONDlg::AddToTreeRecursive(VJSONElement *parent, VJSONElement *toAdd, HTREEITEM &parentHti)
{
	if (parentHti == NULL)
		return NULL;
	VJSONString *curstring = NULL;
	VJSONElement::ElType etype = toAdd->GetElType();
	switch (etype)
	{
	case VJSONElement::ElTypeString:
		curstring = (VJSONString *)toAdd;
		if (curstring->GetIsKeyNameStr() == true)
		{
			VJSONElement *nextItem = curstring->GetNext();
			if (nextItem == NULL)
				throw exception("CVisualJSONDlg::AddToTreeRecursive exception , next element should not be NULL");
			return AddToTreeRecursiveKeyValPair(parent, curstring , nextItem, parentHti);
		}
		else
		{
			return AddToTreeRecursiveNoKeyOnlyVal(parent,toAdd,parentHti);
		}

		break;
	case VJSONElement::ElTypeBasic:
		return AddToTreeRecursiveNoKeyOnlyVal(parent,toAdd,parentHti);
		break;
	case VJSONElement::ElTypeArray:
		return AddToTreeRecursiveNoKeyOnlyVal(parent, toAdd, parentHti);
		break;
	case VJSONElement::ElTypeObject:
		return AddToTreeRecursiveNoKeyOnlyVal(parent, toAdd, parentHti);
		break;
	default:
		break;
	};
	return NULL;
	//MilString entityNameStr = toAdd->GetName().c_str();
	//if (entityNameStr == "")
	//	entityNameStr = toAdd->GetElTypeName();
	////if (entityName == "")
	////	entityName = _T("O");
	////int nameCounter = m_curObjTemplate->GetNameCountSoFar(entityNameStr);
	//CString entityName(entityNameStr.c_str());
	////entityName.Format("%s(%d)",entityNameStr.c_str(),nameCounter);
	//HTREEITEM curhti = m_treeForObject.InsertItem(entityName,parentHti);

	//// Add all its children
	//VJSONElement *curChildElement = toAdd->GetFirstChild();
	///*while*/if (curChildElement != NULL)
	//{
	//	HTREEITEM subcurhti1 = curhti;
	//	bool doAdd = false;
	//	while (doAdd == false && curChildElement != NULL)
	//	{
	//		if (curChildElement->GetElType() == VJSONElement::ElTypeString)
	//		{
	//			VJSONString * curString = (VJSONString *)curChildElement;
	//			if (curString->GetIsKeyNameStr() == true)
	//				doAdd = false;
	//			else
	//				doAdd = true;
	//		}
	//		else
	//			doAdd = true;
	//		if (doAdd)
	//		{
	//			subcurhti1 = AddToTreeRecursive(toAdd, curChildElement, subcurhti1);
	//		}
	//		else
	//		{
	//			curChildElement = curChildElement->GetNext();
	//		}
	//	}
	//}

	//// Add next till 
	//VJSONElement *curNextElement = toAdd->GetNext();
	///*while*/ if(curNextElement != NULL)
	//{
	//	HTREEITEM subcurhti2 = parentHti;
	//	bool doAdd = false;
	//	while (doAdd == false && curNextElement != NULL)
	//	{
	//		if (curNextElement->GetElType() == VJSONElement::ElTypeString)
	//		{
	//			VJSONString * curString = (VJSONString *)curNextElement;
	//			if (curString->GetIsKeyNameStr() == true)
	//				doAdd = false;
	//			else
	//				doAdd = true;
	//		}
	//		else
	//			doAdd = true;
	//		if (doAdd)
	//		{
	//			subcurhti2 = AddToTreeRecursive(parent, curNextElement, subcurhti2);
	//		}
	//		else
	//			curNextElement = curNextElement->GetNext();
	//	}
	//}
	//return curhti;
}

void CVisualJSONDlg::OnCbnSelChangeCurObjTemplate()
{
	try
	{
		int curSel = m_comboObjects.GetCurSel();
		if (curSel != LB_ERR)
		{
			m_curObjTemplate = m_allObjectTemplates.at(curSel);
			RefillTreeJSONObjectTemplate();
		}
	}
	catch (exception &e)
	{
		CString info(_T("error"));
		CString message;
		message.Format("CVisualJSONDlg::OnCbnSelChangeCurObjTemplate std::exception : %s", e.what());
		PrintMessageManager::AddMessage(info, message);
	}
	catch (...)
	{
		CString info(_T("error"));
		CString message;
		message.Format(_T("CVisualJSONDlg::OnCbnSelChangeCurObjTemplate unknown exception"));
		PrintMessageManager::AddMessage(info, message);
	}
}

void CVisualJSONDlg::RefillComboObjects()
{
	m_comboObjects.ResetContent();
	for (size_t i = 0; i < m_allObjectTemplates.size(); ++i)
	{
		VJSONObjectTemplate *curTemplate = m_allObjectTemplates.at(i);
		const MilString &curTemplateName = curTemplate->GetTemplateName();
		m_comboObjects.AddString(curTemplateName.c_str());

		//int myCounter = VJSONObjectTemplate::GetNameCountSoFarGlobal(templateName);
		//CString uniqName;
		//uniqName.Format("%s(%d)", templateName.c_str(), myCounter);
		//templateName = uniqName;
	}
	if (m_allObjectTemplates.size() > 0)
	{
		int curSel = (int)m_allObjectTemplates.size() - 1;
		m_comboObjects.SetCurSel(curSel);
	}
}

void CVisualJSONDlg::OnBnClickedDeleteElement()
{
	try
	{
		//AfxMessageBox("Delete element not implemented yet!");
		HTREEITEM htiSel = m_treeForObject.GetSelectedItem();
		if (htiSel != NULL)
		{
			DWORD_PTR myptr = m_treeForObject.GetItemData(htiSel);
			if (myptr != NULL)
			{
				VJSONElement *selElem = (VJSONElement *)myptr;
				m_curObjTemplate->DeleteElement(selElem);
				RefillTreeJSONObjectTemplate();
			}
		}
	}
	catch (exception &e)
	{
		CString info(_T("error"));
		CString message;
		message.Format("CVisualJSONDlg::OnBnClickedDeleteElement std::exception : %s", e.what());
		PrintMessageManager::AddMessage(info, message);
	}
	catch (...)
	{
		CString info(_T("error"));
		CString message;
		message.Format(_T("CVisualJSONDlg::OnBnClickedDeleteElement unknown exception"));
		PrintMessageManager::AddMessage(info, message);
	}
}
void CVisualJSONDlg::OnBnClickedMoveUp()
{
	try
	{
		AfxMessageBox("Move up not implemented yet!");
	}
	catch (exception &e)
	{
		CString info(_T("error"));
		CString message;
		message.Format("CVisualJSONDlg::OnBnClickedMoveUp std::exception : %s", e.what());
		PrintMessageManager::AddMessage(info, message);
	}
	catch (...)
	{
		CString info(_T("error"));
		CString message;
		message.Format(_T("CVisualJSONDlg::OnBnClickedMoveUp unknown exception"));
		PrintMessageManager::AddMessage(info, message);
	}
}
void CVisualJSONDlg::OnBnClickedMoveDown()
{
	try
	{
		AfxMessageBox("Move down not implemented yet!");
	}
	catch (exception &e)
	{
		CString info(_T("error"));
		CString message;
		message.Format("CVisualJSONDlg::OnBnClickedMoveDown std::exception : %s", e.what());
		PrintMessageManager::AddMessage(info, message);
	}
	catch (...)
	{
		CString info(_T("error"));
		CString message;
		message.Format(_T("CVisualJSONDlg::OnBnClickedMoveDown unknown exception"));
		PrintMessageManager::AddMessage(info, message);
	}
}
void CVisualJSONDlg::OnCompleteInsertObjectTemplate()
{
	try
	{
		UpdateData();
		if (m_curObjTemplate == NULL)
			throw exception("CVisualJSONDlg::OnCompleteInsertObjectTemplate exception , current template is NULL");
		//AfxMessageBox("Complete insert object template is not implemented yet");
		VJSONObjectTemplate * selTemplate = m_dialSelectObjTemplate->GetSelectedObject();
		if (selTemplate != NULL)
		{
			MilString *exportedStr = new MilString();
			ExportAsciiTextJSON(selTemplate, exportedStr);
			VJSONElement::ElType valType(VJSONElement::ElTypeObject);
			DoInsertStringToCurTemplateFromAnotherTemplate(selTemplate->GetTemplateName(), *exportedStr, valType);
			delete exportedStr;
		}
	}
	catch (exception &e)
	{
		CString info("error");
		CString message(e.what());
		PrintMessageManager::AddMessage(info, message);
	}
	catch (...)
	{
		CString info("error");
		CString message("CVisualJSONDlg::OnCompleteInsertObjectTemplate unknown exception");
		PrintMessageManager::AddMessage(info, message);
	}
}

void CVisualJSONDlg::OnCompleteAddElement()
{
	try
	{
		if (m_curObjTemplate == NULL)
			throw exception("CVisualJSONDlg::OnBnClickedMakeKeyValue exception , current object template is NULL");
		UpdateData();
		//DialogKeyValue mydial;
		//if (mydial.DoModal() == IDOK)
		//{
			////AddObjectToTree(mydial);
			//MilString keyName = (LPCTSTR)mydial.m_keyName;
			//int nameTokeni = -2; // it means this it self is name , -1 means element does not have a name
			//MilString jsonStr = keyName;
			//jsmntype_t tokenType = JSMN_STRING;
			//int parenti = -1;// add to root's tail (last element)
			//VJSONString *newJsonString = new VJSONString(nameTokeni,m_curObjTemplate,jsonStr,tokenType,parenti);
			//int newNameTokeni = newJsonString->GetTokeni();
			//MilString valJsonStr = mydial.m_defaultValue;
			//jsmntype_t valTokenType = JSMN_PRIMITIVE;
			//VJSONBasicVal *newBasicVal = new VJSONBasicVal(newNameTokeni, m_curObjTemplate, valJsonStr, valTokenType, parenti);
			//VJSONElement *tailNode = newJsonString->GetPrev();
			//if (tailNode != NULL)
			//	tailNode->SetNext(NULL);
			//// change the place of new entities - name and value
			//HTREEITEM htiSel = m_treeForObject.GetSelectedItem();
			//if (htiSel != NULL)
			//{
			//	DWORD_PTR myptr = m_treeForObject.GetItemData(htiSel);
			//	if (myptr != NULL)
			//	{
			//		VJSONElement *selElem = (VJSONElement *)myptr;
			//		VJSONElement::ElType selType = selElem->GetElType();
			//		BOOL isChild = m_radioAddUnderOrAfter == 0 ? TRUE : FALSE;
			//		if (selType != VJSONElement::ElTypeArray)// || selType != VJSONElement::ElTypeObject)
			//		{
			//			isChild = FALSE;
			//		}
			//		if (isChild)
			//		{
			//			selElem->AddChild(newJsonString);
			//		}
			//		else
			//		{
			//			VJSONElement *nextElem = selElem->GetNext();
			//			selElem->SetNext(newJsonString);
			//			if (nextElem != NULL)
			//			{
			//				nextElem->SetPrev(newBasicVal);
			//				newBasicVal->SetNext(nextElem);
			//			}
			//		}

			//		//ExportAsciiTextJSON *myexport = new ExportAsciiTextJSON(m_curObjTemplate);
			//		//VJSONObjectTemplate *newTemp = new VJSONObjectTemplate();
			//		//VJSONObject * templateRootJSONObj = new VJSONObject(newTemp);
			//		//MilString templateName = m_curObjTemplate->GetTemplateName();
			//		//MilString fileName(m_curObjTemplate->GetFileName());
			//		//newTemp->ConstructAndParse(templateRootJSONObj, templateName, fileName, &myexport->GetStr());
			//		//m_curObjTemplate = newTemp;
			//		//m_allObjectTemplates.push_back(m_curObjTemplate);
			//		m_curObjTemplate->ReconstructStr();
			//		//RefillComboObjects();
			//		RefillTreeJSONObjectTemplate();
			//	}
		DoInsertStringToCurTemplate(MilString(m_dialKeyVal->m_keyName), MilString(m_dialKeyVal->m_defaultValue), m_dialKeyVal->GetSelectedElType());
		//}
	}
	catch (exception e)
	{
		CString info("error");
		CString message(e.what());
		PrintMessageManager::AddMessage(info, message);
	}
	catch (...)
	{
		CString info("error");
		CString message("CVisualJSONDlg::OnCompleteAddElement unknown exception");
	}
}
void CVisualJSONDlg::DoInsertStringToCurTemplate(const MilString &name, const MilString &toInsert, VJSONElement::ElType valType)
{
	HTREEITEM htiSel = m_treeForObject.GetSelectedItem();
	if (htiSel != NULL)
	{
		DWORD_PTR myptr = m_treeForObject.GetItemData(htiSel);
		if (myptr != NULL)
		{
			VJSONElement *selElem = (VJSONElement *)myptr;
			m_curObjTemplate->DoInsertStringAndReconstruct(name,toInsert,valType,selElem,m_radioAddUnderOrAfter);
			RefillTreeJSONObjectTemplate();

		}
	}
}
void CVisualJSONDlg::UpdateKeyNameIf(VJSONElement *bigParent,MilString &newKeyName)
{
	int keyNameCountUnder = bigParent->GetNameCountForFirstLevelChildren(newKeyName);
	if (keyNameCountUnder > 0)
	{
		//if (newKeyName == "")
		//{

		//}
		//else
		//{
			//SimpleInputDialog nysimpleInputDial(keyStr,defaultValStr);
			while (keyNameCountUnder > 0)
			{
				MilStringStream strstr;
				strstr << newKeyName << '(' << keyNameCountUnder+1 << ')';
				newKeyName = strstr.str();
				CString arecomendation(newKeyName.c_str());
				SimpleInputDialog mydial(CString("There is name collision. Please rename:"), arecomendation);
				if (mydial.DoModal() == IDCANCEL)
					return;
				newKeyName = mydial.m_valStr;
				if (newKeyName != "")
				{
					keyNameCountUnder = bigParent->GetNameCountForFirstLevelChildren(newKeyName);
				}
				else
				{
					newKeyName = arecomendation;
				}
			}
		//}
	}
}
void CVisualJSONDlg::DoInsertStringToCurTemplateFromAnotherTemplate(const MilString &name, const MilString &toInsert, VJSONElement::ElType valType)
{
	HTREEITEM htiSel = m_treeForObject.GetSelectedItem();
	if (htiSel != NULL)
	{
		DWORD_PTR myptr = m_treeForObject.GetItemData(htiSel);
		if (myptr != NULL)
		{
			VJSONElement *selElem = (VJSONElement *)myptr;
			VJSONElement::ElType selType = selElem->GetElType();
			//VJSONElement::ElType valType = m_dialKeyVal->GetSelectedElType();
			VJSONElement::ElType parentType = selType;
			MilString keyName(name);
			MilString defaultValue(toInsert);
			BOOL isChild = m_radioAddUnderOrAfter == 0 || m_radioAddUnderOrAfter == 2 ? TRUE : FALSE;
			MilString *strToInsert = NULL;
			if (isChild)
			{
				if (selType != VJSONElement::ElTypeArray && selType != VJSONElement::ElTypeObject)
				{
					//isChild = FALSE;
					AfxMessageBox("You should select either an object or array element to insert a child item!");
					return;
				}

				UpdateKeyNameIf(selElem, keyName);

				if (m_radioAddUnderOrAfter == 0)
				{
					bool addCommaAtStart = true;
					bool addCommaAtEnd = false;
					VJSONElement *prevEl = selElem->GetLastChild();
					if (prevEl == NULL)
						addCommaAtStart = false;
					strToInsert = new MilString(VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd));
					m_curObjTemplate->InsertRawJsonStringAsLastChild(selElem, *strToInsert);
					m_curObjTemplate->ReconstructStr(selElem,prevEl);
					RefillTreeJSONObjectTemplate();
				}
				else if (m_radioAddUnderOrAfter == 2)
				{
					bool addCommaAtStart = false;
					bool addCommaAtEnd = true;
					VJSONElement *prevEl = selElem->GetLastChild();
					if (prevEl == NULL)
						addCommaAtEnd = false;
					strToInsert = new MilString(VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd));
					m_curObjTemplate->InsertRawJsonStringAsFirstChild(selElem, *strToInsert);
					m_curObjTemplate->ReconstructStr(selElem,prevEl);
					RefillTreeJSONObjectTemplate();
				}
			}
			else
			{
				VJSONElement *bigParent = selElem->GetParent();
				if (bigParent == NULL)
					throw exception("CVisualJSONDlg::OnBnClickedMakeKeyValue selected element is root! Neither previous nor next element can be added");
				UpdateKeyNameIf(bigParent, keyName);
				parentType = bigParent->GetElType();
				if (m_radioAddUnderOrAfter == 1)
				{
					bool addCommaAtStart = true;
					bool addCommaAtEnd = selElem->GetNext() != NULL ? true : false;
					UpdateKeyNameIf(bigParent,keyName);
					strToInsert = new MilString(VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd));
					m_curObjTemplate->InsertRawJsonStringAsNextSibling(selElem, *strToInsert);
					m_curObjTemplate->ReconstructStr(bigParent,selElem);
					RefillTreeJSONObjectTemplate();
				}
				else if (m_radioAddUnderOrAfter == 3)
				{
					bool addCommaAtStart = false;
					bool addCommaAtEnd = true;
					strToInsert = new MilString(VJSONObjectTemplate::GetRawJsonString(keyName, defaultValue, parentType, valType, addCommaAtStart, addCommaAtEnd));
					m_curObjTemplate->InsertRawJsonStringAsPrevSibling(selElem, *strToInsert);
					m_curObjTemplate->ReconstructStr(bigParent,selElem->GetPrev());
					RefillTreeJSONObjectTemplate();
				}
			}
			delete strToInsert;
		}
	}
}

void CVisualJSONDlg::OnBnClickedSelectImportFilePath()
{
	UpdateData();
	CFileDialog mydial(TRUE);
	if (mydial.DoModal() == IDOK)
	{
		m_importFilePathStr	= mydial.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}

void CVisualJSONDlg::OnBnClickedSelectExportFilePath()
{
	UpdateData();
	CFileDialog mydial(TRUE);
	if (mydial.DoModal() == IDOK)
	{
		m_exportFilePathStr = mydial.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}
void CVisualJSONDlg::OnBnClickedEditSelectedElement()
{
	if (m_curObjTemplate == NULL)
		throw exception("CVisualJSONDlg::OnBnClickedEditSelectedElement exception , no object template is active");

	HTREEITEM hti = m_treeForObject.GetSelectedItem();
	if (hti != NULL)
	{
		DWORD_PTR itemData = m_treeForObject.GetItemData(hti);
		if (itemData == NULL)
		{

		}
		VJSONElement *selel = (VJSONElement *)itemData;
		if (selel == m_curObjTemplate->GetTemplateRootJSONObject())
			throw exception("CVisualJSONDlg::OnBnClcikedEditSelectedElement current object template root element is selected , it can not be edited");

		VJSONElement *nameStrEl(NULL);
		VJSONElement *prevEl = selel->GetPrev();
		if (prevEl != NULL && prevEl->GetElType() == VJSONElement::ElTypeString && ((VJSONString *)prevEl)->GetIsKeyNameStr())
			nameStrEl = prevEl;
		//VJSONElement *grandParent = selel->GetParent();// this should not be NULL as selel is not root
		//bool addAsNextTrue_AddAsFirstChildFalse(false);
		m_dialKeyVal->ReinitForEditElement(nameStrEl, selel);
		m_dialKeyVal->ShowWindow(SW_SHOW);
	}
	else
	{
		throw exception ("Select an element on tree");
	}
}
void CVisualJSONDlg::OnCompleteEditElement()
{
	try
	{
		if (m_curObjTemplate == NULL)
			throw exception("CVisualJSONDlg::OnBnClickedEditSelectedElement exception , current object template is NULL");
		

			MilString keyName(m_dialKeyVal->m_keyName);
			MilString defaultValue(m_dialKeyVal->m_defaultValue);
			VJSONElement::ElType valType = m_dialKeyVal->GetSelectedElType();
			VJSONElement *selel = m_dialKeyVal->GetSelElAndMakeItNull();
			if (selel == NULL)
				throw ("CVisualJSONDlg::OnCompleteEditElement exception. DialogKeyValue returns null for selected element");
		    std::pair<VJSONElement *,VJSONElement *> ancesters = m_curObjTemplate->DoInsertStringAndReconstruct(keyName , defaultValue , valType, selel, 1);
			if (ancesters.second != NULL)
				m_curObjTemplate->DeleteElement(ancesters.second);
			else
				throw exception("CVisualJSONDlg::OnBnClickedEditSelectedElement exception. Previous brother should not be NULL.");
			RefillTreeJSONObjectTemplate();
	}
	catch (exception &e)
	{
		CString infoTxt("error");
		CString message(e.what());
		PrintMessageManager::AddMessage(infoTxt, message);
	}
	catch (...)
	{
		CString infoTxt("error");
		CString message("Unknown exception in CVisualJSONDlg::OnBnClickedEditSelectedElement");
		PrintMessageManager::AddMessage(infoTxt, message);
	}
}

void CVisualJSONDlg::LoadSettingsIf()
{
	MilString myExeFolder = MilPathStringFunctions::GetVJSONExeFolder();
	MilString settingFileName(myExeFolder);
	settingFileName += "VJSONSettings.json";
	m_importFilePathStr = settingFileName.c_str();
	OnBnClickedImportJSON();
	if (m_allObjectTemplates.rbegin() != m_allObjectTemplates.rend())
	{
		VJSONObjectTemplate *temp = *m_allObjectTemplates.rbegin();
		if (temp->GetTemplateName() == "VJSONSettings")
		{
			m_settings.SetObject(temp);
		}
	}
}

void CVisualJSONDlg::OnBnClickedSettings()
{
	try
	{
		MilString myExeFolder = MilPathStringFunctions::GetVJSONExeFolder();
		MilString settingFileName(myExeFolder);
		settingFileName += "VJSONSettings.json";
		m_settings.OpenFileWithEditor(settingFileName);
	}
	catch (exception &e)
	{
		CString info("error");
		CString message("CVisualJSONDlg::OnBnClickedSettings exception.");
		message += e.what();
		PrintMessageManager::AddMessage(info,message);
	}
	catch (...)
	{
		CString info("error");
		CString message("CVisualJSONDlg::OnBnClickedSettings unknown exception");
		PrintMessageManager::AddMessage(info, message);
	}
}