#include "CFile.h"

CFile::CFile( string filename, file_permission_t eFlags, bool bOpen )
{
	this->m_eFlags		= eFlags;
	this->m_filename	= filename;

	if( bOpen )
		this->Open();
}

CFile::~CFile()
{
	this->Close();
}

void CFile::Open()
{
	string mode;

	if( m_eFlags & FILE_READ )
	{
		mode = "r";
	} else if( m_eFlags & FILE_WRITE ) {
		mode = "w";
	} else if( m_eFlags & FILE_APPEND ) {
		mode = "a";
	}

	if( m_eFlags & FILE_PLUS )
	{
		mode += "+";
	}

	//gApp.AddToLogFile( "Opening file [%s][%s]", m_filename.c_str(), mode.c_str() );

	m_pfs = new fstream( m_filename.c_str() );
}

void CFile::Close()
{
	if( Valid() )
	{
		m_pfs->close();
	}
}

bool CFile::Valid()
{
	if( !m_pfs )
		return false;

	return m_pfs->is_open();
}

long CFile::GetSize()
{
	if( !Valid() )
		return 0;

	long begin	= m_pfs->tellg();
	m_pfs->seekg( 0, ios::end );
	long end	= m_pfs->tellg();
	m_pfs->seekg( 0, ios::beg );

	return ( end - begin );
}

bool CFile::ReadLines( vector<string> &lines, int linesize )
{
	if( !CanRead() )
		return false;

	char *szBuffer = new char[ linesize ];

	while( m_pfs->good() )
	{
		m_pfs->getline( szBuffer, linesize );

		if( szBuffer[ strlen( szBuffer ) - 1 ] == '\n' )
			szBuffer[ strlen( szBuffer ) - 1 ] = '\0';

		lines.push_back( szBuffer );
	}

	m_pfs->seekg( 0, ios::beg );

	return true;
}

bool CFile::ReadContents( unsigned char *ucBuffer, unsigned long *lSize )
{
	if( !CanRead() )
		return false;

	long size = GetSize();
	long num  = 0;

	while( m_pfs->good() && num <= size )
	{
		ucBuffer[ num ] = static_cast< unsigned char >( m_pfs->get() );

		num++;
	}

	if( lSize )
	{
		*lSize = size;
	}

	m_pfs->seekg( 0, ios::beg );

	return true;
}

void CFile::Append( string append )
{
	return Append((char *)append.c_str());
}

void CFile::Append( char *szFormat, ... )
{
	va_list va_alist;

	char szBuffer[ 1024 ] = { 0 };

	va_start( va_alist, szFormat );
	
	_vsnprintf( szBuffer + strlen( szBuffer ), sizeof( szBuffer ) - strlen( szBuffer ), szFormat, va_alist );
	
	va_end( va_alist );

	Append((unsigned char *)szBuffer, (int)strlen( szBuffer ));
}

void CFile::Append( unsigned char *ucData, int size )
{
	if( !CanAppend() )
		return;

	for( int i = 0; i < size; i++ )
	{
		m_pfs->put((char)ucData[i]);
	}
}

void CFile::Write( string write )
{
	return Write((char *)write.c_str());
}

void CFile::Write( char *szFormat, ... )
{
	va_list va_alist;

	char szBuffer[ 1024 ] = { 0 };

	va_start( va_alist, szFormat );
	
	_vsnprintf( szBuffer + strlen( szBuffer ), sizeof( szBuffer ) - strlen( szBuffer ), szFormat, va_alist );
	
	va_end( va_alist );

	Write((unsigned char *)szBuffer, (int)strlen( szBuffer ));
}

void CFile::Write( unsigned char *ucData, int size )
{
	if( !CanWrite() )
		return;

	for( int i = 0; i < size; i++ )
	{
		m_pfs->put((char)ucData[i]);
	}
}

bool CFile::FileExists()
{
	if( Valid() )
		this->Close();

	ifstream f( m_filename.c_str() );
	
	bool valid = f.is_open();

	f.close();

	return valid;
}

bool CFile::CanRead()
{
	return ( Valid() && ( m_eFlags & FILE_READ ) );
}

bool CFile::CanWrite()
{
	return ( Valid() && ( m_eFlags & FILE_WRITE ) );
}

bool CFile::CanAppend()
{
	return ( Valid() && ( m_eFlags & FILE_APPEND ) );
}

bool CFile::DoesOverwrite()
{
	return (m_eFlags & FILE_WRITE) ? true : false;
}

bool CFile::DoesCreate()
{
	return ((m_eFlags & FILE_WRITE) || (m_eFlags & FILE_APPEND));
}