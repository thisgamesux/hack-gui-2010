#include "stdafx.h"
#include "CSS_Panel.h"

cGui			GGui;
CSchemeLoader	GSchemeLoader;
cFillArea		GFill;
cDrawText		GText;

CCSSLayer::CCSSLayer()
{
	this->Panel = NULL;
}

CCSSLayer::~CCSSLayer()
{
	Destroy();
}

void CCSSLayer::Create( vgui::VPANEL parent )
{
	this->Panel = new CCSSPanel( parent );

	Init_Cvar();

	Init_Gui();
}

void CCSSLayer::Destroy( void )
{
	if( this->Panel )
	{
		this->Panel->SetParent( (vgui::Panel *)NULL );

		delete this->Panel;

		this->Panel = NULL;
	}
}

CCSSPanel::CCSSPanel( vgui::VPANEL parent ) : BaseClass( NULL, "CSSPanel" )
{
	SetParent( parent );
	SetPos( 0, 0 );
	SetVisible( true );
	SetCursor( null );
	GEngine->GetScreenSize( ScreenWidth, ScreenHeight );
	SetSize( ScreenWidth, ScreenHeight );
}

void CCSSPanel::RenderESP( void )
{
	CBaseEntity *pLocal = GBase.GetLocalEntity();

	if( pLocal == NULL )
		return;
		
	// Figure it out yourself, loser
	// But I will tell you that 
	// Core::Radar::AddEntity( pBaseEntity );
	// Goes here if you use the radar stuff
}

void CCSSPanel::RenderMenu( void )
{
	GGui.RenderGui();
}

void CCSSPanel::Paint( void )
{
	if( Var_DynamicReady() == false
		|| GEngine->IsConnected() == false
		|| GEngine->IsInGame() == false
		|| GEngine->Con_IsVisible() == true
		|| GEngine->IsLevelMainMenuBackground() == true )
		return;

	RenderESP();
	RenderMenu();

	return;
}

vgui::Panel* CCSSPanel::HasHotkey( wchar_t key )
{
	return BaseClass::HasHotkey( key );
}

void CCSSPanel::OnKeyTyped( wchar_t key )
{
	return BaseClass::OnKeyTyped( key );
}

// We fill in the empty functions
void cFillArea::Fill( int x, int y, int w, int h, cGuiColor *col )
{
	GRender.FillRGBA( x, y, w, h, col->GetR(), col->GetG(), col->GetB(), col->GetA() );
}

void cFillArea::Fill_Fast( int x, int y, int w, int h, cGuiColor *col )
{
	Fill( x, y, w, h, col );
}

void cDrawText::RenderText( int x, int y, cGuiColor color, char *szString )
{
	GRender.TextA( ( HFont ) 17, x, y, color.GetR(), color.GetG(), color.GetB(), color.GetA(), szString );
}

void cDrawText::DrawText( int x, int y, cGuiColor color, char *szString, ... )
{
	va_list va_alist;
	char logBuffer[1024] = {0};

	va_start( va_alist, szString );

	_vsnprintf( logBuffer + strlen( logBuffer ),
		sizeof( logBuffer ) - strlen( logBuffer ),
		szString, va_alist );

	va_end( va_alist );

	RenderText( x, y, color, logBuffer );
}

void cDrawText::DrawOutline( int x, int y, cGuiColor color, cGuiColor outline, char *szString, ... )
{
	va_list va_alist;
	char logBuffer[1024] = {0};

	va_start( va_alist, szString );

	_vsnprintf( logBuffer + strlen( logBuffer ),
		sizeof( logBuffer ) - strlen( logBuffer ),
		szString, va_alist );

	va_end( va_alist );

	RenderText( x - 1, y - 1, outline, logBuffer );
	RenderText( x + 1, y + 1, outline, logBuffer );
	RenderText( x - 1, y + 1, outline, logBuffer );
	RenderText( x + 1, y - 1, outline, logBuffer );
	RenderText( x, y, color, logBuffer );
}

void cDrawText::DrawShadow( int x, int y, cGuiColor color, cGuiColor shadow, char *szString, ... )
{
	va_list va_alist;
	char logBuffer[1024] = {0};

	va_start( va_alist, szString );

	_vsnprintf( logBuffer + strlen( logBuffer ),
		sizeof( logBuffer ) - strlen( logBuffer ),
		szString, va_alist );

	va_end( va_alist );

	RenderText( x - 1, y + 1, shadow, logBuffer );
	RenderText( x, y, color, logBuffer );
}
