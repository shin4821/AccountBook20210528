//Date.h
#ifndef _DATE_H
#define _DATE_H

enum Month { JAN = 1, FAB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, 
	DEC = 12 };
enum WeekDay { SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6 };

class Date {
public:
	Date(); //디폴트 생성자
	~Date();
	Date(int year, Month month, int day); //매개변수 갖는 생성자, Date birthday(2020.JAN.01);
	Date(const Date& source); // 복사생성자, Date other(birthday);
	Date(char(*date)); // 복사생성자(문자로 받는), Date another((char(*)"20200101");
	static Date Today(); // static 멤버, 객체 생성 안되서 this 못씀.
	Date Tomorrow();
	Date Yesterday();
	Date PreviousDate(int days);
	Date NextDate(int days);
	bool IsEqual(const Date&other);
	bool IsNotEqual(const Date& other);
	bool IsGreaterThan(const Date& other);
	bool IsLessThan(const Date& other);
	bool operator==(const Date& other);//IsEqual
	bool operator!=(const Date& other);//IsNotEqual
	bool operator>(const Date& other);//IsGreaterThan
	bool operator>=(const Date& other);//IsGreaterThan
	bool operator<(const Date& other);//IsLessThan
	bool operator<=(const Date& other);//IsLessThan	
	Date& operator--();//Yesterday 전위
	Date operator--(int); //Yesterday 후위
	Date& operator++();//Tomorrow 전위
	Date operator++(int); //Tomorrow 후위

	Date operator+(int days);//NextDate
	Date operator-(int days);//PreviousDate
	Date& operator=(const Date& sourse); // 치환연산자


	int GetYear()const;
	Month GetMonth()const;
	int GetDay()const;
	WeekDay GetWeekDay()const;

private:
	int year;
	Month month;
	int day;
	WeekDay weekDay;
};

inline int Date::GetYear() const {
	return this->year;
}

inline Month Date::GetMonth() const {
	return this->month;
}

inline int Date::GetDay() const {
	return this->day;
}

inline WeekDay Date::GetWeekDay() const {
	return this->weekDay;
}


#endif//DATE_H



