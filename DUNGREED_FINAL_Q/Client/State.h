#pragma once
template <typename T>
class State
{
public:
	virtual void	Enter(T*) = 0;
	virtual void	Execute(T*) = 0;
	virtual void	Exit(T*) = 0;
	virtual ~State() {};
};

