#ifndef SPLASHSCREENGUISHEET_H
#define SPLASHSCREENGUISHEET_H

#include "AbstractGUISheet.h"
#include <ITexture.h>

namespace Steel
{
	class SplashScreenGUISheet : public AbstractGUISheet
	{
	private:

	public:
		SplashScreenGUISheet(irr::video::ITexture *splashImage);
		virtual ~SplashScreenGUISheet();

		void Init(int windowWidth, int windowHeight, GameMaster *game);
		void Draw(irr::gui::IGUIEnvironment *irrGUI);
	};
}

#endif