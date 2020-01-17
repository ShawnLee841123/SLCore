#include "../../Include/Common/Util.h"
#include <string.h>
#pragma region SLDynamicBuffer
SLDynamicBuffer::SLDynamicBuffer():m_pBuffer(nullptr), m_uSize(0)
{}

SLDynamicBuffer::SLDynamicBuffer(UI32 uSize)
{
	SetBufferSize(uSize);
}

SLDynamicBuffer::~SLDynamicBuffer()
{
	if (nullptr != m_pBuffer)
		delete[] m_pBuffer;
}

SLDynamicBuffer& SLDynamicBuffer::operator=(const SLDynamicBuffer& rhv)
{
	SetBufferSize(rhv.m_uSize);
	memcpy(m_pBuffer, rhv.m_pBuffer, sizeof(char) * m_uSize);
	return *this;
}

char* SLDynamicBuffer::GetBuffer()
{
	return m_pBuffer;
}

bool SLDynamicBuffer::SetBufferSize(UI32 uSize)
{
	m_uSize = uSize;
	m_pBuffer = new char[m_uSize];
	return nullptr != m_pBuffer;
}

UI32 SLDynamicBuffer::GetSize()
{
	return m_uSize;
}

#pragma endregion

