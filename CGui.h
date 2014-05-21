#ifndef C_GUI_H
#define C_GUI_H

#include <windows.h>
#include <winable.h>
#include "CCvar.h"
#include <vector>
#include <list>
#include <Shlwapi.h>
using namespace std;

#pragma comment( lib, "Shlwapi.lib" )

#pragma warning( disable : 4238 )

#define GUIHANDLE			DWORD

//might check these later..
#define D3DCOLOR_RED(COL)	((int)(COL >> 16) & 0xFF)
#define D3DCOLOR_GREEN(COL)	((int)(COL >> 8) & 0xFF)
#define D3DCOLOR_BLUE(COL)	((int)(COL & 0xFF))
#define D3DCOLOR_ALPHA(COL)	((int)(COL >> 24))

typedef struct {
	int x, y, width, height;
} viewport_t;

typedef enum {
	DT_CENTERED = 0,
	DT_SHADOW	= 1,
	DT_OUTLINE	= 2,
	DT_NORMAL	= 0xFF //no effects..
} drawtextflags_t;

class		cWindow;
class		cStatusBar;
class		cCheckbox;
class		cButton;
class		cCaption;
class		cListbox;
class		cColorSelector;
class		cRenderer;
class		cTimer;

class		cFillArea;
class		cDrawText;

typedef void (*StatusCall_t)( cStatusBar *pStatus );
typedef void (*CheckboxCall_t)( cCheckbox *pCheck );
typedef void (*ButtonCall_t)( cButton *pButton );
typedef void (*CaptionCall_t)( cCaption *pCaption );
typedef void (*ListCall_t)( cListbox *pList );
typedef void (*ColorCall_t)( cColorSelector* pSelect );
typedef void (*RendererCall_t)( cRenderer* pRender );
typedef void (*RendererDraw_t)( cRenderer* pRender, int x, int y, cFillArea *fill, cDrawText* text );
typedef void (*TimerCall_t)( cWindow *pWindow, cTimer *pTimer );

class cSquareVector
{
public:
	cSquareVector(void){}
	cSquareVector( int x, int y, int w, int h )
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	int GetLeft(){ return x; }
	int GetRight(){ return x + w; }
	int GetTop(){ return y; }
	int GetBottom(){ return y + h; }

	bool IsInVector( int xx, int yy )
	{
		//is something in this vectors bounds...?
		return ( xx >= GetLeft() && xx <= GetRight() 
			&& yy >= GetTop() && yy <= GetBottom() );
	}

	int GetX(){ return x; }
	int GetY(){ return y; }
	int GetWidth(){ return w; }
	int GetHeight(){ return h; }

	void SetX( int xx ){ x = xx; }
	void SetY( int yy ){ y = yy; }
	void SetWidth( int ww ){ w = ww; }
	void SetHeight( int hh ){ h = hh; }

//private:
	int x, y, w, h;
};

#define HLSMAX		240
#define RGBMAX		255
#define UNDEFINED	(HLSMAX*2/3)

class cGuiColor
{
public:
	cGuiColor(void){}
	cGuiColor( int r, int g, int b, int a ){ m_r = r; m_g = g; m_b = b; m_a = a; }

	int GetR(){ return m_r; }
	int GetG(){ return m_g; }
	int GetB(){ return m_b; }
	int GetA(){ return m_a; }

	void SetR(int r){ m_r = r; }
	void SetG(int g){ m_g = g; }
	void SetB(int b){ m_b = b; }
	void SetA(int a){ m_a = a; }

	cGuiColor GetDarker( WORD dark )
	{
		//we need to darken the color....
		WORD hue, lum, sat;
		GetHLS( hue, lum, sat, m_a );

		lum = ((lum - dark)>=0) ? (lum - dark) : 0;

		return GetFromHLS( hue, lum, sat, m_a );
	}

	cGuiColor GetBrighter( WORD bright )
	{
		//lighten..
		WORD hue, lum, sat;
		GetHLS( hue, lum, sat, m_a );

		lum = ((lum + bright)<=HLSMAX) ? (lum + bright) : 0;

		return GetFromHLS( hue, lum, sat, m_a );
	}

	void GetHLS( WORD &hue, WORD &lum, WORD &sat, int &alpha )
	{
		alpha = m_a;

		BYTE cMax, cMin;
		WORD R = static_cast<WORD>(m_r), G = static_cast<WORD>(m_g), B = static_cast<WORD>(m_b);
		WORD Rdelta, Gdelta, Bdelta;
		cMax = (BYTE)max( max( R, G ), B );
		cMin = (BYTE)min( min( R, G ), B );
		lum = ( ( ( cMax + cMin ) * HLSMAX ) + RGBMAX ) / ( 2 * RGBMAX );
		
		if( cMax == cMin )
		{
			sat = 0;
			hue = UNDEFINED;
			return;
		}
		else
		{
			if( lum <= ( HLSMAX / 2 ) )
			{
				sat = ( ( ( cMax - cMin ) * HLSMAX ) + 
					( ( cMax + cMin ) / 2 ) ) / ( cMax + cMin );
			}
			else
			{
				sat = ( ( ( cMax - cMin ) * HLSMAX ) + 
					( ( 2 * RGBMAX - cMax - cMin ) / 2 ) ) / ( 2 * RGBMAX - cMax - cMin);
			}

			Rdelta = ( ((cMax-R)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
			Gdelta = ( ((cMax-G)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
			Bdelta = ( ((cMax-B)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

			if ( R == cMax )
			{
				hue = Bdelta - Gdelta;
			}
			else if( G == cMax )
			{
				hue = (HLSMAX/3) + Rdelta - Bdelta;
			}
			else
			{
				hue = ((2*HLSMAX)/3) + Gdelta - Rdelta;
			}

			if (hue < 0)
				hue += HLSMAX;

			if (hue > HLSMAX)
				hue -= HLSMAX;
		}
	}

	cGuiColor GetFromHLS( WORD hue, WORD lum, WORD sat, int alpha )
	{
		WORD R, G, B;
		WORD Magic1, Magic2;
		if( sat == 0 )
		{
			R = G = B = ( lum * RGBMAX ) / HLSMAX;
			if( hue != UNDEFINED ){
				//error...
			}
		}else{
			if (lum <= (HLSMAX/2))
			{
				Magic2 = (lum*(HLSMAX + sat) + (HLSMAX/2))/HLSMAX;
			}else{
				Magic2 = lum + sat - ((lum*sat) + (HLSMAX/2))/HLSMAX;
			}
			Magic1 = 2 * lum - Magic2;

			R = (HueToRGB(Magic1,Magic2,hue+(HLSMAX/3))*RGBMAX +(HLSMAX/2))/HLSMAX;
			G = (HueToRGB(Magic1,Magic2,hue)*RGBMAX + (HLSMAX/2)) / HLSMAX;
			B = (HueToRGB(Magic1,Magic2,hue-(HLSMAX/3))*RGBMAX + (HLSMAX/2))/HLSMAX;
		}

		return cGuiColor( R, G, B, alpha );
	}

	WORD HueToRGB( WORD n1, WORD n2, WORD hue )
	{
		if( hue < 0 )
			hue += HLSMAX;

		if( hue > HLSMAX )
			hue -= HLSMAX;

		if (hue < (HLSMAX/6))
			return ( n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)) );
		if (hue < (HLSMAX/2))
			return ( n2 );
		if (hue < ((HLSMAX*2)/3))
			return ( n1 +    (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6)));
		
		return ( n1 );
	}

	int m_r, m_g, m_b, m_a;
};

class cPercentile
{
public:
	cPercentile(void){}
	cPercentile(int percent){m_iPercent = percent;}

	int GetValue( int max_number, int percent, int max_percent ){
		return ( max_percent > 0 ) ? ( percent * max_number ) / max_percent : 0;
	}

	int GetPercent( int currentvar, int maxvar, int max_percent ){
		return ( currentvar > 0 ) ? ( ( currentvar * max_percent ) / maxvar ) : 0;
	}

	void SetPercent( int var, int max, int max_percent ){
		m_iPercent = ( var > 0 ) ? ( ( var * max_percent ) / max ) : 0;
	}

	int	m_iPercent;
};

class cFillArea
{
public:
	void Fill( int x, int y, int w, int h, cGuiColor *col );
	void Fill_Fast( int x, int y, int w, int h, cGuiColor *col );
	
	void Fill( int x, int y, int w, int h, int r, int g, int b, int a )
	{
		Fill( x, y, w, h, &cGuiColor( r, g, b, a ) );
	}
	
	void Fill_Fast( int x, int y, int w, int h, int r, int g, int b, int a )
	{
		Fill_Fast( x, y, w, h, &cGuiColor( r, g, b, a ) );
	}

	void FillOutline( int x, int y, int w, int h, cGuiColor *col )
	{
		Fill( x, y, w, 1, col );
		Fill( x, y, 1, h, col );
		Fill( x, y + h, w+1, 1, col );
		Fill( x + w, y, 1, h, col );
	}

	void FillOutline( int x, int y, int w, int h, int r, int g, int b, int a )
	{
		FillOutline( x, y, w, h, &cGuiColor( r, g, b, a ) );
	}

	void FillGradient( int x, int y, int w, int h, cGuiColor *start, cGuiColor *end, bool fast = false )
	{
		float rDif = ((float)end->GetR()-(float)start->GetR())/h;
		float gDif = ((float)end->GetG()-(float)start->GetG())/h;
		float bDif = ((float)end->GetB()-(float)start->GetB())/h;
		float aDif = ((float)end->GetA()-(float)start->GetA())/h;

		for( int i = 0; i < h; i++ )
		{
			cGuiColor col(
				start->GetR() + ((int)rDif * i),
				start->GetG() + ((int)gDif * i),
				start->GetB() + ((int)bDif * i),
				start->GetA() + ((int)aDif * i) );

			if( fast ){ 
				Fill_Fast( x, y + i, w, 1, &col ); 
				continue; 
			}

			Fill( x, y + i, w, 1, &col );			
		}
	}

	void FillRounded( int x, int y, int w, int h, cGuiColor *col )
	{
		FillSmallerLine( x, y, w, 4, col );
		FillSmallerLine( x, y+1, w, 2, col );
		FillSmallerLine( x, y+2, w, 1, col );
		FillSmallerLine( x, y+3, w, 1, col );
		Fill( x, y + 4, w, h - 4, col );
		FillSmallerLine( x, y + h, w, 1, col );
		FillSmallerLine( x, y + h+1, w, 1, col );
		FillSmallerLine( x, y + h+2, w, 2, col );
		FillSmallerLine( x, y + h+3, w, 4, col );
	}

	void FillRoundedGradient( int x, int y, int w, int h, cGuiColor *start, cGuiColor *end )
	{
		float rDif = ((float)end->GetR()-(float)start->GetR())/h;
		float gDif = ((float)end->GetG()-(float)start->GetG())/h;
		float bDif = ((float)end->GetB()-(float)start->GetB())/h;
		float aDif = ((float)end->GetA()-(float)start->GetA())/h;
		
		FillSmallerLine( x, y, w, 4, &cGuiColor( start->GetR() + ((int)rDif * 0), 
			start->GetG() + ((int)gDif * 0), start->GetB() + ((int)bDif * 0), 
			start->GetA() + ((int)aDif * 0) ) );

		FillSmallerLine( x, y+1, w, 2, &cGuiColor( start->GetR() + ((int)rDif * 1), 
			start->GetG() + ((int)gDif * 1), start->GetB() + ((int)bDif * 1), 
			start->GetA() + ((int)aDif * 1) ) );

		FillSmallerLine( x, y+2, w, 1, &cGuiColor( start->GetR() + ((int)rDif * 2), 
			start->GetG() + ((int)gDif * 2), start->GetB() + ((int)bDif * 2), 
			start->GetA() + ((int)aDif * 2) ) );

		FillSmallerLine( x, y+3, w, 1, &cGuiColor( start->GetR() + ((int)rDif * 3), 
			start->GetG() + ((int)gDif * 3), start->GetB() + ((int)bDif * 3), 
			start->GetA() + ((int)aDif * 3) ) );

		for( int i = 4; i < h-3; i++ )
		{
			cGuiColor col(
				start->GetR() + ((int)rDif * i),
				start->GetG() + ((int)gDif * i),
				start->GetB() + ((int)bDif * i),
				start->GetA() + ((int)aDif * i) );

			Fill( x, y + i, w, 1, &col );
		}

		FillSmallerLine( x, y + h, w, 4, &cGuiColor( start->GetR() + ((int)rDif * h), 
			start->GetG() + ((int)gDif * h), start->GetB() + ((int)bDif * h), 
			start->GetA() + ((int)aDif * h) ) );

		FillSmallerLine( x, y + h - 1, w, 2, &cGuiColor( start->GetR() + ((int)rDif * (h - 1)), 
			start->GetG() + ((int)gDif * (h - 1)), start->GetB() + ((int)bDif * (h - 1)), 
			start->GetA() + ((int)aDif * (h - 1)) ) );

		FillSmallerLine( x, y + h - 2, w, 1, &cGuiColor( start->GetR() + ((int)rDif * (h - 2)), 
			start->GetG() + ((int)gDif * (h - 2)), start->GetB() + ((int)bDif * (h - 2)), 
			start->GetA() + ((int)aDif * (h - 2)) ) );

		FillSmallerLine( x, y + h - 3, w, 1, &cGuiColor( start->GetR() + ((int)rDif * (h - 3)), 
			start->GetG() + ((int)gDif * (h - 3)), start->GetB() + ((int)bDif * (h - 3)), 
			start->GetA() + ((int)aDif * (h - 3)) ) );
	}

private:

	void FillSmallerLine( int x, int y, int w, int smaller, cGuiColor *col )
	{
		x += smaller;
		w -= ( smaller * 2 );
		Fill( x, y, w, 1, col );
	}
};

class cDrawText
{
public:
	void RenderText( int x, int y, cGuiColor color, char *szString );
	void DrawText( int x, int y, cGuiColor color, char *szString, ... );
	void DrawOutline( int x, int y, cGuiColor color, cGuiColor outline, char *szString, ... );
	void DrawShadow( int x, int y, cGuiColor color, cGuiColor shadow, char *szString, ... );
};

class cColorScheme
{
public:

	cColorScheme(void){}

	cColorScheme( cGuiColor back, cGuiColor fore, cGuiColor outline )
	{BackColor = back; ForeColor = fore; Outline = outline;}

	cColorScheme( cGuiColor back, cGuiColor fore )
	{BackColor = back; ForeColor = fore;}

	cGuiColor *GetForeColor(){ return &ForeColor; }
	cGuiColor *GetBackColor(){ return &BackColor; }
	cGuiColor *GetOutline(){ return &Outline; }

	void SetForeColor( cGuiColor col ){ ForeColor = col; }
	void SetBackColor( cGuiColor col ){ BackColor = col; }
	void SetOutline( cGuiColor col ){ Outline = col; }

private:

	cGuiColor	BackColor;
	cGuiColor	ForeColor;
	cGuiColor	Outline;
};

class cGuiObject
{
public:

	cGuiObject(void){}

	virtual void SetScheme( cColorScheme *scheme )
	{
		m_scheme = *scheme;
	}

	virtual void Size( int w, int h )
	{
		this->m_bounds.w = w;
		this->m_bounds.h = h;
	}

	virtual void Move( int x, int y )
	{
		this->m_bounds.x = x;
		this->m_bounds.y = y;
	}

	virtual void Draw( cFillArea *fill, cDrawText* text )
	{
		return;
	}

	cSquareVector	m_bounds;
	cColorScheme	m_scheme;
};

class cStatusBar : public cGuiObject
{
public:

	cWindow*		m_Parent;
	cPercentile		m_Percent;
	float			m_flMaxPercent;
	string			m_caption;
	StatusCall_t	m_Callback;
	cSquareVector	m_LastClickSpace;

	void Draw( cFillArea *fill, cDrawText* text );
};

class cCheckbox : public cGuiObject
{
public:

	cWindow*		m_Parent;
	string			m_caption;
	FCvar*			m_pPointerVar;
	CheckboxCall_t	m_Callback;

	void Draw( cFillArea *fill, cDrawText* text );
};

class cButton : public cGuiObject
{
public:

	cWindow*		m_Parent;
	DWORD			m_dwLastClickTick;
	string			m_caption;
	ButtonCall_t	m_Callback;
	bool			m_bHighlighted;

	void Draw( cFillArea *fill, cDrawText* text );
};

class cCaption : public cGuiObject
{
public:

	cWindow*		m_Parent;
	string			m_caption;
	CaptionCall_t	m_Callback;

	void Draw( cFillArea *fill, cDrawText* text );
};

class cListbox : public cGuiObject
{
public:
	cWindow*		m_Parent;
	int				m_beginidx;
	int				m_selected;
	string			m_title;
	vector<string>	m_options;
	ListCall_t		m_Callback;

	bool			m_bDisplayUD;

	void ClearOptions()
	{
		m_options.clear();
	}

	void AddOption( string OptionName )
	{
		m_options.push_back( OptionName );
	}

	bool IsInVisibleRange( int index )
	{
		int range_start = ( m_beginidx * 14 );
		int range_end	= range_start + m_bounds.h;
		int index_pos	= ( index * 14 );

		return ( 
			index_pos >= range_start 
			&& index_pos <= range_end 
			&& ( index_pos + 14 ) <= range_end );
	}

	void Draw( cFillArea *fill, cDrawText* text );
};

class cColorSelector : public cGuiObject
{
public:
	cWindow*		m_Parent;
	string			m_title;
	cGuiColor		m_SelectedColor;
	ColorCall_t		m_Callback;
	WORD			m_SelectedLum;

	void Draw( cFillArea *fill, cDrawText* text );
};

class cTextbox : public cGuiObject
{
public:
	cWindow*		m_Parent;
	vector<char>	m_string;
	int				m_iTypeIndex;
	int				m_iRenderIndex;
	bool			m_bIsEditing;
	string			m_name;

	string GetText()
	{
		string ret;
		for( int i = 0; i < (int)m_string.size(); i++ )
			ret += m_string[i];

		return ret;
	}

	void Input( char c ){ m_string.push_back( c ); }
	void Backspace(){ if(m_string.size())m_string.pop_back(); }
	void Draw( cFillArea *fill, cDrawText* text );
};

class cTextArea : public cGuiObject
{
public:
};

class cRenderer : public cGuiObject
{
public:
	cWindow*		m_Parent;
	RendererDraw_t	m_OnRender;
	RendererCall_t	m_OnClick;

	void Draw( cFillArea *fill, cDrawText* text );
};

class cTimer {
public:
	cWindow*		m_Parent;
	DWORD			m_dwInterval;
	DWORD			m_dwLastRun;
	TimerCall_t		m_Callback;
};

class cWindow : public cGuiObject
{
public:

	cWindow(void){}
	
	string					m_title;
	bool					m_bDrawWhenInactive;
	cSquareVector			m_titlebar;

	list<cStatusBar>		m_vStatus;
	list<cCheckbox>			m_vCheck;
	list<cButton>			m_vButton;
	list<cCaption>			m_vCaption;
	list<cListbox>			m_vListBox;
	list<cColorSelector>	m_vColorSelector;
	list<cTextbox>			m_vTextBox;
	list<cRenderer>			m_vRenderer;
	vector<cTimer>			m_vTimer;

	cStatusBar*				AddStatusBar( string caption, cColorScheme scheme, int x, int y, int w, int h, StatusCall_t StatusCallback, int iPercent = 0, bool bUpdateInit = true, float flMaxPercent = 100.0f );
	cCheckbox*				AddCheckbox( string caption, cColorScheme scheme, int x, int y, FCvar* Value, CheckboxCall_t CheckCall = NULL );
	cButton*				AddButton( string caption, cColorScheme scheme, int x, int y, int w, int h, ButtonCall_t ButtonCallback );
	cCaption*				AddCaption( string caption, cColorScheme scheme, int x, int y, CaptionCall_t CaptionCallback = NULL );
	cListbox*				AddListbox( string caption, cColorScheme scheme, int x, int y, int w, int h, ListCall_t ListCallback = NULL, bool DisplayUpDown = true );
	cColorSelector*			AddColorSelector( string caption, int x, int y, int w, int h, ColorCall_t ColorCallback = NULL );
	cTextbox*				AddTextbox( string name, cColorScheme scheme, int x, int y, int w );
	cRenderer*				AddRenderer( int x, int y, RendererDraw_t OnRender );
	cTimer*					AddTimer( TimerCall_t TickCallback, DWORD dwInterval = 100 );

	cStatusBar*				FindStatusBarByName( string name );
	cCheckbox*				FindCheckboxByName( string name );
	cButton*				FindButtonByName( string name );
	cCaption*				FindCaptionByName( string name );
	cListbox*				FindListboxByName( string name );
	cColorSelector*			FindColorSelectorByName( string name );
	cTextbox*				FindTextboxByName( string name );

	cStatusBar*				GetStatusBarByName( string name );
	cCheckbox*				GetCheckboxByName( string name );
	cButton*				GetButtonByName( string name );
	cCaption*				GetCaptionByName( string name );
	cListbox*				GetListboxByName( string name );
	cColorSelector*			GetColorSelectorByName( string name );
	cTextbox*				GetTextboxByName( string name );

	void					PrepareDelete();
	void					ClearEditingStates();

	void					Draw( cFillArea *fill, cDrawText* text, bool bIsSelectedWindow );
};

typedef struct {
	bool		m_bDragging;
	cWindow*	m_Window;

	int			m_iWindowXOffset;
	int			m_iWindowYOffset;

} dragging_t;

class cMouse : public cGuiObject
{
public:

	cGuiObject*	GetBaseObject(){ return reinterpret_cast<cGuiObject *>( this ); }

	bool InputKeyData( WPARAM wParams, LPARAM lParams, UINT msg );
	
	void ClearData()
	{
		m_LeftHeld				= false;
		m_RightHeld				= false;
		m_LeftClicked			= false;
		m_RightClicked			= false;
		m_dwLastLeftClickTick	= NULL;
		m_dwLastRightClickTick	= NULL;
	}

	void Draw( cFillArea *fill )
	{
		fill->Fill( m_bounds.x-1,m_bounds.y-1,  1, 12, &m_outline );
		fill->Fill( m_bounds.x, m_bounds.y-1,   1, 1, &m_outline );
		fill->Fill( m_bounds.x, m_bounds.y,     1,10, &m_color );
		fill->Fill( m_bounds.x, m_bounds.y+10,  1, 1, &m_outline );
		fill->Fill( m_bounds.x+1, m_bounds.y,   1, 1, &m_outline );
		fill->Fill( m_bounds.x+1, m_bounds.y+1, 1, 8, &m_color );
		fill->Fill( m_bounds.x+1, m_bounds.y+9, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+2, m_bounds.y+1, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+2, m_bounds.y+2, 1, 6, &m_color );
		fill->Fill( m_bounds.x+2, m_bounds.y+8, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+3, m_bounds.y+2, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+3, m_bounds.y+3, 1, 6, &m_color );
		fill->Fill( m_bounds.x+3, m_bounds.y+9, 1, 2, &m_outline );
		fill->Fill( m_bounds.x+4, m_bounds.y+3, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+4, m_bounds.y+4, 1, 7, &m_color );
		fill->Fill( m_bounds.x+4,m_bounds.y+11, 1, 2, &m_outline );
		fill->Fill( m_bounds.x+5, m_bounds.y+4, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+5, m_bounds.y+5, 1, 2, &m_color );
		fill->Fill( m_bounds.x+5,m_bounds.y+7,  1, 2, &m_outline );
		fill->Fill( m_bounds.x+5,m_bounds.y+9,  1, 4, &m_color );
		fill->Fill( m_bounds.x+5,m_bounds.y+13, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+6, m_bounds.y+5, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+6, m_bounds.y+6, 1, 1, &m_color );
		fill->Fill( m_bounds.x+6, m_bounds.y+7, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+6, m_bounds.y+9, 1, 2, &m_outline );
		fill->Fill( m_bounds.x+6,m_bounds.y+11, 1, 2, &m_color );
		fill->Fill( m_bounds.x+6,m_bounds.y+13, 1, 1, &m_outline );
		fill->Fill( m_bounds.x+7, m_bounds.y+6, 1, 2, &m_outline );
		fill->Fill( m_bounds.x+7,m_bounds.y+11, 1, 2, &m_outline );
	}

	void InvalidateClickStates()
	{
		m_LeftClicked	= false;
		m_RightClicked	= false;
	}

	bool		m_LeftHeld,
				m_RightHeld,
				m_LeftClicked,
				m_RightClicked;

	dragging_t	m_Dragging;

	DWORD		m_dwLastLeftClickTick,
				m_dwLastRightClickTick;

	bool		m_bDrawMouseInfo;

	cGuiColor	m_color, m_outline;
};

class cGui
{
public:
	cGui();
	~cGui();

	cWindow*	AddWindow( string title, cColorScheme scheme, 
		int x, int y, int w, int h, bool bDrawWhenInactive = false );

	void		DoFrame( cWindow *pWindow );
	void		HandleMovement();
	bool		UpdateKeyStates( WPARAM wParams, LPARAM lParams, UINT msg );
	void		RenderGui();

	//accessors..
	cMouse*		GetMouse(){ return &m_mouse; }
	cFillArea*	GetFill(){ return m_fill; }
	cDrawText*	GetText(){ return m_text; }

	void		SetFill( cFillArea* fill ){ m_fill = fill; }
	void		SetText( cDrawText* text ){ m_text = text; }

	cWindow* MoveToFront( list<cWindow>::iterator index )
	{
		if( index == m_vWindows.begin() )
		{
			return &(m_vWindows.front());
		}

		vector<cTimer> vStoreTimers;
		for( int i = 0; i < (int)(*index).m_vTimer.size(); i++ )
		{
			vStoreTimers.push_back( (*index).m_vTimer[ i ] );
		}

		m_vWindows.push_front( *index );
		m_vWindows.erase( index );
		
		cWindow *pParent = &(m_vWindows.front());

#define ASSIGN_PARENT(t,l) for(list<t>::iterator v##t = m_vWindows.front().l.begin();		\
		v##t != m_vWindows.front().l.end(); ++v##t ){										\
			t *p##t = &(*v##t);																\
			p##t->m_Parent = pParent;														\
		}

		ASSIGN_PARENT( cStatusBar,		m_vStatus );
		ASSIGN_PARENT( cCheckbox,		m_vCheck );
		ASSIGN_PARENT( cButton,			m_vButton );
		ASSIGN_PARENT( cCaption,		m_vCaption );
		ASSIGN_PARENT( cListbox,		m_vListBox );
		ASSIGN_PARENT( cColorSelector,	m_vColorSelector );
		ASSIGN_PARENT( cTextbox,		m_vTextBox );
		ASSIGN_PARENT( cRenderer,		m_vRenderer );

		for( int i = 0; i < (int)vStoreTimers.size(); i++ )
		{
			pParent->m_vTimer.push_back( vStoreTimers[ i ] );
		}

		return &(m_vWindows.front());
	}

	cWindow *FindWindow( std::string Title )
	{
		for( list<cWindow>::iterator i = m_vWindows.begin(); i != m_vWindows.end(); ++i ){
			cWindow *pCurrent = &(*i);

			if( pCurrent ){
				if( strstr( Title.c_str(), pCurrent->m_title.c_str() ) ){
					return pCurrent;
				}
			}
		}

		return NULL;
	}

	cWindow *GetWindow( std::string Title )
	{
		for( list<cWindow>::iterator i = m_vWindows.begin(); i != m_vWindows.end(); ++i ){
			cWindow *pCurrent = &(*i);

			if( pCurrent ){
				if( pCurrent->m_title.compare( Title ) == 0 ){
					return pCurrent;
				}
			}
		}

		return NULL;
	}

	void RemoveWindow( cWindow *pWindow )
	{
		if( !pWindow )
			return;

		for( list<cWindow>::iterator i = m_vWindows.begin(); i != m_vWindows.end(); ++i ){
			
			cWindow *pCurrent = &(*i);

			if( pCurrent ){
				if( pCurrent == pWindow ){
					pCurrent->PrepareDelete();
					m_vWindows.erase( i );
					return;
				}
			}
		}
	}

	list<cWindow> GetWindowList(){ return m_vWindows; }

	bool IsActive(){ return m_bActive; }

	void SetActive( bool active ){ m_bActive = active; }

private:

	bool				m_bActive;
	cMouse				m_mouse;
	cFillArea*			m_fill;
	cDrawText*			m_text;

public:
	list<cWindow>		m_vWindows;
};

class CSchemeLoader
{
public:

	void			LoadSchemeInformation();
	void			SaveSchemeInformation(cColorScheme *window, cColorScheme *button, cColorScheme *check, cColorScheme *list, cColorScheme *status, cColorScheme *text);

	cColorScheme*	GetWindowScheme();
	cColorScheme*	GetButtonScheme();
	cColorScheme*	GetCheckScheme();
	cColorScheme*	GetListboxScheme();
	cColorScheme*	GetStatusScheme();
	cColorScheme*	GetTextScheme();


	// Generic functions

	cColorScheme	*GetSchemeFromFile( string File );

	void			SaveSchemeToFile( string File, cColorScheme *scheme );

private:

	cColorScheme	*m_pWindow, *m_pButton, *m_pCheck, *m_pListbox, *m_pStatus, *m_pText;
};

extern cGui				GGui;
extern CSchemeLoader	GSchemeLoader;

#endif