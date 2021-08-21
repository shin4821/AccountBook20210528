//AccountBook.h
#ifndef _ACCOUNTBOOK_H
#define _ACCOUNTBOOK_H
#include"Account.h"
#include"Array.h"
#include"Date.h"
#include<string.h>
using namespace std;
typedef double Currency;
typedef signed long int Long;

class AccountBook {
public:
	AccountBook(Long capacity = 256);
	~AccountBook();
	Currency GetBalance(Currency amount);
	Long Record(Date date, string sumUp, Currency amount, Currency balance, string note);
	void FindByDates(Date begindate, Date endDate, Long* (*indexes), Long* count);
	void FindByPeriod(int period, Long* (*indexes), Long* count);
	void FindBySumUp(string sumUp, Long* (*indexes), Long* count);
	void FindBySumUpAndDates(Date beginDate, Date endDate, string sumUp, Long* (*indexes), Long* count);
	Long Correct(Long index, Currency amount, string note);
	void Calculate(Date beginDate, Date endDate, Currency* income, Currency* outcome, Currency* profit);
	Account* GetAt(Long index);
	AccountBook& operator=(const AccountBook& source); //치환연산자
	AccountBook(const AccountBook& source); //복사생성자

	Long GetCapacity() const;
	Long GetLength() const;

private:
	Long capacity;
	Long length;
	Array<Account*> accounts;
};

int CompareDates(void* one, void* other);
int CompareSumUps(void* one, void* other);

inline Long AccountBook::GetCapacity() const{
	return this->capacity;
}

inline Long AccountBook::GetLength() const {
	return this->length;
}

#endif//_ACCOUNTBOOK_H
