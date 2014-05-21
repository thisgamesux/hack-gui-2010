#include "cGui.h"
#include "App.h"
#include "CFile.h"

cGui::cGui()
{
	//init everything we need!
	this->m_mouse.m_Dragging.m_bDragging	= false;
	this->m_mouse.m_Dragging.m_Window		= NULL;
	this->m_bActive							= true;
	this->m_fill							= NULL;
	this->m_text							= NULL;
	this->m_vWindows.clear();
}

cGui::~cGui()
{
	//destroy..
}

cWindow*	cGui::AddWindow( string title, cColorScheme scheme, 
						  int x, int y, int w, int h, bool bDrawWhenInactive )
{
	cWindow new_window;
	new_window.m_title				= title;
	new_window.m_scheme				= scheme;
	new_window.m_bounds.x			= x;
	new_window.m_bounds.y			= y;
	new_window.m_bounds.w			= w;
	new_window.m_bounds.h			= h;
	new_window.m_bDrawWhenInactive	= bDrawWhenInactive;

	m_vWindows.push_back( new_window );

	return &m_vWindows.back();
}

cStatusBar*	cWindow::AddStatusBar( string caption, cColorScheme scheme,
		int x, int y, int w, int h, StatusCall_t StatusCallback, 
		int iPercent, bool bUpdateInit, float flMaxPercent )
{
	cStatusBar new_bar;
	new_bar.m_caption		= caption;
	new_bar.m_scheme		= scheme;
	new_bar.m_bounds.x		= x;
	new_bar.m_bounds.y		= y;
	new_bar.m_bounds.w		= w;
	new_bar.m_bounds.h		= h;
	new_bar.m_Percent		= iPercent;
	new_bar.m_Parent		= this;
	new_bar.m_Callback		= StatusCallback;
	new_bar.m_flMaxPercent	= flMaxPercent;

	if( new_bar.m_Callback && bUpdateInit )
		new_bar.m_Callback( &new_bar );

	this->m_vStatus.push_back( new_bar );
	return &this->m_vStatus.back();
}

cCheckbox*	cWindow::AddCheckbox( string caption, cColorScheme scheme,
		int x, int y, FCvar* Value, CheckboxCall_t CheckCall )
{
	cCheckbox new_box;
	new_box.m_caption		= caption;
	new_box.m_scheme		= scheme;
	new_box.m_bounds.x		= x;
	new_box.m_bounds.y		= y;
	new_box.m_pPointerVar	= Value;
	new_box.m_Callback		= CheckCall;
	new_box.m_Parent		= this;

	if( new_box.m_Callback )
	{
		new_box.m_Callback( &new_box );
	}

	this->m_vCheck.push_back( new_box );

	return &this->m_vCheck.back();
}

cButton*	cWindow::AddButton( string caption, cColorScheme scheme,
		int x, int y, int w, int h, ButtonCall_t ButtonCallback )
{
	cButton new_button;
	new_button.m_caption	= caption;
	new_button.m_scheme		= scheme;
	new_button.m_bounds.x	= x;
	new_button.m_bounds.y	= y;
	new_button.m_bounds.w	= w;
	new_button.m_bounds.h	= h;
	new_button.m_dwLastClickTick = 0;
	new_button.m_Parent		= this;
	new_button.m_bHighlighted = false;
	new_button.m_Callback	= ButtonCallback;
	this->m_vButton.push_back( new_button );
	return &this->m_vButton.back();
}

cCaption*	cWindow::AddCaption( string caption, cColorScheme scheme,
		int x, int y, CaptionCall_t CaptionCallback )
{
	cCaption new_cap;
	new_cap.m_caption		= caption;
	new_cap.m_scheme		= scheme;
	new_cap.m_bounds.x		= x;
	new_cap.m_bounds.y		= y;
	new_cap.m_Parent		= this;
	new_cap.m_Callback		= CaptionCallback;
	this->m_vCaption.push_back( new_cap );
	return &this->m_vCaption.back();
}

cListbox* cWindow::AddListbox( string caption, cColorScheme scheme, 
						   int x, int y, int w, int h, 
						   ListCall_t ListCallback, bool DisplayUpDown )
{
	cListbox new_list;
	new_list.m_Parent		= this;
	new_list.m_title		= caption;
	new_list.m_scheme		= scheme;
	new_list.m_selected		= 0;
	new_list.m_bounds		= cSquareVector( x, y, w, h );
	new_list.m_Callback		= ListCallback;
	new_list.m_beginidx		= 0;
	new_list.m_bDisplayUD	= DisplayUpDown;
	this->m_vListBox.push_back( new_list );
	return &this->m_vListBox.back();
}

cColorSelector* cWindow::AddColorSelector( string caption, 
		int x, int y, int w, int h, ColorCall_t ColorCallback )
{
	cColorSelector new_color;
	new_color.m_Parent		= this;
	new_color.m_title		= caption;
	new_color.m_bounds.x	= x;
	new_color.m_bounds.y	= y;
	new_color.m_bounds.w	= w;
	new_color.m_bounds.h	= h;
	new_color.m_Callback	= ColorCallback;
	new_color.m_SelectedLum = HLSMAX / 2;
	this->m_vColorSelector.push_back( new_color );
	return &this->m_vColorSelector.back();
}

cTextbox* cWindow::AddTextbox( string name, cColorScheme scheme, int x, int y, int w )
{
	cTextbox new_text;
	new_text.m_Parent		= this;
	new_text.m_bIsEditing	= false;
	new_text.m_scheme		= scheme;
	new_text.m_bounds.x		= x;
	new_text.m_bounds.y		= y;
	new_text.m_bounds.w		= w;
	new_text.m_bounds.h		= 10; //?
	new_text.m_iRenderIndex	= 0;
	new_text.m_iTypeIndex	= 0;
	new_text.m_name			= name;
	new_text.m_string.clear();
	this->m_vTextBox.push_back( new_text );
	return &this->m_vTextBox.back();
}

cRenderer* cWindow::AddRenderer( int x, int y, RendererDraw_t OnRender )
{
	cRenderer new_render;
	new_render.m_Parent		= this;
	new_render.m_bounds.x	= x;
	new_render.m_bounds.y	= y;
	new_render.m_OnRender	= OnRender;
	this->m_vRenderer.push_back( new_render );
	return &this->m_vRenderer.back();
}

cTimer* cWindow::AddTimer( TimerCall_t TickCallback, DWORD dwInterval )
{
	cTimer new_timer;
	new_timer.m_Callback	= TickCallback;
	new_timer.m_dwInterval	= dwInterval;
	new_timer.m_dwLastRun	= 0;
	new_timer.m_Parent		= this;
	this->m_vTimer.push_back( new_timer );
	return m_vTimer.end()._Myptr;
}

cStatusBar* cWindow::FindStatusBarByName( string name )
{
	list<cStatusBar>::iterator b = this->m_vStatus.begin();
	list<cStatusBar>::iterator e = this->m_vStatus.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_caption.c_str()))
			return &(*b);
	}

	return NULL;
}

cCheckbox* cWindow::FindCheckboxByName( string name )
{
	list<cCheckbox>::iterator b = this->m_vCheck.begin();
	list<cCheckbox>::iterator e = this->m_vCheck.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_caption.c_str()))
			return &(*b);
	}

	return NULL;
}

cButton* cWindow::FindButtonByName( string name )
{
	list<cButton>::iterator b = this->m_vButton.begin();
	list<cButton>::iterator e = this->m_vButton.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_caption.c_str()))
			return &(*b);
	}

	return NULL;
}

cCaption* cWindow::FindCaptionByName( string name )
{
	list<cCaption>::iterator b = this->m_vCaption.begin();
	list<cCaption>::iterator e = this->m_vCaption.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_caption.c_str()))
			return &(*b);
	}

	return NULL;
}

cListbox* cWindow::FindListboxByName( string name )
{
	list<cListbox>::iterator b = this->m_vListBox.begin();
	list<cListbox>::iterator e = this->m_vListBox.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_title.c_str()))
			return &(*b);
	}

	return NULL;
}

cColorSelector* cWindow::FindColorSelectorByName( string name )
{
	list<cColorSelector>::iterator b = this->m_vColorSelector.begin();
	list<cColorSelector>::iterator e = this->m_vColorSelector.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_title.c_str()))
			return &(*b);
	}

	return NULL;
}

cTextbox* cWindow::FindTextboxByName( string name )
{
	list<cTextbox>::iterator b = this->m_vTextBox.begin();
	list<cTextbox>::iterator e = this->m_vTextBox.end();

	for( ; b != e; ++b )
	{
		if(strstr(name.c_str(), (*b).m_name.c_str()))
			return &(*b);
	}

	return NULL;
}

//=======================================================

cStatusBar* cWindow::GetStatusBarByName( string name )
{
	list<cStatusBar>::iterator b = this->m_vStatus.begin();
	list<cStatusBar>::iterator e = this->m_vStatus.end();

	for( ; b != e; ++b )
	{
		if((*b).m_caption.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

cCheckbox* cWindow::GetCheckboxByName( string name )
{
	list<cCheckbox>::iterator b = this->m_vCheck.begin();
	list<cCheckbox>::iterator e = this->m_vCheck.end();

	for( ; b != e; ++b )
	{
		if((*b).m_caption.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

cButton* cWindow::GetButtonByName( string name )
{
	list<cButton>::iterator b = this->m_vButton.begin();
	list<cButton>::iterator e = this->m_vButton.end();

	for( ; b != e; ++b )
	{
		if((*b).m_caption.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

cCaption* cWindow::GetCaptionByName( string name )
{
	list<cCaption>::iterator b = this->m_vCaption.begin();
	list<cCaption>::iterator e = this->m_vCaption.end();

	for( ; b != e; ++b )
	{
		if((*b).m_caption.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

cListbox* cWindow::GetListboxByName( string name )
{
	list<cListbox>::iterator b = this->m_vListBox.begin();
	list<cListbox>::iterator e = this->m_vListBox.end();

	for( ; b != e; ++b )
	{
		if((*b).m_title.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

cColorSelector* cWindow::GetColorSelectorByName( string name )
{
	list<cColorSelector>::iterator b = this->m_vColorSelector.begin();
	list<cColorSelector>::iterator e = this->m_vColorSelector.end();

	for( ; b != e; ++b )
	{
		if((*b).m_title.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

cTextbox* cWindow::GetTextboxByName( string name )
{
	list<cTextbox>::iterator b = this->m_vTextBox.begin();
	list<cTextbox>::iterator e = this->m_vTextBox.end();

	for( ; b != e; ++b )
	{
		if((*b).m_name.compare( name ) == 0)
			return &(*b);
	}

	return NULL;
}

void cWindow::PrepareDelete()
{
	this->m_vButton.clear();
	this->m_vCaption.clear();
	this->m_vCheck.clear();
	this->m_vColorSelector.clear();
	this->m_vTextBox.clear();
	this->m_vRenderer.clear();
	this->m_vListBox.clear();
	this->m_vStatus.clear();
	this->m_vTimer.clear();
}

void cWindow::ClearEditingStates()
{
	for( list<cTextbox>::iterator tb = this->m_vTextBox.begin();
		tb != this->m_vTextBox.end(); ++tb )
	{
		(*tb).m_bIsEditing = false;
	}
}

void cGui::DoFrame( cWindow *pWindow )
{
	if( !pWindow )
		return;

	DWORD dwCurrentTick = GetTickCount();

	if( pWindow->m_vTimer.size() )
	{
		for( int i = 0; i < (int)pWindow->m_vTimer.size(); i++ )
		{
			cTimer pTimer = pWindow->m_vTimer[i];

			if((pTimer.m_dwLastRun + pTimer.m_dwInterval) < dwCurrentTick)
			{
				pTimer.m_dwLastRun = dwCurrentTick;

				if( pTimer.m_Callback )
					pTimer.m_Callback( pWindow, &pTimer );
			}
		}
	}
}

void cGui::HandleMovement()
{
	if( m_bActive == false )
		return;

	if( !m_vWindows.size() )
		return;

	DWORD dwCurrentTick = GetTickCount();

	int mouse_x = m_mouse.m_bounds.x;
	int mouse_y = m_mouse.m_bounds.y;

	for( list<cWindow>::iterator i = m_vWindows.begin(); i != m_vWindows.end(); ++i )
	{
		cWindow *pWindow = &(*i);

		if( !pWindow )
			continue;

		int m_x = pWindow->m_bounds.x, m_y = pWindow->m_bounds.y, 
			m_w = pWindow->m_bounds.w, m_h = pWindow->m_bounds.h;

		if( m_mouse.m_RightHeld )
		{
			if( m_mouse.m_Dragging.m_bDragging && m_mouse.m_Dragging.m_Window )
			{
				m_mouse.m_Dragging.m_Window->m_bounds.x = 
					mouse_x - m_mouse.m_Dragging.m_iWindowXOffset;

				m_mouse.m_Dragging.m_Window->m_bounds.y = 
					mouse_y - m_mouse.m_Dragging.m_iWindowYOffset;

				m_mouse.InvalidateClickStates();
				return;

			} else {
				if( pWindow->m_bounds.IsInVector( mouse_x, mouse_y ) )
				{
					pWindow = MoveToFront( i );
					m_mouse.m_Dragging.m_bDragging = true;
					m_mouse.m_Dragging.m_iWindowXOffset = (mouse_x - m_x);
					m_mouse.m_Dragging.m_iWindowYOffset = (mouse_y - m_y);
					m_mouse.m_Dragging.m_Window = pWindow;
				}
			}
		} else {
			m_mouse.m_Dragging.m_bDragging = false;
			m_mouse.m_Dragging.m_Window = NULL;
		}

		//we do buttons first because we need mouseover
		for( list<cButton>::iterator b = pWindow->m_vButton.begin();
			b != pWindow->m_vButton.end(); ++b )
		{
			cButton *pButton = &(*b);

			if( cSquareVector(
				m_x + pButton->m_bounds.x,
				m_y + pButton->m_bounds.y,
				pButton->m_bounds.w,
				pButton->m_bounds.h ).IsInVector( mouse_x, mouse_y ) )
			{
				if( !m_mouse.m_LeftClicked && !m_mouse.m_LeftHeld ){
					pButton->m_bHighlighted = true;
					m_mouse.InvalidateClickStates();
					return;
				}else{
					pButton->m_bHighlighted = false;

					//we should do a callback or something
					if((pButton->m_dwLastClickTick + 200) >= dwCurrentTick)
					{
						continue; //already in the clicking process...
					}

					pButton->m_dwLastClickTick = dwCurrentTick;
								
					if( pButton->m_Callback ){
						pButton->m_Callback( pButton );
					}
				}

				m_mouse.InvalidateClickStates();
				return;
			}else{
				pButton->m_bHighlighted = false;
			}
		}

		if( pWindow->m_bounds.IsInVector( mouse_x, mouse_y ) )
		{
			//its over the first window and we should only handle this one

			//handle text first

//			bool bTextClickedElse = false;

			for( list< cTextbox >::iterator tb = pWindow->m_vTextBox.begin();
				tb != pWindow->m_vTextBox.end(); ++tb )
			{
				cTextbox *pText = &(*tb);

				cSquareVector TextBounds( m_x + pText->m_bounds.x, m_y + pText->m_bounds.y, 
					pText->m_bounds.w, pText->m_bounds.h );

				if( TextBounds.IsInVector( mouse_x, mouse_y ) )
				{
					pText->m_bIsEditing = true;
					return;
				}
			}

			if( m_mouse.m_LeftHeld )
			{
				pWindow = MoveToFront( i );

				for( list<cStatusBar>::iterator sb = pWindow->m_vStatus.begin();
					sb != pWindow->m_vStatus.end(); ++sb )
				{
					cStatusBar *pStatus = &(*sb);

					cSquareVector StatusBounds( 
						m_x + pStatus->m_bounds.x, 
						m_y + pStatus->m_bounds.y, 
						pStatus->m_bounds.w, 
						pStatus->m_bounds.h );

					if( StatusBounds.IsInVector( mouse_x, mouse_y ) )
					{
						pStatus->m_LastClickSpace.x = mouse_x;
						pStatus->m_LastClickSpace.y = mouse_y;

						int iComplete = mouse_x - StatusBounds.x;

						pStatus->m_Percent.m_iPercent = 
							pStatus->m_Percent.GetPercent(
								iComplete, pStatus->m_bounds.w, static_cast< int >( pStatus->m_flMaxPercent ) );

						if( pStatus->m_Callback )
							pStatus->m_Callback( pStatus );

						m_mouse.InvalidateClickStates();
						return;
					}
				}

				for( list<cColorSelector>::iterator cs = pWindow->m_vColorSelector.begin();
					cs != pWindow->m_vColorSelector.end(); ++cs )
				{
					cColorSelector *pColor = &(*cs);

					cSquareVector ColorSelector(
						m_x + pColor->m_bounds.x,
						m_y + pColor->m_bounds.y+12, //leave room for text lawl
						pColor->m_bounds.w, 
						pColor->m_bounds.h - /*12*/ 24 );

					cSquareVector ColorSelectrorLum(
						m_x + pColor->m_bounds.x,
						m_y + pColor->m_bounds.y + pColor->m_bounds.h - 10, pColor->m_bounds.w, 9 );

					//	fill->FillOutline( draw_x, draw_y + m_bounds.GetHeight() + 3, m_bounds.GetWidth(), 10, &cGuiColor( 0, 0, 0, 255 ) );

					if( ColorSelector.IsInVector( mouse_x, mouse_y ) )
					{
						int col_x = ( mouse_x - ColorSelector.x );
						int col_y = ( mouse_y - ColorSelector.y );

						pColor->m_SelectedColor = pColor->m_SelectedColor.GetFromHLS( 
							(WORD)(239 * col_x / pColor->m_bounds.w - 1), 
							pColor->m_SelectedLum, 
							(WORD)(240 * col_y / pColor->m_bounds.h - 1), 255 
						);

						if( pColor->m_Callback )
							pColor->m_Callback( pColor );

						m_mouse.InvalidateClickStates();
						return;
					} else if( ColorSelectrorLum.IsInVector( mouse_x, mouse_y ) ) {
						WORD dlum = static_cast< WORD >(((mouse_x - ColorSelectrorLum.x) * HLSMAX / pColor->m_bounds.GetWidth()));
						dlum = HLSMAX - dlum;
						pColor->m_SelectedLum = dlum;

						WORD hue, lum, sat;
						int a;

						pColor->m_SelectedColor.GetHLS( hue, lum, sat, a );
						pColor->m_SelectedColor = pColor->m_SelectedColor.GetFromHLS( hue, pColor->m_SelectedLum, sat, a );

						if( pColor->m_Callback )
							pColor->m_Callback( pColor );

						m_mouse.InvalidateClickStates();
						return;
					}
				}

				/* here we can handle the click portion? */
				if( m_mouse.m_LeftClicked )
				{
					for( list<cCheckbox>::iterator cb = pWindow->m_vCheck.begin();
						cb != pWindow->m_vCheck.end(); ++cb )
					{
						cCheckbox *pCheck = &(*cb);

						if( cSquareVector(
							m_x + pCheck->m_bounds.x,
							m_y + pCheck->m_bounds.y,
							10, 10 ).IsInVector( mouse_x, mouse_y ) )
						{
							if( pCheck->m_pPointerVar )
							{
								if( pCheck->m_pPointerVar->Value == 1.f )
								{
									pCheck->m_pPointerVar->Value = 0.f;
								}
								else
								{
									pCheck->m_pPointerVar->Value = 1.f;
								}
							}

							if( pCheck->m_Callback )
								pCheck->m_Callback( pCheck );

							m_mouse.InvalidateClickStates();
							return;
						}
					}

					for( list<cCaption>::iterator cc = pWindow->m_vCaption.begin();
						cc != pWindow->m_vCaption.end(); ++cc )
					{
						cCaption *pCaption = &(*cc);

						//word height for now is not working. . .
						if( cSquareVector(
							m_x + pCaption->m_bounds.x,
							m_y + pCaption->m_bounds.y,
							( 10 * (int)strlen( pCaption->m_caption.c_str() ) ), 
							10 ).IsInVector( mouse_x, mouse_y ) )
						{
							if( pCaption->m_Callback )
								pCaption->m_Callback( pCaption );

							m_mouse.InvalidateClickStates();
							return;
						}
					}

					for( list<cListbox>::iterator lb = pWindow->m_vListBox.begin();
						lb != pWindow->m_vListBox.end(); ++lb )
					{
						cListbox *pList = &(*lb);

						cSquareVector listClicked(
							m_x + pList->m_bounds.x,
							m_y + pList->m_bounds.y,
							pList->m_bounds.w, 
							pList->m_bounds.h );

						cSquareVector upClicked(
							listClicked.x + (listClicked.w - 11),
							listClicked.y + 3, 8, 8 );

						cSquareVector downClicked(
							listClicked.x + (listClicked.w - 11),
							listClicked.y + (listClicked.h - 11),
							8, 8 );

						if( pList->m_bDisplayUD && upClicked.IsInVector( mouse_x, mouse_y ) )
						{
							int new_select = (pList->m_selected - 1);

							if( pList->m_selected == 0 || new_select < 0 )
								continue; //dont go up

							pList->m_selected = new_select;

							if( pList->IsInVisibleRange( new_select ) == false )
							{
								pList->m_beginidx--;
							}

							if( pList->m_Callback )
								pList->m_Callback( pList );

							m_mouse.InvalidateClickStates();

							return;
						}
						else if( pList->m_bDisplayUD && downClicked.IsInVector( mouse_x, mouse_y ) )
						{
							int new_select = ( pList->m_selected + 1 );

							if( new_select == ( int ) pList->m_options.size() )
								continue; //dont go down

							pList->m_selected = new_select;

							if( pList->IsInVisibleRange( new_select ) == false )
							{
								pList->m_beginidx++;
							}

							if( pList->m_Callback )
								pList->m_Callback( pList );

							m_mouse.InvalidateClickStates();
							return;
						}
						else if( listClicked.IsInVector( mouse_x, mouse_y ) )
						{
							int downy = mouse_y - listClicked.y;

							int max_fit = ( pList->m_bounds.h / 14 ) - 1;

							int sanity_check_idx = ( int )( pList->m_beginidx + ( downy / 14 ) );

							if( sanity_check_idx >= 0 && sanity_check_idx < static_cast< int >( pList->m_options.size() ) )
							{
								pList->m_selected = sanity_check_idx;

								if( pList->m_selected > pList->m_beginidx + max_fit )
								{
									if( pList->m_beginidx + 1 < static_cast< int >( pList->m_options.size() ) )
									{
										pList->m_beginidx++;
									}
								}
								else if( pList->m_selected < pList->m_beginidx )
								{
									if( pList->m_beginidx - 1 >= 0 )
									{
										pList->m_beginidx--;
									}
								}
							}

							if( pList->m_Callback )
								pList->m_Callback( pList );

							m_mouse.InvalidateClickStates();
							return;
						}
					}

					m_mouse.InvalidateClickStates();
					return;
				}
			}

			m_mouse.InvalidateClickStates();
			return;
		}
	}

	m_mouse.InvalidateClickStates();
}

bool cGui::UpdateKeyStates( WPARAM wParams, LPARAM lParams, UINT msg )
{
	if( msg == WM_KEYDOWN )
	{
		if( wParams == VK_INSERT )
		{
			m_bActive = !m_bActive;

			return true;
		}
		if( wParams == VK_DELETE )
		{
			m_mouse.m_bDrawMouseInfo = !m_mouse.m_bDrawMouseInfo;

			return true;
		}
		
		//source-specific

		if( wParams == VK_SNAPSHOT || wParams == VK_F5 )
		{
			return false;
		}
	}
	
	//if you want to lock this when the menu is off
	//or reset it to a fixed value, then edit here

	bool bResult = m_mouse.InputKeyData( wParams, lParams, msg );

	if( m_bActive )
	{
		HandleMovement();

		return bResult;
	}

	return false;
}

void cGui::RenderGui()
{
	if( !m_vWindows.size() )
		return;

	//render mouse last definately.....
	//render in reverse order to draw first on the array last
	list<cWindow>::iterator i = m_vWindows.end();

	while( i != m_vWindows.begin() )
	{
		i--;

		cWindow Window = *i;

		if( m_bActive || Window.m_bDrawWhenInactive )
		{
			DoFrame( &Window );

			Window.Draw( m_fill, m_text, (i == m_vWindows.begin()));

#ifndef __GUI_DRAW_OBJECT__
#define __GUI_DRAW_OBJECT__( t, l ) if( l.size() ){ for( list<t>::iterator i##t = l.begin(); i##t != l.end(); i##t++ ) \
			{ t ii##t = *i##t; ii##t.Draw( m_fill, m_text ); } }

			__GUI_DRAW_OBJECT__( cButton,			Window.m_vButton );
			__GUI_DRAW_OBJECT__( cCaption,			Window.m_vCaption );
			__GUI_DRAW_OBJECT__( cCheckbox,			Window.m_vCheck );
			__GUI_DRAW_OBJECT__( cStatusBar,		Window.m_vStatus );
			__GUI_DRAW_OBJECT__( cListbox,			Window.m_vListBox );
			__GUI_DRAW_OBJECT__( cColorSelector,	Window.m_vColorSelector );
			__GUI_DRAW_OBJECT__( cTextbox,			Window.m_vTextBox );
			__GUI_DRAW_OBJECT__( cRenderer,			Window.m_vRenderer );

#undef __GUI_DRAW_OBJECT__
#endif
		}
	}

	if( !m_bActive )
		return;

	//draw other shit

	m_mouse.Draw( m_fill );

	if( m_mouse.m_bDrawMouseInfo )
	{
		/*
		m_text->DrawText( 10, 10, cGuiColor( 255, 0, 0, 255 ),
			"Mouse ( x = %i || y = %i )", 
			m_mouse.m_bounds.x,
			m_mouse.m_bounds.y );

		m_text->DrawText( 10, 30, cGuiColor( 255, 0, 0, 255 ),
			"Click Info ( Left: %i ( Held: %i ) || Right: %i ( Held: %i ))",
			m_mouse.m_LeftClicked, m_mouse.m_LeftHeld,
			m_mouse.m_RightClicked, m_mouse.m_RightHeld );
			*/

		//disabled
	}
}

//=================================================================

void cWindow::Draw( cFillArea *fill, cDrawText* text, bool bIsSelectedWindow )
{
	//if there is a renderer object in there we don't want to fuck with alpha
	int iDrawAlpha = ( bIsSelectedWindow && GGui.IsActive()) ? 255 : 140;

	cColorScheme gradient(
		cGuiColor( 
			m_scheme.GetBackColor()->GetR(),
			m_scheme.GetBackColor()->GetG(),
			m_scheme.GetBackColor()->GetB(),
			iDrawAlpha ),
		cGuiColor( 
			m_scheme.GetForeColor()->GetR(),
			m_scheme.GetForeColor()->GetG(),
			m_scheme.GetForeColor()->GetB(),
			iDrawAlpha ) );

	//titlebar is drawn above the given X,Y
	
	// Here

	// Internal?...

	fill->FillGradient( m_bounds.x + 1, m_bounds.y + 1, m_bounds.w - 1, m_bounds.h - 1,
		gradient.GetForeColor(), gradient.GetBackColor() );

	fill->FillOutline( m_bounds.x, m_bounds.y, m_bounds.w, m_bounds.h, 
		&cGuiColor( 0, 0, 0, iDrawAlpha ) );

	fill->FillOutline( m_bounds.x - 1, m_bounds.y - 1, m_bounds.w + 2, m_bounds.h + 2,
		&cGuiColor( 255, 255, 255, iDrawAlpha ) );

	cGuiColor tc = *m_scheme.GetForeColor();

	tc.SetA( 255 );

	text->DrawText( m_bounds.x, m_bounds.y - 14, tc, (char *)m_title.c_str() );
}

void cStatusBar::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	cSquareVector sqvec( draw_x, draw_y, m_bounds.w, m_bounds.h );

	cGuiColor complete	= *m_scheme.GetForeColor();
	cGuiColor behindcol = *m_scheme.GetBackColor();

	char pszRenderText[ 256 ] = { 0 };

	sprintf( pszRenderText, "%s %i%%%%", m_caption.c_str(), m_Percent.m_iPercent );

	int charHeight	= 6;
	int charWidth	= 5;
	int iXAdd		= ( m_bounds.w / 2 ) - ( ( ( int ) strlen( pszRenderText ) * ( charWidth ) ) / 2 );
	int iYAdd		= ( m_bounds.h / 2 ) - charHeight;

	fill->Fill( draw_x, draw_y, m_bounds.w, m_bounds.h, &behindcol );
	
	int draw_width = m_Percent.GetValue( m_bounds.w, m_Percent.m_iPercent, static_cast< int >( m_flMaxPercent ) );

	if( draw_width > 1 )
	{
		fill->FillGradient(
			draw_x, draw_y, draw_width, m_bounds.h, 
			m_scheme.GetForeColor(), 
			&m_scheme.GetForeColor()->GetBrighter( 70 ), true );

		int white_height = m_bounds.h / 2;

		for( int i = 0; i < white_height; i++ )
		{
			fill->Fill( draw_x, draw_y + i, draw_width, 1, &cGuiColor( 255, 255, 255, ( 120 / white_height ) * ( white_height - i ) ) );
		}
	}
	
	fill->FillOutline( draw_x, draw_y, m_bounds.w, m_bounds.h, &cGuiColor( 0, 0, 0, 255 ) );

	text->DrawShadow( sqvec.x + iXAdd, sqvec.y + iYAdd, *m_scheme.GetBackColor(), 
		*m_scheme.GetForeColor(), pszRenderText );
}

void cCheckbox::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	cGuiColor outline = (m_scheme.GetOutline()->GetA() == 0) ? cGuiColor( 255, 255, 255, 255 )
		: *m_scheme.GetOutline();

	bool bChecked = m_pPointerVar->Boolean();

	//not checked

	fill->FillGradient(
		draw_x, draw_y, 10, 10, 
		(bChecked) ? m_scheme.GetForeColor() : m_scheme.GetBackColor(),
		(bChecked) ? &m_scheme.GetForeColor()->GetBrighter( 70 ) 
		: &m_scheme.GetBackColor()->GetBrighter( 70 ) );

	//glassy

	fill->Fill( draw_x, draw_y, 10, 4, &cGuiColor( 255, 255, 255, 160 ) );

	fill->FillOutline( draw_x, draw_y, 10, 10, &outline );
	
	text->DrawText( draw_x + 16, draw_y + 1, outline, 
		(char *) m_caption.c_str() );
}

void cButton::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	cSquareVector sqvec( draw_x, draw_y, m_bounds.w, m_bounds.h );

	cGuiColor outline = (m_scheme.GetOutline()->GetA() == 0) ? cGuiColor( 0, 0, 0, 255 )
		: *m_scheme.GetOutline();

	//center..
	int charHeight	= 6;
	int charWidth	= 5;
	int iXAdd		= (m_bounds.w/2) - (((int)m_caption.length()*(charWidth))/2);
	int iYAdd		= (m_bounds.h/2) - charHeight;

	if((m_dwLastClickTick + 200) >= GetTickCount())
	{
		//fill clicked
		fill->FillGradient(
			sqvec.x, sqvec.y, sqvec.w, sqvec.h,
			&m_scheme.GetForeColor()->GetDarker( 10 ),
			&m_scheme.GetBackColor()->GetDarker( 10 ) );

		fill->FillOutline( sqvec.x, sqvec.y, sqvec.w, sqvec.h,
			&outline );

		text->DrawText( sqvec.x + iXAdd, sqvec.y + iYAdd,
			outline,
			(char *)m_caption.c_str() );
	}
	else if( m_bHighlighted == true )
	{
		fill->FillGradient(
			sqvec.x, sqvec.y, sqvec.w, sqvec.h,
			&m_scheme.GetForeColor()->GetBrighter( 10 ),
			&m_scheme.GetBackColor()->GetBrighter( 10 ) );
		
		fill->FillOutline( sqvec.x, sqvec.y, sqvec.w, sqvec.h,
			&outline );

		text->DrawText( sqvec.x + iXAdd, sqvec.y + iYAdd,
			outline,
			(char *)m_caption.c_str() );

		return;
	}
	else
	{
		//fill not clicked
		fill->FillGradient(
			sqvec.x, sqvec.y, sqvec.w, sqvec.h,
			m_scheme.GetForeColor(),
			m_scheme.GetBackColor() );
		
		fill->FillOutline( sqvec.x, sqvec.y, sqvec.w, sqvec.h,
			&outline );

		text->DrawText( sqvec.x + iXAdd, sqvec.y + iYAdd,
			cGuiColor( 0, 0, 0, 220 ),
			(char *)m_caption.c_str() );
	}
}

void cCaption::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	text->DrawText( draw_x, draw_y, *m_scheme.GetForeColor(), 
		(char *) m_caption.c_str() );
}

void cListbox::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	//draw bg
	fill->Fill( draw_x, draw_y, m_bounds.w, m_bounds.h, m_scheme.GetBackColor() );

	//max we can fit in here

	int last_index = 0;

	for( int i = m_beginidx; i < (int)m_options.size(); i++ )
	{
		if( !IsInVisibleRange( i ) )
		{
			last_index = i;

			continue;
		}

		int current_item_y = ( ( i - m_beginidx ) * 14 );

		string os = m_options[i];

		int strw = ( ( int ) os.length() * 7 );
		
		if( strw > m_bounds.w )
		{
			int diff = strw - m_bounds.w;
			//how many characters fit into that space and how many should we remove....
			int remove_chars = (diff / 8);
			os = os.substr( 0, os.length() - remove_chars );
			os += "...";
		}

		if( i == m_selected )
		{
			fill->Fill( draw_x + 1, draw_y + current_item_y, m_bounds.w - 2, 14,
				m_scheme.GetForeColor() );

			text->DrawText( draw_x + 4, draw_y + current_item_y, 
				*m_scheme.GetBackColor(), (char *)os.c_str() );
		}
		else
		{
			text->DrawText( draw_x + 4, draw_y + current_item_y, 
				*m_scheme.GetForeColor(), (char *) os.c_str() );
		}
	}

	//we should draw up/down buttons here

	if( m_bDisplayUD )
	{
		cGuiColor UpFillColor = *m_scheme.GetOutline(), DownFillColor = *m_scheme.GetOutline();

		if( m_selected == 0 )
		{
			UpFillColor = *m_scheme.GetForeColor();
		}
		
		if( m_selected == static_cast< int >( m_options.size() ) - 1 ) 
		{
			DownFillColor = *m_scheme.GetForeColor();
		}

		int draw_x_buttons = draw_x + ( m_bounds.w - 11 );
		int draw_y_buttonu = draw_y + 3;
		int draw_y_buttond = draw_y + ( m_bounds.h - 11 );

		fill->Fill( draw_x_buttons, draw_y_buttonu, 8, 8, &UpFillColor );
		fill->Fill( draw_x_buttons, draw_y_buttond, 8, 8, &DownFillColor );

		fill->FillOutline( draw_x_buttons, draw_y_buttonu, 8, 8, &cGuiColor( 0, 0, 0, 255 ) );
		fill->FillOutline( draw_x_buttons, draw_y_buttond, 8, 8, &cGuiColor( 0, 0, 0, 255 ) );

		// Up Arrow
		fill->Fill( draw_x_buttons + 4, draw_y_buttonu + 3, 1, 1, m_scheme.GetForeColor() );
		fill->Fill( draw_x_buttons + 3, draw_y_buttonu + 4, 3, 1, m_scheme.GetForeColor() );
		fill->Fill( draw_x_buttons + 2, draw_y_buttonu + 5, 5, 1, m_scheme.GetForeColor() );

		// Down Arrow
		fill->Fill( draw_x_buttons + 4, draw_y_buttond + 5, 1, 1, m_scheme.GetForeColor() );
		fill->Fill( draw_x_buttons + 3, draw_y_buttond + 4, 3, 1, m_scheme.GetForeColor() );
		fill->Fill( draw_x_buttons + 2, draw_y_buttond + 3, 5, 1, m_scheme.GetForeColor() );

		//draw outline...
		fill->FillOutline( draw_x, draw_y, m_bounds.w, m_bounds.h, m_scheme.GetOutline() );
	}

	cSquareVector IsOver( draw_x, draw_y, m_bounds.w, m_bounds.h );

	int mouse_x = GGui.GetMouse()->m_bounds.x;
	int mouse_y = GGui.GetMouse()->m_bounds.y;

	if( IsOver.IsInVector( mouse_x, mouse_y ) )
	{
//		fill->FillOutline( mouse_x + 5, mouse_y, (m_title.length() * 6), 10 );
//		fill->Fill( mouse_x + 6, mouse_y + 1, (m_title.length() * 6) - 1, 9 );

		text->DrawText( mouse_x + 10, mouse_y, *m_scheme.GetForeColor(), (char *) m_title.c_str() );
	}
}

void cColorSelector::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	text->DrawText( draw_x, draw_y, cGuiColor( 0, 0, 0, 255 ), (char *)m_title.c_str() );

	//we need to fill the entire square with it, its gonna suck but here we go

	int new_h = m_bounds.h - 12;

	fill->FillOutline( draw_x - 1, ( draw_y + 12 ) - 1, 
		m_bounds.w + 1, ( new_h - 12 ) + 1, &cGuiColor( 0, 0, 0, 255 ) );

	for( int y = 12; y < new_h; y++ )
	{
		for( int x = 0; x < m_bounds.w; x++ )
		{
			cGuiColor col;
			
			col = col.GetFromHLS( 
				239 * static_cast< WORD >( x ) / static_cast< WORD >( m_bounds.w ) - 1, 
				HLSMAX / 2, 
				240 * static_cast< WORD >( y ) / static_cast< WORD >( new_h ) - 1, 
				255 );

			fill->Fill_Fast( draw_x + x, draw_y + y, 1, 1, &col );
		}
	}

	//draw lum under it

	int draw_lum_x = draw_x;
	int draw_lum_y = draw_y + m_bounds.GetHeight() - 8;
	int draw_lum_w = m_bounds.GetWidth();
	int draw_lum_h = 8;

	fill->FillOutline( draw_lum_x - 1, draw_lum_y - 1, draw_lum_w + 1, draw_lum_h + 1, &cGuiColor( 0, 0, 0, 255 ) );

	cGuiColor col = m_SelectedColor;

	WORD hue = 0, lum = 0, sat = 0;
	
	int a = 0;
	
	col.GetHLS( hue, lum, sat, a );

	for( int l = 1; l < m_bounds.GetWidth(); l++ )
	{
		WORD dlum = ( static_cast< WORD >( l ) * HLSMAX / static_cast< WORD >( m_bounds.GetWidth() ) );

		dlum = HLSMAX - dlum;

		fill->Fill_Fast( draw_lum_x + l, draw_lum_y, 1, draw_lum_h, &col.GetFromHLS( hue, dlum, sat, 255 ) );
	}
}

void cTextbox::Draw( cFillArea *fill, cDrawText* text )
{
	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	fill->Fill( draw_x + 1, draw_y + 1, m_bounds.w - 1, m_bounds.h - 1, m_scheme.GetBackColor() );
	fill->FillOutline( draw_x, draw_y, m_bounds.w, m_bounds.h, m_scheme.GetOutline() );

	if( m_bounds.w < 6 )
		return;

	int char_width		= 6;
	int max_chars		= (m_bounds.w / char_width);

	int end = (m_iRenderIndex + max_chars);

	if( end > static_cast< int >( m_string.size() ) )
	{
		end = static_cast< int >( m_string.size() );
	}

	char *szRender = new char[ m_string.size() + 1 ];

	for( int i = m_iRenderIndex; i < end; i++ )
	{
		if( i > static_cast< int >( m_string.size() ) )
			continue;

		szRender[i] = m_string[i];

		if( i == static_cast< int >( m_string.size() ) )
		{
			szRender[i+1] = '\0';
			break;
		}
	}

	cGuiColor textcol = *m_scheme.GetForeColor();

	text->DrawText( draw_x + 2, draw_y + 2, textcol, szRender );
}

void cRenderer::Draw( cFillArea *fill, cDrawText* text )
{
	if( !m_OnRender )
		return;

	int draw_x = (m_Parent) ? m_Parent->m_bounds.x + m_bounds.x : m_bounds.x;
	int draw_y = (m_Parent) ? m_Parent->m_bounds.y + m_bounds.y : m_bounds.y;

	m_OnRender( this, draw_x, draw_y, fill, text );
}

bool cMouse::InputKeyData( WPARAM wParams, LPARAM lParams, UINT msg )
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			m_LeftHeld = true;
			m_LeftClicked = true;
			m_dwLastLeftClickTick = GetTickCount();
			break;
		}
	case WM_LBUTTONUP:
		{
			m_LeftHeld = false;
			break;
		}
	case WM_RBUTTONDOWN:
		{
			m_RightHeld = true;
			m_RightClicked = true;
			m_dwLastRightClickTick = GetTickCount();
			break;
		}
	case WM_RBUTTONUP:
		{
			m_RightHeld = false;
			break;
		}
	default:
		{
			return false;
		}
	}

	return true;
}

void CSchemeLoader::LoadSchemeInformation()
{
#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "CSchemeLoader.log", "LoadSchemeInformation" );
#endif

	m_pWindow	= GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\window.scheme" ) );
	m_pButton	= GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\button.scheme" ) );
	m_pCheck	= GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\check.scheme" ) );
	m_pListbox	= GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\list.scheme" ) );
	m_pStatus	= GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\status.scheme" ) );
	m_pText		= GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\text.scheme" ) );
}

void CSchemeLoader::SaveSchemeInformation( 
	cColorScheme *window, 
	cColorScheme *button, 
	cColorScheme *check, 
	cColorScheme *list, 
	cColorScheme *status, 
	cColorScheme *text )
{
#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "CSchemeLoader.log", "SaveSchemeInformation" );
#endif

	SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\window.scheme" ), window );
	SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\button.scheme" ), button );
	SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\check.scheme" ), check );
	SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\list.scheme" ), list );
	SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\status.scheme" ), status );
	SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\text.scheme" ), text );
}

cColorScheme* CSchemeLoader::GetWindowScheme(){ return m_pWindow; }
cColorScheme* CSchemeLoader::GetButtonScheme(){ return m_pButton; }
cColorScheme* CSchemeLoader::GetCheckScheme(){ return m_pCheck; }
cColorScheme* CSchemeLoader::GetListboxScheme(){ return m_pListbox; }
cColorScheme* CSchemeLoader::GetStatusScheme(){ return m_pStatus; }
cColorScheme* CSchemeLoader::GetTextScheme(){ return m_pText; }

cColorScheme *CSchemeLoader::GetSchemeFromFile( string File )
{
#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "CSchemeLoader.log", "Loading Scheme file[%s]", File.c_str() );
#endif

	CFile *pFile = new CFile( File, FILE_READ );

	pFile->Open();

	cColorScheme *defaultScheme = new cColorScheme(
		cGuiColor( 0, 0, 0, 255 ),
		cGuiColor( 255, 255, 255, 255 ),
		cGuiColor( 0, 0, 0, 255 ) );

	long size = pFile->GetSize();

	if( !pFile->Valid() || size == 0 )
	{
#ifdef _CSS_LOGGING_ENABLED_
		GApp.AddToLogFileA( "CSchemeLoader.log", "Invalid Scheme file.." );
#endif

		return defaultScheme;
	}

	unsigned char *ucBuffer = new unsigned char[ 12 ];

	if( pFile->ReadContents( ( unsigned char* ) ucBuffer, NULL ) == false )
	{
#ifdef _CSS_LOGGING_ENABLED_
		GApp.AddToLogFileA( "CSchemeLoader.log", "Unable to read scheme information...[0x%X]", GetLastError() );
#endif

		return defaultScheme;
	}

	int b_r = static_cast<int>( ucBuffer[0] );
	int b_g = static_cast<int>( ucBuffer[1] );
	int b_b = static_cast<int>( ucBuffer[2] );
	int b_a = static_cast<int>( ucBuffer[3] );

	int f_r = static_cast<int>( ucBuffer[4] );
	int f_g = static_cast<int>( ucBuffer[5] );
	int f_b = static_cast<int>( ucBuffer[6] );
	int f_a = static_cast<int>( ucBuffer[7] );

	int o_r = static_cast<int>( ucBuffer[8] );
	int o_g = static_cast<int>( ucBuffer[9] );
	int o_b = static_cast<int>( ucBuffer[10] );
	int o_a = static_cast<int>( ucBuffer[11] );

	pFile->Close();

	cColorScheme *loadedScheme = new cColorScheme(
		cGuiColor( b_r, b_g, b_b, b_a ),
		cGuiColor( f_r, f_g, f_b, f_a ),
		cGuiColor( o_r, o_g, o_b, o_a ) );

	if( loadedScheme )
	{
		delete[] defaultScheme;

		return loadedScheme;
	}

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "CSchemeLoader.log", "Invalid Scheme file.." );
#endif

	return defaultScheme;
}

void CSchemeLoader::SaveSchemeToFile( string File, cColorScheme *scheme )
{
	if( !scheme )
		return;

	FILE *fp = fopen( File.c_str(), "w" );

	if( !fp )
		return;
	
	unsigned char b_r = static_cast<unsigned char>( scheme->GetBackColor()->GetR() );
	unsigned char b_g = static_cast<unsigned char>( scheme->GetBackColor()->GetG() );
	unsigned char b_b = static_cast<unsigned char>( scheme->GetBackColor()->GetB() );
	unsigned char b_a = static_cast<unsigned char>( scheme->GetBackColor()->GetA() );

	unsigned char f_r = static_cast<unsigned char>( scheme->GetForeColor()->GetR() );
	unsigned char f_g = static_cast<unsigned char>( scheme->GetForeColor()->GetG() );
	unsigned char f_b = static_cast<unsigned char>( scheme->GetForeColor()->GetB() );
	unsigned char f_a = static_cast<unsigned char>( scheme->GetForeColor()->GetA() );

	unsigned char o_r = static_cast<unsigned char>( scheme->GetOutline()->GetR() );
	unsigned char o_g = static_cast<unsigned char>( scheme->GetOutline()->GetG() );
	unsigned char o_b = static_cast<unsigned char>( scheme->GetOutline()->GetB() );
	unsigned char o_a = static_cast<unsigned char>( scheme->GetOutline()->GetA() );

	fwrite( &b_r, 1, 1, fp );
	fwrite( &b_g, 1, 1, fp );
	fwrite( &b_b, 1, 1, fp );
	fwrite( &b_a, 1, 1, fp );

	fwrite( &f_r, 1, 1, fp );
	fwrite( &f_g, 1, 1, fp );
	fwrite( &f_b, 1, 1, fp );
	fwrite( &f_a, 1, 1, fp );

	fwrite( &o_r, 1, 1, fp );
	fwrite( &o_g, 1, 1, fp );
	fwrite( &o_b, 1, 1, fp );
	fwrite( &o_a, 1, 1, fp );

	fclose( fp );
}