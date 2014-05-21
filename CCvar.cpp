#include "stdafx.h"
#include <windows.h>
#include <sstream>
#include "CCvar.h"

CCvar GCvar;

int CCvar::AddSection( string Name )
{
	FSection *NewSection = new FSection;

	NewSection->Name = Name;
	NewSection->Cvar.clear();

	int iReturn = GetSectionCount();

	Sections.push_back( NewSection );

	return iReturn;
}

int CCvar::AddCvar( string SectionName, string Name, string Clean, string Desc, float Value, float Min, float Max )
{
	return AddCvar( GetSectionByName( SectionName ), Name, Clean, Desc, Value, Min, Max );
}

int CCvar::AddCvar( int SectionIndex, string Name, string Clean, string Desc, float Value, float Min, float Max )
{
	return AddCvar( GetSectionByIndex( SectionIndex ), Name, Clean, Desc, Value, Min, Max );
}

int CCvar::AddCvar( FSection* Section, string Name, string Clean, string Desc, float Value, float Min, float Max )
{
	if( Section == 0 ) return INVALID_CVAR_INDEX;

	FCvar *NewCvar = new FCvar;

	NewCvar->NativeName		= Name;
	NewCvar->DisplayName	= Clean;
	NewCvar->DisplayDesc	= Desc;
	NewCvar->min			= Min;
	NewCvar->max			= Max;
	NewCvar->Value			= Value;

	int iReturn = Section->GetCvarCount();

	if( Min == 0.f && Max == 1.f )
	{
		vector< string > OffOn;

		OffOn.push_back( "Off" );
		OffOn.push_back( "On" );

		NewCvar->LoadCleanArray( OffOn );
	}

	Section->Cvar.push_back( NewCvar );

	return iReturn;
}

FSection* CCvar::GetSectionByName( string Name )
{
	for( int i = 0; i < GetSectionCount(); i++ )
	{
		if( Name == Sections[ i ]->Name )
		{
			return GetSectionByIndex( i );
		}
	}

	static FSection Blank;

	memset( &Blank, 0, sizeof( FSection ) );

	return &Blank;
}

FSection* CCvar::GetSectionByIndex( int i )
{
	return Sections[ i ];
}

int CCvar::GetSectionCount()
{
	return static_cast< int >( Sections.size() );
}

int CCvar::GetCvarCount( FSection* Section )
{
	return static_cast< int >( Section->Cvar.size() );
}

vector<FSection*> CCvar::GetSectionList()
{
	return Sections;
}

vector<FCvar*> CCvar::GetCvarList( FSection* Section )
{
	return Section->Cvar;
}

bool CCvar::FromFile( string Path )
{
	FILE *fp = NULL;
	
	errno_t e = fopen_s( &fp, Path.c_str(), "r" );

	if( fp == NULL || e != 0 ) return false;

	char pszLine[ 2048 ] = { 0 };

	memset( pszLine, 0, 2048 );

	while( fgets( pszLine, 2048, fp ) != NULL )
	{
		char pszSectionName[ 256 ] = { 0 };
		char pszVariablName[ 256 ] = { 0 };
		char pszVariablUnit[ 256 ] = { 0 };

		bool FinishedSection = false, FinishedVarName = false;

		for( int i = 0, CurrentStringIdx = 0; i < ( int )strlen( pszLine ); i++ )
		{
			if( pszLine[ i ] == ' ' )
				continue;

			if( FinishedSection == false )
			{
				if( pszLine[ i ] == '.' )
				{
					pszSectionName[ CurrentStringIdx ] = '\0';

					CurrentStringIdx = 0;

					FinishedSection = true;

					continue;
				}
			}
			else
			{
				if( pszLine[ i ] == '=' )
				{
					pszVariablName[ CurrentStringIdx ] = '\0';

					CurrentStringIdx = 0;

					FinishedVarName = true;

					continue;
				}
			}

			if( FinishedSection == false )
			{
				pszSectionName[ CurrentStringIdx ] = pszLine[ i ];
			}
			else if( FinishedVarName == false )
			{
				pszVariablName[ CurrentStringIdx ] = pszLine[ i ];
			}
			else
			{
				pszVariablUnit[ CurrentStringIdx ] = pszLine[ i ];
			}

			CurrentStringIdx++;
		}

		float flValue = ( float )atof( pszVariablUnit );

		FSection *pSection = GetSectionByName( pszSectionName );

		if( pSection == NULL )
			continue;

		FCvar *pCvar = pSection->GetCvarByName( pszVariablName );

		if( pCvar == NULL )
			continue;

		pCvar->Value = flValue;
	}

	fclose( fp );

	return true;
}

bool CCvar::ToFile( string Path )
{
	FILE *fp = NULL;
	
	errno_t e = fopen_s( &fp, Path.c_str(), "w" );

	if( fp == NULL || e != 0 )
		return false;

	for( int i = 0; i < GetSectionCount(); i++ )
	{
		FSection *pSection = GetSectionByIndex( i );

		if( pSection == NULL )
			continue;

		for( int c = 0; c < pSection->GetCvarCount(); c++ )
		{
			FCvar *pCvar = pSection->GetCvarByIndex( c );

			if( pCvar == NULL )
				continue;

			char pszLineToAdd[ 2048 ] = { 0 };

			sprintf_s( pszLineToAdd, 2048, "%s.%s=%f\n", pSection->GetName().c_str(), pCvar->NativeName.c_str(), pCvar->Float() );

			fwrite( pszLineToAdd, strlen( pszLineToAdd ) * sizeof( char ), 1, fp );
		}
	}

	fclose( fp );

	return true;
}