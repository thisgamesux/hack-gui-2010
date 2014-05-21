#include "stdafx.h"
#include "Radar.h"

namespace Core
{
	namespace Radar
	{
		int x, y, w, h;

		vector< C_BaseEntity* > RadarEntities;

		bool __IsInSpace( int rx, int ry )
		{
			return ( ( rx > x && ry > y ) && ( rx < ( x + w ) && ry < y + h ) );
		}

		bool __CalcPoint( Vector Origin, int *cx, int *cy, float range )
		{
			C_BaseEntity *pLocal = GBase.GetLocalEntity();

			if( pLocal == NULL )
				return false;

			float dx = Origin.x - GBase.GetEyePosition( pLocal ).x;
			float dy = Origin.y - GBase.GetEyePosition( pLocal ).y;

//			float ya = DEG2RAD( GBase.GetEyeAngles( pLocal ).y );
			float ya = DEG2RAD( pLocal->GetAbsAngles().y );

			float zx = dy * ( -cos( ya ) ) + dx * sin( ya );
			float zy = dx * ( -cos( ya ) ) - dy * sin( ya );

			if( range > 0 )
			{
				zx /= range;
				zy /= range;
			}

			float fx = ( float( x ) + zx ) + float( w / 2 );
			float fy = ( float( y ) + zy ) + float( h / 2 );

			if( __IsInSpace( fx, fy ) == false )
			{
				return false;
			}

			*cx = fx;
			*cy = fy;

			return true;
		}

		void SetW( int rw )
		{
			w = rw;
		}

		void SetH( int rh )
		{
			h = rh;
		}

		void SetX( int rx )
		{
			x = rx;
		}

		void SetY( int ry )
		{
			y = ry;
		}

		void SetB( int rx, int ry, int rw, int rh )
		{
			SetX( rx );
			SetY( ry );
			SetW( rw );
			SetH( rh );
		}

		void Render( cFillArea *fill, cDrawText* text, float Scale )
		{
			fill->Fill( x, y, w, h, 90, 90, 90, 90 );
			fill->FillOutline( x, y, w, h, 0, 0, 0, 255 );
			fill->Fill( x + ( w / 2 ), y, 1, w, 0, 0, 0, 255 );
			fill->Fill( x, y + ( h / 2 ), w, 1, 0, 0, 0, 255 );

			for( int i = 0; i < static_cast< int >( RadarEntities.size() ); i++ )
			{
				C_BaseEntity* BaseEntity = RadarEntities[ i ];

				if( BaseEntity == NULL )
					continue;

				int cx = 0, cy = 0;

				if( __CalcPoint( BaseEntity->GetAbsOrigin(), &cx, &cy, Scale ) == false )
					continue;

				Color TeamColor = GBase.GetTeamColor( BaseEntity, true, false );

				int nx = cx - 3, ny = cy - 3;

				fill->FillOutline( nx, ny, 6, 6, 0, 0, 0, 255 );

				fill->Fill( nx + 1, ny + 1, 5, 5, TeamColor.r(), TeamColor.g(), TeamColor.b(), TeamColor.a() );

				Vector f, r, u;

				Core::Spread::__AngleVectors( GBase.GetEyeAngles( BaseEntity ), &f, &r, &u );

//				AngleVectors( GBase.GetEyeAngles( BaseEntity ), &forward );

				f = GBase.GetEyePosition( BaseEntity ) + f * 300;

				int ex = 0, ey = 0;

				if( __CalcPoint( f, &ex, &ey, Scale ) == false )
					continue;

				GSurface->DrawSetColor( TeamColor.r(), TeamColor.g(), TeamColor.b(), TeamColor.a() );
				GSurface->DrawLine( cx, cy, ex, ey );
			}
		}

		void AddEntity( C_BaseEntity* pEntity )
		{
			RadarEntities.push_back( pEntity );
		}

		void ClearEntities( void )
		{
			RadarEntities.clear();
		}
	};
};