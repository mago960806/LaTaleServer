
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPStream
// Comment     : 
// Creation    : metalgeni 2005-05-10 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once


#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

#include <assert.h>
#include <wtypes.h>
#include <string>

class SPStream
{
public:
	enum {
		OPEN_READ			= 1<<0,
		OPEN_WRITE			= 2<<0,
		OPEN_READWRITE		= 3<<0,
		OPEN_CREATE			= 4<<0,	

		OPEN_READ_ASCII				= 5<<0,
		OPEN_WRITE_ASCII			= 6<<0,
		OPEN_READWRITE_ASCII		= 7<<0,
		OPEN_CREATE_ASCII			= 8<<0,	

	};

	virtual ~SPStream() {}		
	virtual void Release() = 0; 

	virtual int Seek( int iOffset, int iOrigin ) = 0;
	virtual int Read( void* buffer, unsigned int iSize ) = 0;
	virtual int Write( const void* ptr, unsigned int iSize ) = 0;	
	virtual bool Eos() const = 0;				
	virtual unsigned int Tell() const = 0;
	virtual unsigned int Length() const = 0;
	virtual bool Valid() const = 0;		

};


class SPFileStream : public SPStream
{
public:
	SPFileStream(const char* pszFilename, int iMode = OPEN_READ );
	virtual ~SPFileStream();
	virtual void Release();

	virtual int Seek( int iOffset, int iOrigin );
	virtual int Read( void* ptr, unsigned int iSize );
	virtual int Write( const void* ptr, unsigned int iSize );
	virtual bool Eos() const;
	virtual unsigned int Tell() const;
	virtual unsigned int Length() const;
	virtual bool Valid() const;

private:
	int m_iFileHandle;
};


class SPMemoryStream : public SPStream
{
public:
	SPMemoryStream(BYTE* buf, unsigned int iSize);

	virtual int Seek( int iOffset, int iOrigin );
	virtual int Read( void* ptr, unsigned int iSize );
	virtual int Write( const void* ptr, unsigned int iSize );	
	virtual bool Eos() const;
	virtual unsigned int Tell() const;
	virtual unsigned int Length() const;
	virtual bool Valid() const;

	virtual void*			Buf();			
	virtual const void*		Buf()	const;
	virtual void*			operator() ();
	virtual const void*		operator() ()		const;

protected:
	BYTE* m_pBuf;
	unsigned int m_cur;
	unsigned int m_size;
};


class SPMemoryMappedFileStream : public SPStream
{
public:
	SPMemoryMappedFileStream(const char* pszFilename, int iMode = OPEN_READ );
	virtual ~SPMemoryMappedFileStream();
	virtual void Release();
	virtual int Seek( int iOffset, int iOrigin );
	virtual int Read( void* ptr, unsigned int iSize );
	virtual int Write( const void* ptr, unsigned int iSize );
	virtual bool Eos() const;
	virtual unsigned int Tell() const;
	virtual unsigned int Length() const;
	virtual bool Valid() const;

private:
		
	HANDLE m_hFile;
	HANDLE m_hMap;

	LPVOID m_pBuf;
	unsigned int m_cur;
	unsigned int m_size;

};

class SPResourceBase;
class SPManagedStream : public SPMemoryStream
{
public:		
	virtual ~SPManagedStream();	
	virtual int Write ( const void* ptr, unsigned int iSize );
	virtual void Release();

protected:
	friend SPResourceBase;
	SPManagedStream( BYTE* buf, unsigned int iSize, int iInstance);	


protected:		
	int						m_iResInstanceID;	
};


