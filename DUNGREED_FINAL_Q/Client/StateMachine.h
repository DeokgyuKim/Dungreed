#pragma once
#include "stdafx.h"
#include "State.h"

template <typename T>
class StateMachine
{
private:
	State<T>*	m_pCurrentState;
	T*			m_pOwner;
public:
	StateMachine(T* pOwner)
		:m_pOwner(pOwner)
		, m_pCurrentState(NULL)
	{}
	void		Update()
	{
		m_pCurrentState->Execute(m_pOwner);
	}
	void		ChangeState(State<T>* pState)
	{
		if (m_pCurrentState != pState)
		{
			m_pCurrentState->Exit(m_pOwner);
			m_pCurrentState = pState;
			m_pCurrentState->Enter(m_pOwner);
		}
	}
	State<T>*	GetCurrentState() { return this->m_pCurrentState; }
	void		SetCurrentState(State<T>* pState) { m_pCurrentState = pState; }
	

};