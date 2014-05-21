#ifndef __INIT_GUI__
#define __INIT_GUI__

void Init_Gui();

namespace NGui
{
	namespace Schemes
	{
		extern cColorScheme		Window;
		extern cColorScheme		Button;
		extern cColorScheme		Check;
		extern cColorScheme		Listbox;
		extern cColorScheme		Status;
		extern cColorScheme		Text;

		namespace Team
		{
			extern cColorScheme	T;
			extern cColorScheme	CT;
			extern cColorScheme	Friend;
			extern cColorScheme	Target;
		};
	};

	namespace Core
	{
		extern cFillArea		GFill;
		extern cDrawText		GText;
	};

	namespace Windows
	{
		extern cWindow*			GAimbot;
		extern cWindow*			GVisual;
		extern cWindow*			GRemoval;
		extern cWindow*			GMisc;
	};

	namespace ObjectGlobals
	{
		extern cListbox*		GSelectedScheme;
	};
};

#endif