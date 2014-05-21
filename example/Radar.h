#ifndef __RADAR_HEADER__
#define __RADAR_HEADER__

namespace Core
{
	namespace Radar
	{
		extern void SetW( int w );
		extern void SetH( int h );
		extern void SetX( int x );
		extern void SetY( int y );
		extern void SetB( int x, int y, int w, int h );

		extern void Render( cFillArea *fill, cDrawText* text, float Scale );
		extern void AddEntity( C_BaseEntity* pEntity );
		extern void ClearEntities( void );
	};
};

#endif