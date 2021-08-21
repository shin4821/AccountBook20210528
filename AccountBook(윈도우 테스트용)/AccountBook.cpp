//AccountBook.cpp
#include"AccountBook.h"
#include"Income.h"
#include"Outcome.h"

AccountBook::AccountBook(Long capacity)
	:accounts(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

AccountBook::~AccountBook() {
}

Currency AccountBook::GetBalance(Currency amount) { //+ 또는 -로 들어온다.
	Long index;
	Currency balance = 0.0;
	Account* account;
	Currency totalBalance;

	//1. 기존 장부 있었으면 기존의 balance 구한다.
	if (this->length > 0) {
		account = this->accounts.GetAt(this->length - 1);
		balance = account->GetBalance();
	}
	totalBalance = balance + amount;

	return totalBalance;
}



Long AccountBook::Record(Date date, string sumUp, Currency amount, Currency balance, string note) {
	Long index;

	//1. 입력받은 amount가 양수이면 income을 꽂는다.
	if (amount > 0.0) {
		Income* income = new Income(date, sumUp, amount, balance, note);

		if (this->length < this->capacity) {
			index = this->accounts.Store(this->length, income);
		}
		else if (this->length >= this->capacity) {
			index = this->accounts.AppendForRear(income);
			this->capacity++;
		}
		this->length++;
	}
	//2. 입력받은 amount가 음수이면 outcome을 꽂는다.
	else {
		Outcome* outcome = new Outcome(date, sumUp, amount*-1, balance, note);

		if (this->length < this->capacity) {
			index = this->accounts.Store(this->length, outcome);
		}
		else if (this->length >= this->capacity) {
			index = this->accounts.AppendForRear(outcome);
			this->capacity++;
		}
		this->length++;
	}

	return index;
}

void AccountBook::FindByDates(Date beginDate, Date endDate, Long* (*indexes), Long* count) {
	Long(*dayIndexes) = NULL;
	Long dayCount = 0;
	(*count) = 0;
	*indexes = new Long[this->length];
	Long i;
	Long j = 0;

	//1. beginDate가 endDate보다 작거나 같은 동안 반복한다.
	while (beginDate <= endDate) {
		this->accounts.BinarySearchDuplicate(&beginDate, &dayIndexes, &dayCount, CompareDates);

		i = 0;
		while (i < dayCount) {
			(*indexes)[j] = dayIndexes[i];
			(*count)++;
			j++;
			i++;
		}
		if (dayIndexes != 0) {
			delete[] dayIndexes;
		}
		beginDate = beginDate.Tomorrow();
	}
}

void AccountBook::FindByPeriod(int period, Long* (*indexes), Long* count) {
	Date beginDate;
	Date today;
	Long(*dayIndexes) = NULL;
	Long dayCount;
	Long i;
	Long j = 0;
	(*count) = 0;
	*indexes = new Long[this->length];

	//1. 오늘 날짜를 구한다.
	today = Date::Today(); 

	//2. period만큼 이전 날짜 구한다.
	beginDate = today.PreviousDate(period);

	//3. beginDate가 today보다 작거나 같을 동안 반복한다.
	while (beginDate <= today) {
		this->accounts.BinarySearchDuplicate(&beginDate, &dayIndexes, &dayCount, CompareDates);

		i = 0;
		while (i < dayCount) {
			(*indexes)[j] = dayIndexes[i];
			(*count)++;
			j++;
			i++;
		}
		if (dayIndexes != 0) {
			delete[] dayIndexes;
		}
		beginDate = beginDate.Tomorrow();
	}
	//4.indexes, count를 반환한다.
	//5. 끝낸다.
}

void AccountBook::FindBySumUp(string sumUp, Long* (*indexes), Long* count) {
	this->accounts.LinearSearchDuplicate(&sumUp, indexes, count, CompareSumUps);
}

void AccountBook::FindBySumUpAndDates(Date beginDate, Date endDate, string sumUp, Long* (*indexes), Long* count) {
	Long(*dayIndexes) = 0;
	Long dayCount = 0;
	(*count) = 0;
	*indexes = new Long[this->length];
	Long i;
	Long j = 0;
	Account* account;

	//1. beginDate가 endDate보다 작거나 같을 동안 반복한다.
	while (beginDate <= endDate) {
		this->accounts.BinarySearchDuplicate(&beginDate, &dayIndexes, &dayCount, CompareDates);
		i = 0;
		while (i < dayCount) {//해당 기간 내에 동일한 적요가 있으면 처리한다.
			account = this->accounts.GetAt(dayIndexes[i]);
			if (account->GetSumUp().compare(sumUp) == 0) {
				(*indexes)[j] = dayIndexes[i];
				(*count)++;
				j++;
			}
			i++;
		}
		if (dayIndexes != 0) {
			delete[] dayIndexes;
		}
		beginDate = beginDate.Tomorrow();
	}
}

Long AccountBook::Correct(Long index, Currency amount, string note) {
	Account* account;
	Account* previousAccount;
	Account* nextAccount;
	Long nextIndex;
	Long previousIndex;
	Currency balance = 0.0;

	account = this->accounts.GetAt(index);

	//1. index하위 장부가 있으면 balance 구해준다.
	if (index > 0) {
		previousAccount = this->accounts.GetAt(index - 1);
		balance = previousAccount->GetBalance();
	}

	//2. amount가 양수면 income, 음수면 outcome으로 Modify한다.
	if (dynamic_cast<Income*>(account)) {
		Income* income = new Income(account->GetDate(), account->GetSumUp(), amount, balance+amount, note);
		index = this->accounts.Modify(index, income);
	}
	else if (dynamic_cast<Outcome*>(account)) {
		Outcome* outcome = new Outcome(account->GetDate(), account->GetSumUp(), amount, balance-amount, note);
		index = this->accounts.Modify(index, outcome);
	}

	//3. 해당 장부 이후 꽂힌 장부가 있다면 balance를 바꿔준다.
	nextIndex = index + 1;
	while (nextIndex < this->length) {
		nextAccount = this->accounts.GetAt(nextIndex);
		amount = nextAccount->GetAmount();

		previousIndex = nextIndex - 1;
		previousAccount = this->accounts.GetAt(previousIndex);
		balance = previousAccount->GetBalance();

		//nextAmount의 amount가 양수면 income, 음수면 outcome으로 Modify한다.
		if (dynamic_cast<Income*>(nextAccount)) {
			Income* income = new Income(nextAccount->GetDate(), nextAccount->GetSumUp(), 
				amount, balance + amount, nextAccount->GetNote());
			this->accounts.Modify(nextIndex, income);
		}
		else if (dynamic_cast<Outcome*>(nextAccount)) {
			Outcome* outcome = new Outcome(nextAccount->GetDate(), nextAccount->GetSumUp(),
				amount, balance - amount, nextAccount->GetNote());
			this->accounts.Modify(nextIndex, outcome);
		}

		nextIndex++;
	}


	//4. index를 반환한다,
	return index;
}

void AccountBook::Calculate(Date beginDate, Date endDate, Currency* income, Currency* outcome, Currency* profit) {
	Long(*indexes) = 0;
	Long count = 0;
	Long i = 0;
	Account* account;
	Currency amount;
	(*profit) = 0;
	(*income) = 0;
	(*outcome) = 0;

	//1. beginDate, endDate 입력받는다.
	//2. beginDate가 endDate보다 작거나 같은 동안 반복한다.
	while (beginDate <= endDate) {
		this->accounts.BinarySearchDuplicate(&beginDate, &indexes, &count, CompareDates);

		i = 0;
		while (i < count) {
			account = this->accounts.GetAt(indexes[i]);

			amount = account->GetAmount();
			//amount가 양수이면 수익을 계산한다.
			if (dynamic_cast<Income*>(account)) {
				(*income) = (*income) + amount;
				//차익을 계산한다.
				(*profit) = (*profit) + amount;
			}
			//amount가 음수이면 지출을 계산한다.
			else if (dynamic_cast<Outcome*>(account)) {
				(*outcome) = (*outcome) + amount;
				//차익을 계산한다.
				(*profit) = (*profit) - amount;
			}


			i++;
		}
		if (indexes != 0) {
			delete[] indexes;
		}

		beginDate = beginDate.Tomorrow();
	}
}

Account* AccountBook::GetAt(Long index) {
	return this->accounts.GetAt(index);
}

AccountBook& AccountBook::operator=(const AccountBook& source) {//치환연산자
	this->accounts = source.accounts;
	this->capacity = source.capacity;
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

AccountBook::AccountBook(const AccountBook& source)
	:accounts(source.accounts) {
	this->capacity = source.capacity;
	this->length = source.length;
}


int CompareDates(void* one, void* other) {
	Account** one_ = (Account**)one;
	Date* other_ = (Date*)other;
	int ret;

	if ((*one_)->GetDate() == *other_) {
		ret = 0;
	}
	else if ((*one_)->GetDate() > * other_) {
		ret = 1;
	}
	else if ((*one_)->GetDate() < *other_) {
		ret = -1;
	}

	return ret;
}

int CompareSumUps(void* one, void* other) {
	Account** one_ = (Account**)one;
	string* other_ = (string*)other;
	int ret;

	if ((*one_)->GetSumUp().compare(*other_) == 0) {
		ret = 0;
	}
	else{
		ret = -1;
	}

	return ret;
}



#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	AccountBook accountBook;
	Date today;
	Account* account;
	Long index;
	Date date;
	Date nextDate;
	Date previousDate;
	Long(*indexes);
	Long count = 0;
	Long i = 0;
	Currency profit;
	Currency income;
	Currency outcome;
	Currency balance;

	today = Date::Today();
	previousDate = today.PreviousDate(2); //21/5/21

	balance = accountBook.GetBalance(200000.0);
	cout << balance << endl;

	index = accountBook.Record(previousDate, "용돈", 200000.0, balance, "엄마");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	balance = accountBook.GetBalance(-10000.0);
	cout << balance << endl;

	index = accountBook.Record(previousDate, "교통비", -10000.0, balance, "");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;


#if 0
	index = accountBook.Record(previousDate, "용돈", 200000.0, "엄마");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	index = accountBook.Record(previousDate, "교통비", -10000.0, "");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	index = accountBook.Record(previousDate, "식비", -10000.0, "");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	previousDate = today.PreviousDate(1); //21/5/22

	index = accountBook.Record(previousDate, "용돈", 50000.0, "아빠");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	index = accountBook.Record(previousDate, "문방구", -5000.0, "");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	index = accountBook.Record(today, "교통비", -10000.0, "");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

	index = accountBook.Record(today, "식비", -10000.0, "");
	account = accountBook.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;
#endif




#if 0

	cout << "FindByDates" << endl;

	accountBook.FindByDates(previousDate, today, &indexes, &count);
	while (i < count) {
		account = accountBook.GetAt(indexes[i]);
		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() 
			<< "/" <<account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
	}

	cout << "FindByPeriod" << endl;

	accountBook.FindByPeriod(1, &indexes, &count);
	i = 0;
	while (i < count) {
		account = accountBook.GetAt(indexes[i]);
		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp()
			<< "/" << account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
	}


	cout << "FindBySumUp" << endl;

	accountBook.FindBySumUp("용돈", &indexes, &count);
	i = 0;
	while (i < count) {
		account = accountBook.GetAt(indexes[i]);
		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp()
			<< "/" << account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
	}

	cout << "Correct" << endl;

	index = accountBook.Correct(4, "문방구", -2000.0, "수정");

	while (index < accountBook.GetLength()) {

		account = accountBook.GetAt(index);
		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp()
			<< "/" << account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;

		index++;

	}


	cout << "Calculate" << endl;

	previousDate = today.PreviousDate(2);
	accountBook.Calculate(previousDate, today, &income, &outcome, & profit);
	cout << "profit:" << profit << "income:" << income << "outcome:" << outcome << endl;


	cout << "accountBook1" << endl;
	AccountBook accountBook1(accountBook);
	i = 0;
	while (i < accountBook1.GetLength()) {
		account = accountBook1.GetAt(i);

		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp()
			<< "/" << account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;
		i++;
	}

	cout << "accountBook2" << endl;
	AccountBook accountBook2;

	index = accountBook2.Record(today, "용돈", 50000.0, "아빠");
	account = accountBook2.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;


	index = accountBook2.Record(today, "식비", -10000.0, "");
	account = accountBook2.GetAt(index);
	date = account->GetDate();
	cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp() << "/" <<
		account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;


	cout << "accountBook2 치환" << endl;
	accountBook2 = accountBook1;
	i = 0;
	while (i < accountBook2.GetLength()) {
		account = accountBook2.GetAt(i);

		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp()
			<< "/" << account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;
		i++;
	}

	cout << "accountBook 기간 내 적요" << endl;
	previousDate = today.PreviousDate(2);
	accountBook.FindBySumUpAndDates(today, today, "식비", &indexes, &count);
	i = 0;
	while (i < count) {
		account = accountBook.GetAt(indexes[i]);

		date = account->GetDate();
		cout << date.GetYear() << "/" << date.GetMonth() << "/" << date.GetDay() << "/" << account->GetSumUp()
			<< "/" << account->GetAmount() << "/" << account->GetBalance() << "/" << account->GetNote() << endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
	}

#endif




	return 0;
}


















