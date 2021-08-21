//Account.cpp
#include"Account.h"

Account::Account()
	:sumUp(""), note("") {
	this->amount = 0.0;
	this->balance = 0.0;
}

Account::~Account() { //°¡»ó¼Ò¸êÀÚ

}

Account::Account(Date date, string sumUp, Currency amount, Currency balance, string note)
	:date(date), sumUp(sumUp), amount(amount), balance(balance), note(note) {

}

bool Account::IsEqual(const Account& other) {
	bool ret = false;

	if (this->date == other.date && this->sumUp == other.sumUp && this->amount == other.amount && 
		this->balance == other.balance && this->note == other.note) {
		ret = true;
	}

	return ret;
}

bool Account::IsNotEqual(const Account& other) {
	bool ret = false;

	if (this->date != other.date || this->sumUp != other.sumUp || this->amount != other.amount ||
		this->balance != other.balance || this->note != other.note) {
		ret = true;
	}

	return ret;
}

bool Account::operator==(const Account& other) {
	bool ret = false;

	if (this->date == other.date && this->sumUp == other.sumUp && this->amount == other.amount &&
		this->balance == other.balance && this->note == other.note) {
		ret = true;
	}

	return ret;
}

bool Account::operator!=(const Account& other) {
	bool ret = false;

	if (this->date != other.date || this->sumUp != other.sumUp || this->amount != other.amount ||
		this->balance != other.balance || this->note != other.note) {
		ret = true;
	}

	return ret;
}


Account& Account::operator=(const Account& source) {
	this->date = source.date;
	this->sumUp = source.sumUp;
	this->amount = source.amount;
	this->balance = source.balance;
	this->note = source.note;

	return *this;
}

Account::Account(const Account& source) {
	this->date = source.date;
	this->sumUp = source.sumUp;
	this->amount = source.amount;
	this->balance = source.balance;
	this->note = source.note;
}
























