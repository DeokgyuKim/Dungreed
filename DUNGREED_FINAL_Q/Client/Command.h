#pragma once
class CObj;
class Command
{
public:
	Command();
	virtual ~Command();
	virtual void	Execute(CObj* pActor)PURE;
};

