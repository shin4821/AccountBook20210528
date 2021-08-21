//Outcome.h
#ifndef _OUTCOME_H
#define _OUTCOME_H
#include"Account.h"
#include"Date.h"
#include<string.h>
using namespace std;
typedef double Currency;

class Outcome :public Account {
public:
	Outcome();
	Outcome(Date date, string sumUp, Currency amount, Currency balance, string note);
	virtual ~Outcome();
};

#endif//_OUTCOME_H
