#ifndef GUISTATEMACHINE_H
#define GUISTATEMACHINE_H

#include <stack>
#include "GUIStates.h"

/* The GUI state machine is basically a way of determining what should be drawn on the GUI
 * at any point during gameplay. It doesn't necessarily determine the game state, though will
 * be tied to it. I'm pretty sure this lets you have nested menus and you can just pop them off
 * the stack as you go.
 */

namespace Steel
{
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