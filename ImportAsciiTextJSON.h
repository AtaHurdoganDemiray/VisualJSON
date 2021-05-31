// ImportAsciiTextJSON.h

#ifndef __ImportAsciiTextJSON_h__
#define __ImportAsciiTextJSON_h__

#include "jsmn.h"

class VJSONObject;
class VJSONBasicVal;
class VJSONElement;
class VJSONObjectTemplate;

class ImportAsciiTextJSON
{
public:
	ImportAsciiTextJSON(const MilString &fileName); //, const MilString &rootObjectName);
	
private:
	VJSONObjectTemplate * m_importResult;
public:
	VJSONObjectTemplate * GetImportResult()const;
};

#endif