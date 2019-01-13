#ifndef GUISTATEMACHINE_H
#define GUISTATEMACHINE_H

#include <stack>

/* The GUI state machine is basically a way of determining what should be drawn on the GUI
 * at any point during gameplay. It doesn't necessarily determine the game state, though will
 * be tied to it. I'm pretty sure this lets you have nested menus and you can just pop them off
 * the stack as you go.
 */

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

	struct StateInfo
	{
		GUIState	m_currentState;
		GUIState	m_previousState;//"back"
		GUIState	m_nextState;	//transitioning into this
	};

	class GUIStateMachine
	{
	private:
		std::stack<StateInfo>	m_stack;
		bool					m_popNext;
		bool					m_stateChanged;

	public:
		GUIStateMachine();
		GUIStateMachine(GUIState startingMenu);
		~GUIStateMachine();

		void		ChangeState(GUIState next);
		void		Back();
		void		Tick();

		GUIState	GetCurrentState() const { return m_stack.top().m_currentState; }
		GUIState	GetPreviousState() const { return m_stack.top().m_previousState; }
		GUIState	GetNextState() const { return m_stack.top().m_nextState; }
		bool		UpdateRequired() { return m_stateChanged; }
	};
}

#endif