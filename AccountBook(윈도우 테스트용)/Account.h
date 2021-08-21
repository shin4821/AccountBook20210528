//Account.h
#ifndef _ACCOUNT_H
#define _ACCOUNT_H
#include"Date.h"
#include<string>
using namespace std;
typedef double Currency;

class Account {
public:
	Account();
	virtual ~Account() = 0; //����Ҹ���, ����������(�߻�Ŭ���� �ǹ�)
	Account(Date date, string sumUp, Currency amount, Currency balance, string note);
	bool IsEqual(const Account& other);
	bool IsNotEqual(const Account& other);
	bool operator==(const Account& other);
	bool operator!=(const Account& other);
	Account& operator=(const Account& source); //ġȯ������
	Account(const Account& source);//���������

	Date& GetDate() const;
	string& GetSumUp() const;
	Currency GetAmount() const;
	Currency GetBalance() const;
	string& GetNote() const;

protected:
	Date date;
	string sumUp;
	Currency amount;
	Currency balance;
	string note;
};

inline Date& Account::GetDate() const {
	return const_cast<Date&>(this->date);
}
inline string& Account::GetSumUp() const {
	return const_cast<string&>(this->sumUp);
}
inline Currency Account::GetAmount() const {
	return this->amount;
}
inline Currency Account::GetBalance() const {
	return this->balance;
}
inline string& Account::GetNote() const {
	return const_cast<string&>(this->note);
}


#endif//_ACCOUNT_H

