// VisualJSONSettings.h

#ifndef __VisualJSONSettings_h__
#define __VisualJSONSettings_h__

class VJSONObject;
class VJSONElement;
class VJSONObjectTemplate;

class VisualJSONSettings
{
public:
	VisualJSONSettings(VJSONObjectTemplate *settingsObject);
	void SetObject(VJSONObjectTemplate *settingsObject);
	VJSONObject * SearchParameterSet(const MilString &categoryName)const;
	VJSONElement * SearchParameter(VJSONObject *parameterSet, const MilString &parameterKey)const;
	void OpenFileWithEditor(const MilString &filePathToOpen)const;
	MilString GetAutomatedTestOutputFolder()const;
private:
	VJSONObjectTemplate * m_settingsObject;
};

#endif