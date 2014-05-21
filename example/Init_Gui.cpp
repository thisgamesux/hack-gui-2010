#include "stdafx.h"
#include "Init_Gui.h"
#include "CGui.h"

namespace NGui
{
	namespace Schemes
	{
		cColorScheme		Window;
		cColorScheme		Button;
		cColorScheme		Check;
		cColorScheme		Listbox;
		cColorScheme		Status;
		cColorScheme		Text;

		namespace Team
		{
			cColorScheme	T;
			cColorScheme	CT;
			cColorScheme	Friend;
			cColorScheme	Target;
		};
	};

	namespace Core
	{
		cFillArea			GFill;
		cDrawText			GText;
	};

	namespace Windows
	{
		cWindow*			GAimbot			= NULL;
		cWindow*			GVisual			= NULL;
		cWindow*			GRemoval		= NULL;
		cWindow*			GMisc			= NULL;
	};

	namespace ObjectGlobals
	{
		cListbox*			GSelectedScheme	= NULL;
	};
};

void GUI_KeyHook();

void AimMode_Click( cListbox* pList )
{
	GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Mode )->Value =
		static_cast< float >( pList->m_selected );
}

void AimSmooth_Click( cStatusBar *pStatus )
{
	GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Smooth )->Value =
		static_cast< float >( pStatus->m_Percent.m_iPercent );
}

void AimFOV_Click( cStatusBar *pStatus )
{
	GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Fov )->Value =
		static_cast< float >( pStatus->m_Percent.m_iPercent );
}

void AimbotKey_Click( cListbox* pList )
{
	GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Key )->Value =
		static_cast< float >( pList->m_selected );
}

void AimbotPrediction_Click( cListbox* pList )
{
	GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Prediction )->Value =
		static_cast< float >( pList->m_selected );
}

void BoneListSelection_Click( cListbox* pList )
{
	GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Bone )->Value =
		static_cast< float >( pList->m_selected );
}

void SpeedSelect_Click( cStatusBar* pStatus )
{
	GCvar.GetSectionByIndex( NCvar::Section::Misc::Singleton )->GetCvarByIndex( NCvar::Section::Misc::Speedhack )->Value =
		static_cast< float >( pStatus->m_Percent.m_iPercent );
}

void SpinbotSelect_Click( cListbox* pList )
{
	GCvar.GetSectionByIndex( NCvar::Section::Misc::Singleton )->GetCvarByIndex( NCvar::Section::Misc::Spinbot )->Value =
		static_cast< float >( pList->m_selected );
}

int GCurrentSelectedEditingIndex = 0;

void Color_Window( cColorSelector *pColor )
{
	switch( GCurrentSelectedEditingIndex )
	{
	case 0:
		{
			//window color
			NGui::Schemes::Window.SetBackColor( pColor->m_SelectedColor );
			NGui::Schemes::Window.SetForeColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				(*i).m_scheme = NGui::Schemes::Window;
			}
			break;
		}
	case 1:
		{
			//button top
			NGui::Schemes::Button.SetForeColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cButton>::iterator c = (*i).m_vButton.begin(); c != (*i).m_vButton.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Button;
				}
			}
			break;
		}
	case 2:
		{
			//button bottom
			NGui::Schemes::Button.SetBackColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cButton>::iterator c = (*i).m_vButton.begin(); c != (*i).m_vButton.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Button;
				}
			}
			break;
		}
	case 3:
		{
			//button outline
			NGui::Schemes::Button.SetOutline( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cButton>::iterator c = (*i).m_vButton.begin(); c != (*i).m_vButton.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Button;
				}
			}
			break;
		}
	case 4:
		{
			//check on
			NGui::Schemes::Check.SetForeColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cCheckbox>::iterator c = (*i).m_vCheck.begin(); c != (*i).m_vCheck.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Check;
				}
			}
			break;
		}
	case 5:
		{
			//check off
			NGui::Schemes::Check.SetBackColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cCheckbox>::iterator c = (*i).m_vCheck.begin(); c != (*i).m_vCheck.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Check;
				}
			}
			break;
		}
	case 6:
		{
			//list front
			NGui::Schemes::Listbox.SetForeColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cListbox>::iterator c = (*i).m_vListBox.begin(); c != (*i).m_vListBox.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Listbox;
				}
			}
			break;
		}
	case 7:
		{
			//list back
			NGui::Schemes::Listbox.SetBackColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cListbox>::iterator c = (*i).m_vListBox.begin(); c != (*i).m_vListBox.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Listbox;
				}
			}
			break;
		}
	case 8:
		{
			NGui::Schemes::Status.SetForeColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cStatusBar>::iterator c = (*i).m_vStatus.begin(); c != (*i).m_vStatus.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Status;
				}
			}
			break;
		}
	case 9:
		{
			NGui::Schemes::Status.SetBackColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cStatusBar>::iterator c = (*i).m_vStatus.begin(); c != (*i).m_vStatus.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Status;
				}
			}
			break;
		}
	case 10:
		{
			NGui::Schemes::Text.SetBackColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cTextbox>::iterator c = (*i).m_vTextBox.begin(); c != (*i).m_vTextBox.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Text;
				}
			}
			break;
		}
	case 11:
		{
			NGui::Schemes::Text.SetForeColor( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cTextbox>::iterator c = (*i).m_vTextBox.begin(); c != (*i).m_vTextBox.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Text;
				}
			}
			break;
		}
	case 12:
		{
			NGui::Schemes::Text.SetOutline( pColor->m_SelectedColor );

			for( list<cWindow>::iterator i = GGui.m_vWindows.begin(); i != GGui.m_vWindows.end(); ++i )
			{
				for( list<cTextbox>::iterator c = (*i).m_vTextBox.begin(); c != (*i).m_vTextBox.end(); ++c )
				{
					(*c).m_scheme = NGui::Schemes::Text;
				}
			}
			break;
		}
	case 13:
		{
			//T F

			NGui::Schemes::Team::T.SetForeColor( pColor->m_SelectedColor );

			GBase.SetTerroristFront( 
				Color( 
				NGui::Schemes::Team::T.GetForeColor()->GetR(),
				NGui::Schemes::Team::T.GetForeColor()->GetG(),
				NGui::Schemes::Team::T.GetForeColor()->GetB(),
				NGui::Schemes::Team::T.GetForeColor()->GetA() ) );

			break;
		}
	case 14:
		{
			//T B

			NGui::Schemes::Team::T.SetBackColor( pColor->m_SelectedColor );

			GBase.SetTerroristBack( 
				Color( 
				NGui::Schemes::Team::T.GetBackColor()->GetR(),
				NGui::Schemes::Team::T.GetBackColor()->GetG(),
				NGui::Schemes::Team::T.GetBackColor()->GetB(),
				NGui::Schemes::Team::T.GetBackColor()->GetA() ) );

			break;
		}
	case 15:
		{
			//C F

			NGui::Schemes::Team::CT.SetForeColor( pColor->m_SelectedColor );

			GBase.SetCounterTerroristFront(  
				Color( 
				NGui::Schemes::Team::CT.GetForeColor()->GetR(),
				NGui::Schemes::Team::CT.GetForeColor()->GetG(),
				NGui::Schemes::Team::CT.GetForeColor()->GetB(),
				NGui::Schemes::Team::CT.GetForeColor()->GetA() ) );

			break;
		}
	case 16:
		{
			//C B

			NGui::Schemes::Team::CT.SetBackColor( pColor->m_SelectedColor );

			GBase.SetCounterTerroristBack(  
				Color( 
				NGui::Schemes::Team::CT.GetBackColor()->GetR(),
				NGui::Schemes::Team::CT.GetBackColor()->GetG(),
				NGui::Schemes::Team::CT.GetBackColor()->GetB(),
				NGui::Schemes::Team::CT.GetBackColor()->GetA() ) );

			break;
		}
	case 17:
		{
			//F F

			NGui::Schemes::Team::Friend.SetForeColor( pColor->m_SelectedColor );

			GBase.SetFriendFront(  
				Color( 
				NGui::Schemes::Team::Friend.GetForeColor()->GetR(),
				NGui::Schemes::Team::Friend.GetForeColor()->GetG(),
				NGui::Schemes::Team::Friend.GetForeColor()->GetB(),
				NGui::Schemes::Team::Friend.GetForeColor()->GetA() ) );

			break;
		}
	case 18:
		{
			//F B

			NGui::Schemes::Team::Friend.SetBackColor( pColor->m_SelectedColor );

			GBase.SetFriendFront(  
				Color( 
				NGui::Schemes::Team::Friend.GetBackColor()->GetR(),
				NGui::Schemes::Team::Friend.GetBackColor()->GetG(),
				NGui::Schemes::Team::Friend.GetBackColor()->GetB(),
				NGui::Schemes::Team::Friend.GetBackColor()->GetA() ) );

			break;
		}
	case 19:
		{
			// Target

			NGui::Schemes::Team::Target.SetForeColor( pColor->m_SelectedColor );
		}
	}
}

void ColorSchemeEditorSelect_Click( cListbox* pList )
{
	GCurrentSelectedEditingIndex = pList->m_selected;
}

void ColorPicked_OnRender( cRenderer* pRender, int x, int y, cFillArea *fill, cDrawText* text )
{
	cGuiColor DrawColor( 255, 255, 255, 255 );

	if( pRender->m_Parent )
	{
		cColorSelector* pSelection = pRender->m_Parent->GetColorSelectorByName( "Selector" );

		if( pSelection )
		{
			DrawColor = pSelection->m_SelectedColor;
		}
	}

	fill->FillOutline( x, y, 8, 70, &cGuiColor( 0, 0, 0, 255 ) );

	fill->Fill( x + 1, y + 1, 7, 69, &DrawColor );
}

void RenderColors_Click( cButton *pButton )
{
	GGui.RemoveWindow( GGui.GetWindow( "Color Editor" ) );

	cWindow *pGuiMainColor = GGui.AddWindow( "Color Editor", NGui::Schemes::Window, 10, 380, 115, 110 );

	cColorSelector* c = pGuiMainColor->AddColorSelector( "Selector", 5, 5, 90, 100, Color_Window );

	pGuiMainColor->AddRenderer( 100, 16, ColorPicked_OnRender );

	c->m_SelectedColor	= cGuiColor( 255, 255, 255, 255 );
	c->m_SelectedLum	= 150;
}

void SaveSchemes_Click( cButton *pButton )
{
	GSchemeLoader.SaveSchemeInformation( 
		&NGui::Schemes::Window, 
		&NGui::Schemes::Button, 
		&NGui::Schemes::Check, 
		&NGui::Schemes::Listbox, 
		&NGui::Schemes::Status, 
		&NGui::Schemes::Text );

	GSchemeLoader.SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\team\\t.scheme" ), &NGui::Schemes::Team::T );
	GSchemeLoader.SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\team\\ct.scheme" ), &NGui::Schemes::Team::CT );
	GSchemeLoader.SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\team\\f.scheme" ), &NGui::Schemes::Team::Friend );
	GSchemeLoader.SaveSchemeToFile( GApp.GetDirectoryFileA( "scheme\\team\\target.scheme" ), &NGui::Schemes::Team::Target );

	Msg( "Saved scheme information to files\n" );
}

void ResetSchemes_Click( cButton *pButton )
{
	GSchemeLoader.LoadSchemeInformation();

	NGui::Schemes::Window		= *GSchemeLoader.GetWindowScheme();
	NGui::Schemes::Button		= *GSchemeLoader.GetButtonScheme();
	NGui::Schemes::Check		= *GSchemeLoader.GetCheckScheme();
	NGui::Schemes::Listbox		= *GSchemeLoader.GetListboxScheme();
	NGui::Schemes::Status		= *GSchemeLoader.GetStatusScheme();
	NGui::Schemes::Text			= *GSchemeLoader.GetTextScheme();

	NGui::Schemes::Team::T		= *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\t.scheme" ) );
	NGui::Schemes::Team::CT		= *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\ct.scheme" ) );
	NGui::Schemes::Team::Friend	= *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\f.scheme" ) );
	NGui::Schemes::Team::Target = *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\target.scheme" ) );

	Msg( "Reloaded scheme information from files\n" );
}

void SaveAllSettings_Click( cButton* pButton )
{
	GCvar.ToFile( GApp.GetDirectoryFileA( "settings" ) );

	SaveSchemes_Click( pButton );
}

void Radar_Render( cRenderer* pRender, int x, int y, cFillArea *fill, cDrawText* text )
{
	Core::Radar::SetB( x, y,
		pRender->m_Parent->m_bounds.w - 20,
		pRender->m_Parent->m_bounds.h - 20 );

	Core::Radar::Render( fill, text, 10.f );

	Core::Radar::ClearEntities();
}

void RadarCheckEnable_Click( cCheckbox* pCheckbox )
{
	if( pCheckbox->m_pPointerVar->Boolean() )
	{
		cWindow* pRadarWindow = GGui.AddWindow( "Radar", NGui::Schemes::Window, 10, 400, 200, 200, true );

		pRadarWindow->AddRenderer( 10, 10, Radar_Render );
	}
	else
	{
		GGui.RemoveWindow( GGui.FindWindow( "Radar" ) );
	}
}

void Init_Gui()
{
	GSchemeLoader.LoadSchemeInformation();

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "gui.log", "Loaded Scheme Information!" );
#endif

	NGui::Schemes::Window		= *GSchemeLoader.GetWindowScheme();
	NGui::Schemes::Button		= *GSchemeLoader.GetButtonScheme();
	NGui::Schemes::Check		= *GSchemeLoader.GetCheckScheme();
	NGui::Schemes::Listbox		= *GSchemeLoader.GetListboxScheme();
	NGui::Schemes::Status		= *GSchemeLoader.GetStatusScheme();
	NGui::Schemes::Text			= *GSchemeLoader.GetTextScheme();

	NGui::Schemes::Team::T		= *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\t.scheme" ) );
	NGui::Schemes::Team::CT		= *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\ct.scheme" ) );
	NGui::Schemes::Team::Friend	= *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\f.scheme" ) );
	NGui::Schemes::Team::Target = *GSchemeLoader.GetSchemeFromFile( GApp.GetDirectoryFileA( "scheme\\team\\target.scheme" ) );

	GBase.SetTerroristFront( 
		Color( 
		NGui::Schemes::Team::T.GetForeColor()->GetR(),
		NGui::Schemes::Team::T.GetForeColor()->GetG(),
		NGui::Schemes::Team::T.GetForeColor()->GetB(),
		NGui::Schemes::Team::T.GetForeColor()->GetA() ) );

	GBase.SetTerroristBack( 
		Color( 
		NGui::Schemes::Team::T.GetBackColor()->GetR(),
		NGui::Schemes::Team::T.GetBackColor()->GetG(),
		NGui::Schemes::Team::T.GetBackColor()->GetB(),
		NGui::Schemes::Team::T.GetBackColor()->GetA() ) );

	GBase.SetCounterTerroristFront( 
		Color( 
		NGui::Schemes::Team::CT.GetForeColor()->GetR(),
		NGui::Schemes::Team::CT.GetForeColor()->GetG(),
		NGui::Schemes::Team::CT.GetForeColor()->GetB(),
		NGui::Schemes::Team::CT.GetForeColor()->GetA() ) );

	GBase.SetCounterTerroristBack( 
		Color( 
		NGui::Schemes::Team::CT.GetBackColor()->GetR(),
		NGui::Schemes::Team::CT.GetBackColor()->GetG(),
		NGui::Schemes::Team::CT.GetBackColor()->GetB(),
		NGui::Schemes::Team::CT.GetBackColor()->GetA() ) );

	GBase.SetFriendFront( 
		Color( 
		NGui::Schemes::Team::Friend.GetForeColor()->GetR(),
		NGui::Schemes::Team::Friend.GetForeColor()->GetG(),
		NGui::Schemes::Team::Friend.GetForeColor()->GetB(),
		NGui::Schemes::Team::Friend.GetForeColor()->GetA() ) );

	GBase.SetFriendBack( 
		Color( 
		NGui::Schemes::Team::Friend.GetBackColor()->GetR(),
		NGui::Schemes::Team::Friend.GetBackColor()->GetG(),
		NGui::Schemes::Team::Friend.GetBackColor()->GetB(),
		NGui::Schemes::Team::Friend.GetBackColor()->GetA() ) );

	GSchemeLoader.SaveSchemeInformation( 
		&NGui::Schemes::Window, 
		&NGui::Schemes::Button, 
		&NGui::Schemes::Check, 
		&NGui::Schemes::Listbox, 
		&NGui::Schemes::Status, 
		&NGui::Schemes::Text );

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "gui.log", "Saved Scheme Information" );
#endif

	GGui.SetFill( &NGui::Core::GFill );
	GGui.SetText( &NGui::Core::GText );

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "gui.log", "Setup Classes" );
#endif

	GGui.GetMouse()->m_color	= cGuiColor( 255, 255, 255, 255 );
	GGui.GetMouse()->m_outline	= cGuiColor( 0, 0, 0, 255 );

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "gui.log", "Mouse configured" );
#endif

	NGui::Windows::GAimbot		= GGui.AddWindow( "Aimbot", NGui::Schemes::Window, 10, 20, 160, 385 );
	NGui::Windows::GVisual		= GGui.AddWindow( "Visual", NGui::Schemes::Window, 180, 20, 160, 330 );
	NGui::Windows::GRemoval		= GGui.AddWindow( "Removal", NGui::Schemes::Window, 350, 20, 160, 150 );
	NGui::Windows::GMisc		= GGui.AddWindow( "Misc", NGui::Schemes::Window, 520, 20, 160, 160 );
	
	// 

	NGui::Windows::GAimbot->AddCheckbox( "Enabled", NGui::Schemes::Check, 10, 10,
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Enabled ) );

	NGui::Windows::GAimbot->AddCheckbox( "Auto Shoot", NGui::Schemes::Check, 10, 25,
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Autoshoot ) );

	NGui::Windows::GAimbot->AddCheckbox( "Auto Wall", NGui::Schemes::Check, 10, 40,
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Wall ) );

	NGui::Windows::GAimbot->AddCheckbox( "Ignore Friends", NGui::Schemes::Check, 10, 55,
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Friend ) );

	NGui::Windows::GAimbot->AddCheckbox( "Silent Aim", NGui::Schemes::Check, 10, 70,
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Silent ) );

	// Aimbot mode here

	cListbox* pAimbotMode = NGui::Windows::GAimbot->AddListbox( "Aimbot Mode", NGui::Schemes::Listbox, 10, 85, 140, 42, AimMode_Click, true );
	pAimbotMode->ClearOptions();
	pAimbotMode->AddOption( "Closest to Player" );
	pAimbotMode->AddOption( "Closest to Crosshair" );
	pAimbotMode->AddOption( "Lowest Health" );
	pAimbotMode->AddOption( "Aiming at you" );
	pAimbotMode->m_selected = 
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Mode )->Int32();
	pAimbotMode->m_beginidx = pAimbotMode->m_selected;

	// Aimbot Team here

	cListbox* pAimbotTeam = NGui::Windows::GAimbot->AddListbox( "Aimbot Team", NGui::Schemes::Listbox, 10, 135, 140, 42, NULL, true );
	pAimbotTeam->ClearOptions();
	pAimbotTeam->AddOption( "Aim at team" );
	pAimbotTeam->AddOption( "Aim at enemies" );
	pAimbotTeam->AddOption( "Aim at both" );
	pAimbotTeam->m_selected = 
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Team )->Int32();
	pAimbotTeam->m_beginidx = pAimbotTeam->m_selected;

	// Aimbot bone here

	cListbox* pAimBone = NGui::Windows::GAimbot->AddListbox( "Bone Selection", NGui::Schemes::Listbox, 10, 185, 140, 42, BoneListSelection_Click, true );
	pAimBone->ClearOptions();
	pAimBone->AddOption( "Pelvis" );
	pAimBone->AddOption( "Left Thigh" );
	pAimBone->AddOption( "Left Calf" );
	pAimBone->AddOption( "Left Foot" );
	pAimBone->AddOption( "Left Toe" );
	pAimBone->AddOption( "Right Thigh" );
	pAimBone->AddOption( "Right Calf" );
	pAimBone->AddOption( "Right Foot" );
	pAimBone->AddOption( "Right Toe" );
	pAimBone->AddOption( "Lower Spine" );
	pAimBone->AddOption( "Upper Spine" );
	pAimBone->AddOption( "Neck" );
	pAimBone->AddOption( "Head" );
	pAimBone->AddOption( "Left Upperarm" );
	pAimBone->AddOption( "Left Forearm" );
	pAimBone->AddOption( "Left Hand" );
	pAimBone->AddOption( "Right Upperarm" );
	pAimBone->AddOption( "Right Forearm" );
	pAimBone->AddOption( "Right Hand" );
	pAimBone->m_selected = 
		GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Bone )->Int32();
	pAimBone->m_beginidx = pAimBone->m_selected;

	cListbox *pAimKey = NGui::Windows::GAimbot->AddListbox( "Aim key", NGui::Schemes::Listbox, 10, 235, 140, 42, AimbotKey_Click );
	pAimKey->AddOption( "None" );
	pAimKey->AddOption( "Left mouse" );
	pAimKey->AddOption( "Right mouse" );
	pAimKey->m_selected = GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Key )->Int32();
	pAimKey->m_beginidx = pAimKey->m_selected;

	cListbox *pAimPred = NGui::Windows::GAimbot->AddListbox( "Aim Prediction", NGui::Schemes::Listbox, 10, 285, 140, 42, AimbotPrediction_Click );
	pAimPred->AddOption( "None" );
	pAimPred->AddOption( "Time Delta" );
	pAimPred->AddOption( "Frame Time" );
	pAimPred->m_selected = GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Prediction )->Int32();
	pAimPred->m_beginidx = pAimPred->m_selected;

	NGui::Windows::GAimbot->AddStatusBar( "Aim Smooth", NGui::Schemes::Status, 10, 335, 140, 16, AimSmooth_Click, GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Smooth )->Int32(), true, 10.0f );
	NGui::Windows::GAimbot->AddStatusBar( "Aim FOV", NGui::Schemes::Status, 10, 355, 140, 16, AimFOV_Click, GCvar.GetSectionByIndex( NCvar::Section::Aimbot::Singleton )->GetCvarByIndex( NCvar::Section::Aimbot::Fov )->Int32(), true, 360.0f );

	//

	NGui::Windows::GVisual->AddCheckbox( "Name ESP", NGui::Schemes::Check, 10, 10, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::NameESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Box ESP", NGui::Schemes::Check, 10, 25, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::BoxESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Health ESP", NGui::Schemes::Check, 10, 40, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::HealthESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Team ESP", NGui::Schemes::Check, 10, 55, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::TeamESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Weapon ESP", NGui::Schemes::Check, 10, 70, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::WeaponESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Friends ESP", NGui::Schemes::Check, 10, 85, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::FriendESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Radar ESP", NGui::Schemes::Check, 10, 100, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::Radar ), RadarCheckEnable_Click );
	NGui::Windows::GVisual->AddCheckbox( "Barrel ESP", NGui::Schemes::Check, 10, 115, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::BarrelESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Grenade ESP", NGui::Schemes::Check, 10, 130, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::GrenadeESP ) );
	NGui::Windows::GVisual->AddCheckbox( "Grenade Warning", NGui::Schemes::Check, 10, 145, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::GrenadeWarning ) );
	NGui::Windows::GVisual->AddCheckbox( "Aiming Warning", NGui::Schemes::Check, 10, 160, GCvar.GetSectionByIndex( NCvar::Section::Visual::Singleton )->GetCvarByIndex( NCvar::Section::Visual::AimingAtESP ) );

	NGui::ObjectGlobals::GSelectedScheme = 
		NGui::Windows::GVisual->AddListbox( "Color Editor", NGui::Schemes::Listbox, 10, 175, 140, 42, ColorSchemeEditorSelect_Click );

	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Window" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Button Top" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Button Bottom" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Button Outline" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Check On" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Check Off" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "List Front" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "List Back" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Bar Front" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Bar Back" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Text BG" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Text Font" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Text Outline" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "T Front" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "T Back" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "CT Front" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "CT Back" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Friend Front" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Friend Back" );
	NGui::ObjectGlobals::GSelectedScheme->AddOption( "Aimbot Target" );

	NGui::Windows::GVisual->AddButton( "Edit Color", NGui::Schemes::Button, 10, 225, 140, 20, RenderColors_Click );
	NGui::Windows::GVisual->AddButton( "Save Schemes", NGui::Schemes::Button, 10, 255, 140, 20, SaveSchemes_Click );
	NGui::Windows::GVisual->AddButton( "Reset Schemes", NGui::Schemes::Button, 10, 285, 140, 20, ResetSchemes_Click );

	// Removal
	NGui::Windows::GRemoval->AddCheckbox( "No Recoil", NGui::Schemes::Check, 10, 10, GCvar.GetSectionByIndex( NCvar::Section::Removal::Singleton )->GetCvarByIndex( NCvar::Section::Removal::Recoil ) );
	NGui::Windows::GRemoval->AddCheckbox( "No Spread", NGui::Schemes::Check, 10, 25, GCvar.GetSectionByIndex( NCvar::Section::Removal::Singleton )->GetCvarByIndex( NCvar::Section::Removal::Spread ) );
	NGui::Windows::GRemoval->AddCheckbox( "No Smoke", NGui::Schemes::Check, 10, 40, GCvar.GetSectionByIndex( NCvar::Section::Removal::Singleton )->GetCvarByIndex( NCvar::Section::Removal::Smoke ) );
	NGui::Windows::GRemoval->AddCheckbox( "No Flash", NGui::Schemes::Check, 10, 55, GCvar.GetSectionByIndex( NCvar::Section::Removal::Singleton )->GetCvarByIndex( NCvar::Section::Removal::Flash ) );
	NGui::Windows::GRemoval->AddCheckbox( "No Scope", NGui::Schemes::Check, 10, 70, GCvar.GetSectionByIndex( NCvar::Section::Removal::Singleton )->GetCvarByIndex( NCvar::Section::Removal::Scope ) );

	// Misc
	NGui::Windows::GMisc->AddCheckbox( "Auto Pistol", NGui::Schemes::Check, 10, 10, GCvar.GetSectionByIndex( NCvar::Section::Misc::Singleton )->GetCvarByIndex( NCvar::Section::Misc::AutoPistol ) );
	NGui::Windows::GMisc->AddCheckbox( "Bunny Hop", NGui::Schemes::Check, 10, 25, GCvar.GetSectionByIndex( NCvar::Section::Misc::Singleton )->GetCvarByIndex( NCvar::Section::Misc::Hop ) );
	
	NGui::Windows::GMisc->AddStatusBar( "Speed Hack", NGui::Schemes::Status, 10, 50, 140, 16, SpeedSelect_Click, GCvar.GetSectionByIndex( NCvar::Section::Misc::Singleton )->GetCvarByIndex( NCvar::Section::Misc::Speedhack )->Int32(), true, 10.0f );
	
	cListbox* pSpinbotList = NGui::Windows::GMisc->AddListbox( "Spinbot", NGui::Schemes::Listbox, 10, 70, 140, 42, SpinbotSelect_Click, true );
	pSpinbotList->AddOption( "None" );
	pSpinbotList->AddOption( "360 Spin" );
	pSpinbotList->AddOption( "Anti Aim 1" );
	pSpinbotList->AddOption( "Anti Aim 2" );
	pSpinbotList->AddOption( "Anti Aim 3" );
	pSpinbotList->AddOption( "Anti Aim 4" );
	pSpinbotList->m_selected = GCvar.GetSectionByIndex( NCvar::Section::Misc::Singleton )->GetCvarByIndex( NCvar::Section::Misc::Spinbot )->Int32();
	pSpinbotList->m_beginidx = pSpinbotList->m_selected;

	NGui::Windows::GMisc->AddButton( "Save All Settings", NGui::Schemes::Button, 10, 120, 140, 20, SaveAllSettings_Click );

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "gui.log", "GUI Initialisation completed!" );
#endif

	// Hook window proc

	GUI_KeyHook();

#ifdef _CSS_LOGGING_ENABLED_
	GApp.AddToLogFileA( "gui.log", "Hooked window proc [Valve001]" );
#endif
}

QAngle GSafeAngle;

void HandleAngleInput( QAngle* Angle )
{
	if( GGui.IsActive() )
	{
		Angle->x = GSafeAngle.x;
		Angle->y = GSafeAngle.y;
		Angle->z = GSafeAngle.z;
	} 
	else 
	{
		GSafeAngle = *Angle;
	}
}

WNDPROC GOriginalProc = NULL;

LRESULT CALLBACK new_WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	C_BaseEntity *pLocal = GBase.GetLocalEntity();

	if( GetCursorPos( NULL ) == TRUE || pLocal == NULL )
	{
		if( GGui.IsActive() )
		{
			GGui.SetActive( false );
		}

		return ( CallWindowProc( GOriginalProc, hWnd, uMsg, wParam, lParam ) );
	}

	if( GGui.UpdateKeyStates( wParam, lParam, uMsg ) )
	{
		return 0;
	}

	if( GGui.IsActive() )
	{
		if( uMsg == WM_MOUSEMOVE ) 
		{
			GGui.GetMouse()->Move( LOWORD( lParam ), HIWORD( lParam ) );

			return 0;
		}
	}

	return ( CallWindowProc( GOriginalProc, hWnd, uMsg, wParam, lParam ) );
}

void GUI_KeyHook()
{
	GOriginalProc = ( WNDPROC ) SetWindowLong( FindWindowA( "Valve001", 0 ), GWL_WNDPROC, ( LONG ) new_WindowProc );
}