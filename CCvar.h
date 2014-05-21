#ifndef __CCVAR_HEADER__
#define __CCVAR_HEADER__

#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#define INVALID_SECTION_INDEX	-1
#define INVALID_CVAR_INDEX		-1

typedef struct {
	string 				NativeName;
	string 				DisplayName;
	string 				DisplayDesc;
	vector<string>		DisplayClean;
	float 				min;
	float 				max;
	float 				Value;

	void LoadCleanArray( const char **StringArray )
	{
		LoadCleanArray( ( char** )StringArray );
	}

	void LoadCleanArray( char **StringArray )
	{
		if( StringArray == NULL ) return;

		vector< string > Display;

		for( int i = 0; i < static_cast< int >( max ); i++ )
		{
			Display.push_back( StringArray[ i ] );
		}

		LoadCleanArray( Display );
	}

	void LoadCleanArray( vector< string > StringArray )
	{
		if( max == 0.f ) return;

		DisplayClean.clear();

		DisplayClean = StringArray;
	}

	void Inc( float f )
	{
		float fNext = ( Value + f );

		if( fNext > max )
		{
			return;
		}
		
		Value = fNext;
	}

	void Dec( float f )
	{
		float fPrev = ( Value - f );

		if( fPrev < min )
		{
			return;
		}

		Value = fPrev;
	}

	float Float()
	{
		return Value;
	}

	int Int32()
	{
		return static_cast< int >( Float() );
	}

	bool Boolean()
	{
		return ( Value != 0.f );
	}

} FCvar;

typedef struct {
	string 			Name;
	vector<FCvar*> 	Cvar;

	string GetName()
	{
		return Name;
	}

	int	GetCvarCount()
	{
		return static_cast< int >( Cvar.size() );
	}

	FCvar* GetCvarByIndex( int index )
	{
		return Cvar[ index ];
	}

	FCvar* GetCvarByName( string CvarName )
	{
		for( int i = 0; i < GetCvarCount(); i++ )
		{
			if( Cvar[ i ] == 0 ) continue;

			if( CvarName.compare( Cvar[ i ]->NativeName ) == 0 || CvarName.compare( Cvar[ i ]->DisplayName ) == 0 )
			{
				return GetCvarByIndex( i );
			}
		}

		static FCvar Blank;

		memset( &Blank, 0, sizeof( FCvar ) );

		return &Blank;
	}

} FSection;

class CCvar
{
public:

	int							AddSection( string Name );

	int							AddCvar( string SectionName, string Name, string Clean, string Desc, float Value = 0.f, float Min = 0.f, float Max = 1.f );
	int							AddCvar( int SectionIndex, string Name, string Clean, string Desc, float Value = 0.f, float Min = 0.f, float Max = 1.f );
	int							AddCvar( FSection* Section, string Name, string Clean, string Desc, float Value = 0.f, float Min = 0.f, float Max = 1.f );

	FSection*					GetSectionByName( string Name );
	FSection*					GetSectionByIndex( int i );

	int							GetSectionCount();
	int							GetCvarCount( FSection* Section );

	vector<FSection*>			GetSectionList();
	vector<FCvar*>				GetCvarList( FSection* Section );

	bool						FromFile( string Path );
	bool						ToFile( string Path );

private:

	vector<FSection*>			Sections;
};

extern CCvar GCvar;

#endif