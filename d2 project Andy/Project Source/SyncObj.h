
#ifndef __SYNCOBJ_H__
#define __SYNCOBJ_H__

#include <windows.h>
#include <stdio.h>

class CSyncObj  
{
public:
	
	/////////////////////////////////////////////////////////////////
	// Constructor & Destructor
	/////////////////////////////////////////////////////////////////
	CSyncObj() { ::InitializeCriticalSection(&m_cs); IsLocked = false;}
	virtual ~CSyncObj() { ::DeleteCriticalSection(&m_cs); }
	
	/////////////////////////////////////////////////////////////////
	// Public Operations
	/////////////////////////////////////////////////////////////////
	void Lock()  { ::EnterCriticalSection((LPCRITICAL_SECTION)&m_cs); IsLocked = true; }
	void Unlock()  { ::LeaveCriticalSection((LPCRITICAL_SECTION)&m_cs); IsLocked = false; }	

	bool IsLocked;

private:

	/////////////////////////////////////////////////////////////////
	// Private Member Data
	/////////////////////////////////////////////////////////////////
	CRITICAL_SECTION m_cs;
};

#endif // __SYNCOBJ_H__
