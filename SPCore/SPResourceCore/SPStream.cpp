


#include "SPStream.h"
#include "SPResourceBaseDEF.h"
#include "SPResourceBase.h"


SPFileStream::SPFileStream(const char* pszFilename, int iMode )
{
	switch ( iMode ) {
	case OPEN_READ_ASCII :
		m_iFileHandle=::_open( pszFilename, _O_RDONLY | _O_TEXT );		
		break;

	case OPEN_WRITE_ASCII :
		m_iFileHandle=::_open( pszFilename, _O_RDWR| _O_TEXT | _O_TRUNC | _O_CREAT, S_IREAD | S_IWRITE );
		break;

	case OPEN_READWRITE_ASCII :
		m_iFileHandle=::_open( pszFilename, _O_RDWR | _O_TEXT );
		break;

	case OPEN_CREATE_ASCII :
		m_iFileHandle=::_open( pszFilename, _O_RDWR | _O_CREAT | _O_TEXT, S_IREAD | S_IWRITE );
		break;

	case OPEN_WRITE :
		m_iFileHandle=::_open( pszFilename, _O_RDWR| _O_BINARY | _O_TRUNC | _O_CREAT, S_IREAD | S_IWRITE );
		break;

	case OPEN_READWRITE :
		m_iFileHandle=::_open( pszFilename, _O_RDWR | _O_BINARY );
		break;

	case OPEN_CREATE :
		m_iFileHandle=::_open( pszFilename, _O_RDWR | _O_CREAT | _O_BINARY, S_IREAD | S_IWRITE );
		break;

	case OPEN_READ :
	default :
		m_iFileHandle=::_open( pszFilename, _O_RDONLY | _O_BINARY );		
		break;
	}
}

SPFileStream::~SPFileStream()
{
	if ( m_iFileHandle != -1 )
		::_close( m_iFileHandle );
}

void SPFileStream::Release()
{
	delete this; //澜澜..入入..
}

int SPFileStream::Seek( int iOffset, int iOrigin )
{
	return ::_lseek( m_iFileHandle, iOffset, iOrigin );
}

int SPFileStream::Read( void* ptr, unsigned int iSize )
{
	return ::_read( m_iFileHandle, ptr, iSize );
}

int SPFileStream::Write( const void *ptr, unsigned int iSize )
{
	return ::_write( m_iFileHandle, ptr, iSize );
}

bool SPFileStream::Eos() const
{
	return ::_eof( m_iFileHandle )!=0;
}

unsigned int SPFileStream::Tell() const
{
	return ::_tell( m_iFileHandle );
}

unsigned int SPFileStream::Length() const
{
	return ::_filelength( m_iFileHandle );
}

bool SPFileStream::Valid() const
{
	return m_iFileHandle != -1;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SPMemoryStream::SPMemoryStream( BYTE* pBuf, unsigned int iSize )
{
	//m_pBuf = static_cast< BYTE* >( pBuf );
	m_pBuf = pBuf;
	m_size = iSize;
	m_cur = 0;
}

int SPMemoryStream::Seek( int iOffset, int iOrigin )
{
	switch ( iOrigin ) {
	case SEEK_SET: m_cur = iOffset; break;
	case SEEK_CUR: m_cur += iOffset; break;
	case SEEK_END: m_cur = m_size+iOffset; break;
	default : assert( false ); return 0;
	}
	if ( m_cur >= m_size ) m_cur = m_size;
	return m_cur;
}

int SPMemoryStream::Read( void *ptr, unsigned int iSize )
{
	if ( m_cur + iSize > m_size ) iSize = m_size-m_cur;
	memcpy( ptr, m_pBuf + m_cur, iSize );
	m_cur += iSize;
	return iSize;
}

int SPMemoryStream::Write( const void *ptr, unsigned int iSize )
{
	if ( m_cur + iSize > m_size ) iSize = m_size-m_cur;
	memcpy( m_pBuf + m_cur, ptr, iSize );
	m_cur += iSize;
	return iSize;
}

bool SPMemoryStream::Eos () const
{
	return m_cur == m_size;
}

unsigned int SPMemoryStream::Tell () const
{
	return m_cur;
}

unsigned int SPMemoryStream::Length () const
{
	return m_size;
}

bool SPMemoryStream::Valid() const
{
	return m_pBuf != static_cast< BYTE* >(0);
}

void* SPMemoryStream::Buf()			
{ 
	return static_cast<void*>(m_pBuf); 
}

const void* SPMemoryStream::Buf()	const	
{ 
	return static_cast<const void*>(m_pBuf); 
}

void* SPMemoryStream::operator()()				
{ 
	return static_cast<void*>(m_pBuf); 
}

const void* SPMemoryStream::operator()()	const	
{ 
	return static_cast<const void*>(m_pBuf); 
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
SPMemoryMappedFileStream::SPMemoryMappedFileStream(const char* pszFilename, int iMode )
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_hMap	= NULL;
	m_size	= 0;
	m_cur	= 1;
	m_pBuf = NULL;

	switch ( iMode ) {
	case OPEN_WRITE :		
	case OPEN_READWRITE :
		m_hFile = CreateFile(pszFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );		
		if(m_hFile != INVALID_HANDLE_VALUE) {
			m_size	= GetFileSize( m_hFile, NULL);
			m_cur = 0;
			m_hMap = CreateFileMapping( m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
		}	
		break;	
		
	case OPEN_CREATE :		
		m_hFile = CreateFile(pszFilename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING, NULL );		
		if(m_hFile != INVALID_HANDLE_VALUE) {
			m_size	= GetFileSize( m_hFile, NULL);
			m_cur = 0;
			m_hMap = CreateFileMapping( m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
		}	
		break;
		
	case OPEN_READ :
	default :
		m_hFile = CreateFile(pszFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );		
		if(m_hFile != INVALID_HANDLE_VALUE) {
			m_size	= GetFileSize( m_hFile, NULL);
			m_cur = 0;

			m_hMap = CreateFileMapping( m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		}	
		break;
	}	

	if(m_hMap)
		m_pBuf = MapViewOfFile(m_hMap,	FILE_MAP_READ,	0, 0, 0);

}


SPMemoryMappedFileStream::~SPMemoryMappedFileStream()
{
	UnmapViewOfFile( m_pBuf );

	if(m_hMap != NULL) {
		CloseHandle( m_hMap);
	}
	if(m_hFile != INVALID_HANDLE_VALUE) {
		CloseHandle( m_hFile);
	}	
}

void SPMemoryMappedFileStream::Release()
{
	delete this;
}


int SPMemoryMappedFileStream::Seek( int iOffset, int iOrigin )
{
	switch ( iOrigin ) {
	case SEEK_SET: m_cur = iOffset; break;
	case SEEK_CUR: m_cur += iOffset; break;
	case SEEK_END: m_cur = m_size+iOffset; break;
	default : assert( false ); return 0;
	}
	if ( m_cur >= m_size ) m_cur = m_size;
	return m_cur;
}


int SPMemoryMappedFileStream::Read( void* ptr, unsigned int iSize )
{	
	if ( m_cur + iSize > m_size ) 
		iSize = m_size-m_cur;	

	//m_pBuf = MapViewOfFile(m_hMap,	FILE_MAP_READ,	0, 0, 0);
	if (m_pBuf ==  NULL) {
		//UnmapViewOfFile( m_pBuf );
		return 0;
	}
	CopyMemory( ptr, ((BYTE*)m_pBuf) + m_cur, iSize );
	m_cur += iSize;
	//UnmapViewOfFile( m_pBuf );
	return iSize;
}

int SPMemoryMappedFileStream::Write( const void* ptr, unsigned int iSize )
{
	if ( m_cur + iSize > m_size ) iSize = m_size-m_cur;

	//m_pBuf = MapViewOfFile(m_hMap,	FILE_MAP_WRITE,	0, 0, 0);
	if (m_pBuf ==  NULL) {
		//UnmapViewOfFile( m_pBuf );
		return 0;
	}
	CopyMemory( ((BYTE*)m_pBuf) + m_cur, ptr, iSize );
	m_cur += iSize;
	//UnmapViewOfFile( m_pBuf );
	return iSize;
}

bool SPMemoryMappedFileStream::Eos() const
{
	return m_cur == m_size;
}

unsigned int SPMemoryMappedFileStream::Tell() const
{
	return m_cur;
}

unsigned int SPMemoryMappedFileStream::Length() const
{
	return m_size;
}

bool SPMemoryMappedFileStream::Valid() const
{
	if(m_hFile != INVALID_HANDLE_VALUE && m_hMap != NULL) {
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


SPManagedStream::SPManagedStream(  BYTE* buf, unsigned int iSize, int iInstance)
: SPMemoryStream(buf, iSize)
, m_iResInstanceID(iInstance)
{}


SPManagedStream::~SPManagedStream()
{
}


int SPManagedStream::Write ( const void* ptr, unsigned int iSize )
{ 
	//Cannot write to SPManagedStream..!!
	return 0; 
} 

void SPManagedStream::Release()
{
	SPResourceBase::GetInstance()->ReleaseStreamData(m_iResInstanceID);

	delete this; //澜澜..入入..
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

