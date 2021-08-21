//Outcome.cpp
#include"Outcome.h"

Outcome::Outcome() {
}

Outcome::Outcome(Date date, string sumUp, Currency amount, Currency balance, string note)
	:Account(date, sumUp, amount, balance, note) {
}

Outcome::~Outcome() {
}

#if 0
#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	Outcome outcome1;
	Date today;
	Date date;
	bool ret;

	today = Date::Today();
	cout << today.GetMonth()<<"-"<<today.GetDay()<< endl;

	Outcome outcome(today, "용돈", 100000.0, 100000.0, "아빠");
	
	cout << today.GetMonth() << "-" << today.GetDay() << "-" << outcome.GetSumUp() << "-" << outcome.GetAmount() << "-" <<
		outcome.GetBalance() << "-" << outcome.GetNote() << endl;

	outcome1 = outcome;

	date = outcome1.GetDate();

	cout << date.GetMonth() << "-" << date.GetDay() << "-" << outcome1.GetSumUp() << "-" << outcome1.GetAmount() << "-" <<
		outcome1.GetBalance() << "-" << outcome1.GetNote() << endl;

	ret =outcome1.IsEqual(outcome);
	if (ret == true) {
		cout << "같습니다." << endl;
	}


	return 0;
}


#endif



