#ifndef CFILE_HEADER
#define CFILE_HEADER

#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

typedef enum {
	FILE_READ		= 0x1,
	FILE_APPEND		= 0x2,
	FILE_WRITE		= 0x4,
	FILE_PLUS		= 0x8
} file_permission_t;

class CFile
{
public:
	CFile( string filename, file_permission_t eFlags, bool bOpen = false );
	~CFile();

	void	Open();
	void	Close();
	
	bool	Valid();
	
	long	GetSize();

	bool	ReadLines( vector<string> &lines, int linesize );
	bool	ReadContents( unsigned char *ucBuffer, unsigned long *lSize );

	void	Append( string append );
	void	Append( char *szFormat, ... );
	void	Append( unsigned char *ucData, int size );
	
	void	Write( string write );
	void	Write( char *szFormat, ... );
	void	Write( unsigned char *ucData, int size );

	bool	FileExists();

	file_permission_t GetFilePermissions(){ return m_eFlags; }

	bool	CanRead();
	bool	CanAppend();
	bool	CanWrite();
	bool	DoesOverwrite();
	bool	DoesCreate();
	
private:

	fstream*			m_pfs;
	file_permission_t	m_eFlags;
	string				m_filename;
};

#endif