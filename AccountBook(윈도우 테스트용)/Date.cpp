//Date.cpp
#include"Date.h"
#include<ctime>
#include<cstdio>
#pragma warning(disable:4996)


Date::Date() {
	this->year = 1900;
	this->month = JAN;
	this->day = 1;
	this->weekDay = MON;
}


Date::Date(int year, Month month, int day) { //매개변수 생성자
	struct tm date = { 0, };

	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;

	mktime(&date);

	this->year = date.tm_year + 1900;
	this->month = static_cast<Month>(date.tm_mon + 1);
	this->day = date.tm_mday;
	this->weekDay = static_cast<WeekDay>(date.tm_wday);
}


Date::Date(const Date& source) { //복사생성자
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekDay = source.weekDay;
}

Date::Date(char(*date)) {
	struct tm exactDate = { 0, };

	//date를 this에 옮겨적는다.
	sscanf(date, "%4d%02d%02d", &this->year, &this->month, &this->day);

	exactDate.tm_year = this->year - 1900;
	exactDate.tm_mon = this->month - 1;
	exactDate.tm_mday = this->day;

	mktime(&exactDate);

	this->year = exactDate.tm_year + 1900;
	this->month = static_cast<Month>(exactDate.tm_mon + 1);
	this->day = exactDate.tm_mday;
	this->weekDay = static_cast<WeekDay>(exactDate.tm_wday);
}

Date Date::Today() {
	time_t timer;
	struct tm* today;
	Date today_;

	time(&timer);
	today = localtime(&timer);

	today_.year = today->tm_year + 1900;
	today_.month = static_cast<Month>(today->tm_mon + 1);
	today_.day = today->tm_mday;
	today_.weekDay = static_cast<WeekDay>(today->tm_wday);

	return today_;
}

Date Date::Tomorrow() {
	struct tm tomorrow = { 0, };
	Date tomorrow_; //Date() 생성자

	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1; //내일이므로 1증가

	mktime(&tomorrow);

	tomorrow_.year = tomorrow.tm_year + 1900;
	tomorrow_.month = static_cast<Month>(tomorrow.tm_mon + 1);
	tomorrow_.day = tomorrow.tm_mday;
	tomorrow_.weekDay = static_cast<WeekDay>(tomorrow.tm_wday);

	return tomorrow_; //~Date(); 소멸자
}

Date Date::Yesterday() {
	struct tm yesterday = { 0, };
	Date yesterday_;

	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1; //어제므로 1감소

	mktime(&yesterday);

	yesterday_.year = yesterday.tm_year + 1900;
	yesterday_.month = static_cast<Month>(yesterday.tm_mon + 1);
	yesterday_.day = yesterday.tm_mday;
	yesterday_.weekDay = static_cast<WeekDay>(yesterday.tm_wday);

	return yesterday_; //~Date() 
}

Date Date::PreviousDate(int days) {
	struct tm previousDate = { 0, };
	Date previousDate_;//Date()

	previousDate.tm_year = this->year - 1900;
	previousDate.tm_mon = this->month - 1;
	previousDate.tm_mday = this->day-days; //입력받는 days만큼 뺀다.

	mktime(&previousDate);

	previousDate_.year = previousDate.tm_year + 1900;
	previousDate_.month = static_cast<Month>(previousDate.tm_mon + 1);
	previousDate_.day = previousDate.tm_mday;
	previousDate_.weekDay = static_cast<WeekDay>(previousDate.tm_wday);

	return previousDate_; //~Date()
}

Date Date::NextDate(int days) {
	struct tm nextDate = { 0, };
	Date nextDate_;
	
	nextDate.tm_year = this->year - 1900;
	nextDate.tm_mon = this->month - 1;
	nextDate.tm_mday = this->day + days;//입력받은 days만큼 더한다.
	
	mktime(&nextDate);

	nextDate_.year = nextDate.tm_year + 1900;
	nextDate_.month = static_cast<Month>(nextDate.tm_mon + 1);
	nextDate_.day = nextDate.tm_mday;
	nextDate_.weekDay = static_cast<WeekDay>(nextDate.tm_wday);

	return nextDate_;
}

bool Date::IsEqual(const Date& other) {
	bool ret = false;

	if (this->year == other.year && this->month == other.month && this->day == other.day) {
		ret = true;
	}

	return ret;
}

bool Date::IsNotEqual(const Date& other) {
	bool ret = false;

	if (this->year != other.year || this->month != other.month || this->day != other.day) {
		ret = true;
	}

	return ret;
}

bool Date::IsGreaterThan(const Date& other) {
	bool ret = false;

	if (this->year > other.year) {
		ret = true;
	}
	else if (this->year == other.year && this->month > other.month) {
		ret = true;
	}
	else if (this->year == other.year && this->month == other.month && this->day > other.day) {
		ret = true;
	}
	return ret;
}

bool Date::IsLessThan(const Date& other) {
	bool ret = false;

	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year && this->month < other.month) {
		ret = true;
	}
	else if (this->year == other.year && this->month == other.month && this->day < other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator==(const Date& other) {
	bool ret = false;

	if (this->year == other.year && this->month == other.month && this->day == other.day) {
		ret = true;
	}

	return ret;
}

bool Date::operator!=(const Date& other) {
	bool ret = false;

	if (this->year != other.year || this->month != other.month || this->day != other.day) {
		ret = true;
	}

	return ret;

}

bool Date::operator>(const Date& other) {
	bool ret = false;

	if (this->year > other.year) {
		ret = true;	
	}
	else if (this->year == other.year && this->month > other.month) {
		ret = true;
	}
	else if (this->year == other.year && this->month == other.month && this->day > other.day) {
		ret = true;
	}

	return ret;
}

bool Date::operator>=(const Date& other) {
	bool ret = false;

	if (this->year > other.year) {
		ret = true;
	}
	else if (this->year == other.year && this->month > other.month) {
		ret = true;
	}
	else if (this->year == other.year && this->month == other.month && this->day >= other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator<(const Date& other) {
	bool ret = false;

	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year && this->month < other.month) {
		ret = true;	
	}
	else if (this->year == other.year && this->month == other.month && this->day < other.day) {
		ret = true;	
	}
	return ret;
}

bool Date::operator<=(const Date& other) {
	bool ret=false;

	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year && this->month < other.month) {
		ret = true;	
	}
	else if (this->year == other.year && this->month == other.month && this->day <= other.day) {
		ret = true;
	}
	return ret;
}

Date& Date::operator--() {//Yesterday 전위
	struct tm yesterday = { 0, };

	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1;

	mktime(&yesterday);

	this->year = yesterday.tm_year + 1900;
	this->month = static_cast<Month>(yesterday.tm_mon + 1);
	this->day = yesterday.tm_mday;
	this->weekDay = static_cast<WeekDay>(yesterday.tm_wday);

	return *this;
}

Date Date::operator--(int) {//Yesterday 후위
	struct tm yesterday = { 0, };
	
	Date other(*this);// 바꾸기 전 객체를 복사생성자로 복사해놓기.

	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1;

	mktime(&yesterday);

	this->year = yesterday.tm_year + 1900;
	this->month = static_cast<Month>(yesterday.tm_mon + 1);
	this->day = yesterday.tm_mday;
	this->weekDay = static_cast<WeekDay>(yesterday.tm_wday);

	return other;
}

Date& Date::operator++() { //Tomorrow 전위
	struct tm tomorrow = { 0, };

	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1;

	mktime(&tomorrow);

	this->year = tomorrow.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow.tm_mon + 1);
	this->day = tomorrow.tm_mday;
	this->weekDay = static_cast<WeekDay>(tomorrow.tm_wday);

	return *this;
}

Date Date::operator++(int) { //Tomorrow 후위
	struct tm tomorrow = { 0, };

	Date other(*this);// 복사생성자

	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1;

	mktime(&tomorrow);

	this->year = tomorrow.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow.tm_mon + 1);
	this->day = tomorrow.tm_mday;
	this->weekDay = static_cast<WeekDay>(tomorrow.tm_wday);

	return other;
}


Date Date::operator-(int days) {
	struct tm previousDate = { 0, };
	Date previousDate_;

	previousDate.tm_year = this->year - 1900;
	previousDate.tm_mon = this->month - 1;
	previousDate.tm_mday = this->day - days;

	mktime(&previousDate);

	previousDate_.year = previousDate.tm_year + 1900;
	previousDate_.month = static_cast<Month>(previousDate.tm_mon + 1);
	previousDate_.day = previousDate.tm_mday;
	previousDate_.weekDay = static_cast<WeekDay>(previousDate.tm_wday);

	return previousDate_;
}


Date Date::operator+(int days) {
	struct tm nextDate = { 0, };
	Date nextDate_;

	nextDate.tm_year = this->year - 1900;
	nextDate.tm_mon = this->month - 1;
	nextDate.tm_mday = this->day + days;

	mktime(&nextDate);

	nextDate_.year = nextDate.tm_year + 1900;
	nextDate_.month = static_cast<Month>(nextDate.tm_mon + 1);
	nextDate_.day = nextDate.tm_mday;
	nextDate_.weekDay = static_cast<WeekDay>(nextDate.tm_wday);

	return nextDate_;
}

Date& Date::operator=(const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekDay = source.weekDay;

	return *this;
}


Date::~Date(){
}

#include<iostream>
using namespace std;

#if 0
int main(int argc, char* argv[]) {
	Date date;
	Date today;
	Date tomorrow;
	Date yesterday;
	Date previousDate;
	Date nextDate;
	bool ret;
	Date advanceDate;
	Date rearDate;


	today = Date::Today();
	cout << today.GetYear() << "-" << today.GetMonth() << "-" << today.GetDay() << endl;

	tomorrow = today.Tomorrow();
	cout << tomorrow.GetYear() << "-" << tomorrow.GetMonth() << "-" << tomorrow.GetDay() << endl;

	yesterday = tomorrow.Yesterday();
	cout << yesterday.GetYear() << "-" << yesterday.GetMonth() << "-" << yesterday.GetDay() << endl;

	previousDate = tomorrow.PreviousDate(3);
	cout << previousDate.GetYear() << "-" << previousDate.GetMonth() << "-" << previousDate.GetDay() << endl;

	nextDate = previousDate.NextDate(4);
	cout << nextDate.GetYear() << "-" << nextDate.GetMonth() << "-" << nextDate.GetDay() << endl;


	ret = today.IsEqual(yesterday);
	if (ret == true) {
		cout << "같습니다." << endl;
	}
	if (today == yesterday) {
		cout << "같습니다." << endl;
	}


	ret = today.IsNotEqual(tomorrow);
	if (ret == true) {
		cout << "같지 않습니다." << endl;
	}
	if (today != tomorrow) {
		cout << "같지 않습니다." << endl;
	}



	ret = today.IsGreaterThan(previousDate);
	if (ret == true) {
		cout << "더 큽니다." << endl;
	}
	cout << "연산자함수 >." << endl;
	if (today > previousDate) {
		cout << "더 큽니다." << endl;
	}
	cout << "연산자함수 >=." << endl;
	if (today >= previousDate) {
		cout << "같거나 더 큽니다." << endl;
	}



	ret = today.IsLessThan(nextDate);
	if (ret == true) {
		cout << "더 작습니다." << endl;
	}
	cout << "연산자함수 <." << endl;
	if (today < tomorrow) {
		cout << "더 작습니다." << endl;
	}
	cout << "연산자함수 <=." << endl;
	if (today <= tomorrow) {
		cout << "같거나 더 작습니다." << endl;
	}


	Date birthday(1993, JUN, 25);
	rearDate = --birthday + 3; //전위
	cout << rearDate.GetYear() << "-" << rearDate.GetMonth() << "-" << rearDate.GetDay() << endl;

	advanceDate = birthday-- + 2; //후위
	cout << advanceDate.GetYear() << "-" << advanceDate.GetMonth() << "-" << advanceDate.GetDay() << endl;

	rearDate = ++birthday - 5;//전위
	cout<< rearDate.GetYear() << "-" << rearDate.GetMonth() << "-" << rearDate.GetDay() << endl;

	advanceDate = birthday++ - 3;//후위
	cout << advanceDate.GetYear() << "-" << advanceDate.GetMonth() << "-" << advanceDate.GetDay() << endl;



	ret = today.IsGreaterThan(previousDate);
	if (ret == true) {
		cout << "더 큽니다." << endl;
	}
	cout << "연산자함수 >." << endl;
	if (today > previousDate) {
		cout << "더 큽니다." << endl;
}
	cout << "연산자함수 >=." << endl;
	if (today >= previousDate) {
		cout << "같거나 더 큽니다." << endl;
	}


	Date another((char(*))"20200101");
	cout << another.GetYear() << "-" << another.GetMonth() << "-" << another.GetDay() << endl;


	return 0;
}


#endif



