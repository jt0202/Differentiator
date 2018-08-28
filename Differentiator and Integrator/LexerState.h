#pragma once

class State
{
public:
	virtual void readNumber() = 0;
	virtual void readOperator() = 0;
	virtual void readLetter() = 0;
};

class NumberState : public State
{
public:
	void readNumber();
	void readOperator();
	void readLetter();
};

class OperatorState : public State
{
public:
	void readNumber();
	void readOperator();
	void readLetter();
};
class LetterState : public State
{
public:
	void readNumber();
	void readOperator();
	void readLetter();
};