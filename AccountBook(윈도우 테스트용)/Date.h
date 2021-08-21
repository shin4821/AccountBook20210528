//Date.h
#ifndef _DATE_H
#define _DATE_H

enum Month { JAN = 1, FAB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, 
	DEC = 12 };
enum WeekDay { SUN = 0, MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6 };

class Date {
public:
	Date(); //����Ʈ ������
	~Date();
	Date(int year, Month month, int day); //�Ű����� ���� ������, Date birthday(2020.JAN.01);
	Date(const Date& source); // ���������, Date other(birthday);
	Date(char(*date)); // ���������(���ڷ� �޴�), Date another((char(*)"20200101");
	static Date Today(); // static ���, ��ü ���� �ȵǼ� this ����.
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
	Date& operator--();//Yesterday ����
	Date operator--(int); //Yesterday ����
	Date& operator++();//Tomorrow ����
	Date operator++(int); //Tomorrow ����

	Date operator+(int days);//NextDate
	Date operator-(int days);//PreviousDate
	Date& operator=(const Date& sourse); // ġȯ������


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



