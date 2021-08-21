//Income.h
#ifndef _INCOME_H
#define _INCOME_H
#include"Account.h"
#include"Date.h"
#include<string.h>
using namespace std;
typedef double Currency;

class Income :public Account {
public:
	Income();
	Income(Date date, string sumUp, Currency amount, Currency balance, string note);
	virtual ~Income();
};

#endif//_INCOME_H

