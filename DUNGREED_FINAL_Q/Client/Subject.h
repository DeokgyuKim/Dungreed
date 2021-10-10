#pragma once

class CObserver;
class CSubject
{
	DECLARE_SINGLETON(CSubject)

protected:
	CSubject();
	virtual ~CSubject();

public:
	void Subscribe(CObserver* pObserver);
	void UnSubscribe(CObserver* pObserver);
	void Notify(int iMessage, void* pData);

protected:
	list<CObserver*>	m_ObserverList;	
};

