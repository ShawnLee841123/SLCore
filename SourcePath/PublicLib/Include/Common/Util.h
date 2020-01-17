
#ifndef __PUBLIC_LIB_COMMON_UTIL_H__
#define __PUBLIC_LIB_COMMON_UTIL_H__

#include "TypeDefines.h"

#pragma region SLDynamicBuffer
class SLDynamicBuffer
{
public:
	SLDynamicBuffer();
	SLDynamicBuffer(UI32 uSize);
	~SLDynamicBuffer();

	SLDynamicBuffer& operator=(const SLDynamicBuffer& rhv);

	char* GetBuffer();
	bool SetBufferSize(UI32 uSize);
	UI32 GetSize();

protected:
	UI32		m_uSize;
	char*		m_pBuffer;
};
#pragma endregion


#endif	//	__PUBLIC_LIB_COMMON_UTIL_H__
