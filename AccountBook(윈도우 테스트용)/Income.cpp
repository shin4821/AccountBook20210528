//Income.cpp
#include"Income.h"
Income::Income() {
}

Income::Income(Date date, string sumUp, Currency amount, Currency balance, string note)
	:Account(date, sumUp, amount, balance, note) {
}

Income::~Income() {
}
