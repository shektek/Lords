#ifndef GUISTATES_H
#define GUISTATES_H

namespace Steel
{
	enum GUIState
	{
		G_NONE = 0,		//the void. If m_currentState is ever set to this
		G_GAME,			//game-state dependent GUI
		G_SPLASH,		//timed screen, no GUI
		G_LOADING,		//previous-state dependent, cancel option only
		G_MAINMENU,		//links to options, new, load and a quit button
		G_OPTIONSMENU,	//game, audio, video submenus
		G_NEWGAME,		//difficulty, scenario, etc.
		G_LOADGAME,		//show game file list
		G_CINEMATIC,	//may skip, may not... probably a fade-in effect
		G_GUIStateMAX
	};
}

#endif