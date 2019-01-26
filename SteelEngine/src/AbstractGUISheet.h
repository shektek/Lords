#ifndef ABSTRACTGUISHEET_H
#define ABSTRACTGUISHEET_H

#include <IGUIEnvironment.h>
#include "GameMaster.h"

namespace Steel
{
	class AbstractGUISheet
	{
		protected:
			GameMaster			*m_gameMaster;

			double				m_windowWidth;
			double				m_windowHeight;
			double				m_windowRatio;
			double				m_buttonWidth;
			double				m_buttonHeight;
			double				m_buttonDistance;

		public:
			virtual ~AbstractGUISheet() {}
			virtual void Init(int windowWidth, int windowHeight, GameMaster *game) = 0;
			virtual void Draw(irr::gui::IGUIEnvironment *irrGUI) = 0;
	};
}

#endif