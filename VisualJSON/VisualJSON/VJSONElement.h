// VJSONElement.h

#ifndef __VJSONElement_h__
#define __VJSONElement_h__

#include "jsmn.h"

class VJSONObjectTemplate;

class VJSONElement
{
public:
	VJSONElement(VJSONObjectTemplate *root);// used only for VJSONObjectTemplate::m_templateRootJSONObject
	VJSONElement(int nameTokeni, VJSONObjectTemplate *root, int tokeni);
	VJSONElement(int nameTokeni, VJSONObjectTemplate *root,const MilString &jsonStr,jsmntype_t tokenType , int parenti);// not from parsed file but made manually by GUI 
	virtual ~VJSONElement();
	//VJSONElement(const VJSONElement &toCopy);
	//const VJSONElement & operator = (const VJSONElement &toCopy);
	enum ElType{ElTypeString , ElTypeBasic , ElTypeObject , ElTypeArray};
	//void AddToJSONStringWithNameQuotAndColons(MilString &str)const;
	MilString GetJSONString()const;
	MilString GetName()const;
	virtual ElType GetElType()const = 0;
	enum KeyCompareResult{EqualKey,DifferentKey};
	enum ValueCompareResult {EqualValue,SameSubTypeDifferentValue,DifferentSubType,DifferentTypes};// Type is object,array,string,basictype   subtype is floatingpoint,boolean,int
	enum LocationCompareResult{EqualLocation,DifferentLocation};
	class CompareResult
	{
	public:
		KeyCompareResult m_keyComp;
		ValueCompareResult m_valComp;
		LocationCompareResult m_locComp;
	};
	CompareResult Compare(const VJSONElement *other)const; // other element and this should belong to different VJSONObjectTemplate , main goal is to compare 2 VJSONObjectTemplate
	//virtual void Compare(const VJSONObjectTemplate &objTemplate , vector<CompareResult> &comp);
protected:
	VJSONObjectTemplate *m_root;
	int m_nameTokeni;// -1 means Element does not have a key name , -2 means Element it self is a key name string , 0 or positive integer are index to VJSONString element for key name string
	VJSONElement * m_parent;
	VJSONElement * m_firstChild;
	VJSONElement * m_next;
	VJSONElement * m_prev;
	int m_tokeni;
	//void VJSONElement::SetParentForConstructor(const jsmntok_t &t);
public:
	VJSONElement * GetParent();
	VJSONElement * GetFirstChild();
	VJSONElement * GetNext();
	VJSONElement * GetPrev();
	int GetTokeni()const;
	int GetNameTokeni()const;
//protected:
	void SetParent(VJSONElement * parent);
	void SetFirstChild(VJSONElement *child);
	void SetNext(VJSONElement * next);
	void SetPrev(VJSONElement *prev);
	//void AddNameToJSONString(MilString &str)const;
	void AddChild(VJSONElement *child);
public:
	MilString GetElTypeName()const;
	MilString GetJSONStringWithoutWhiteSpaceIf()const;// all white spaces out of quotations (strings) will be removed
	static MilString RemoveWhiteSpaceOutOfQuotations(const MilString &toClean);
	int GetNameCountForFirstLevelChildren(const MilString &keyName);
	VJSONElement * GetLastChild();
	void CollectAllChildrenRecursive(std::list<VJSONElement *> &allChildren);
};

#endif