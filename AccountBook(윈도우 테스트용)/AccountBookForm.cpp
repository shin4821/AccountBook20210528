//AccountBookForm.cpp
#include"AccountBookForm.h"
#include"FindingForm.h"
#include"AccountBook.h"
#include"Income.h"
#include"Outcome.h"
#include<afxcmn.h>//CListCtrl
#include<afxdtctl.h>//CDateTimeCtrl
#include<afxdb.h>

BEGIN_MESSAGE_MAP(AccountBookForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnRecordButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, OnCalculateButtonClicked)
	ON_EN_KILLFOCUS(IDC_EDIT_AMOUNT, OnAmountKillFocus)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACCOUNTS, OnListViewItemDoubleClicked)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT, OnCorrectButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

AccountBookForm::AccountBookForm(CWnd* parent)
	:CDialog(AccountBookForm::IDD, parent) {
	this->accountBook = NULL;
	this->findButton = FALSE;
	this->calculateButton = FALSE;
}

CString AccountBookForm::GetCode(Long index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql;
	CString code;
	CString code_;
	CString alphabet;
	CString number;
	int number_;
	CString basicToday;
	CString basicToday_;
	CString today_;

	Account* account = this->accountBook->GetAt(index);

	//date를 문자열로 바꿔준다.
	Date today = account->GetDate();

	today_.Format("%04d-%02d-%02d", today.GetYear(), today.GetMonth(), today.GetDay()); //2021-05-27
	sql.Format("SELECT code FROM Account WHERE date='%s' ORDER BY code DESC;", (LPCTSTR)today_);

	//1. 기본코드의 날짜 형식 만들어준다 (210527)
	CString year = today_.Mid(2, 2);
	basicToday.Format("%s%02d%02d", year, today.GetMonth(), today.GetDay()); //210527

	//2. db, sql을 오픈한다.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	//3. 수익일 경우,
	if (dynamic_cast<Income*>(account)) {
		//3.1. 기본 코드를 만들어 놓는다.
		code.Format("%s001I", basicToday); //210527001I
		//3.2. rs에 파일이 있는 경우,
		if (!rs.IsEOF()) {

			rs.GetFieldValue((short)0, code_);
			alphabet = code_.Right(1);

			if (alphabet != "I") {// 첫번째 코드가 I가 아닐경우 내부에 수익코드 있는지 찾는다.
				rs.MoveNext();
				while (alphabet != "I" && !rs.IsEOF()) {
					rs.GetFieldValue((short)0, code_);
					alphabet = code_.Right(1);
					rs.MoveNext();
				}
			}

			//3.2.2. 수입 코드가 있는 경우,
			if (alphabet == "I") {
				//3.2.2.1. 해당 코드가 오늘 날짜인 경우, 001부분을 1씩 더해준 후 다시 붙힌다.
				basicToday_ = code_.Left(6);
				if (basicToday_ == basicToday) {
					number = code_.Mid(6, 3);//001
					number_ = _ttoi(number);
					number_++;
					code.Format("%s%03dI", basicToday, number_);
				}
			}
		}
	}
	//4. 지출인 경우,
	else if (dynamic_cast<Outcome*>(account)) {
		//4.1. 기본 코드를 만들어 놓는다.
		code.Format("%s001O", basicToday);
		//4.2. rs의 파일이 있는 경우,
		if (!rs.IsEOF()) {

			rs.GetFieldValue((short)0, code_);
			alphabet = code_.Right(1);

			if (alphabet != "O") {// 첫번째 코드가 O이 아닐경우 내부에 수익코드 있는지 찾는다.
				rs.MoveNext();
				while (alphabet != "O" && !rs.IsEOF()) {
					rs.GetFieldValue((short)0, code_);
					alphabet = code_.Right(1);
					rs.MoveNext();
				}
			}


			//4.2.2. 지출 코드가 있는 경우,
			if (alphabet == "O") {
				//4.2.2.1. 해당 코드가 오늘 날짜인 경우, 001부분 1씩 더하여 붙힌다.
				basicToday_ = code_.Left(6);
				if (basicToday_ == basicToday) {
					number = code_.Mid(6, 3);
					number_ = _ttoi(number);
					number_++;
					code.Format("%s%03dO", basicToday, number_);
				}
			}
		}
	}
	//5. 코드를 출력한다.
	rs.Close();
	db.Close();
	
	return code;
}

void AccountBookForm::Insert(Long index) {
	CString sql;
	CDatabase db;
	CString code;
	Account* account;
	CString date_;
	Currency amount;
	CString amount_;

	//1. code를 구한다.
	code = GetCode(index);

	//2. date를 문자열로 변환한다.
	account = this->accountBook->GetAt(index);
	Date date = account->GetDate();
	date_.Format("%04d-%02d-%02d", date.GetYear(), date.GetMonth(), date.GetDay()); //2021-05-27

	//amount를 문자열로 변환한다.
	amount = account->GetAmount();
	amount_.Format("%.1lf", amount);


	//3. Insert한다.
	sql.Format("INSERT INTO Account(code, date, sumUp, amount, note) VALUES('%s','%s','%s','%s','%s');",
		(LPCTSTR)code, (LPCTSTR)date_, account->GetSumUp().c_str(), (LPCTSTR)amount_, account->GetNote().c_str());

	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	db.ExecuteSQL(sql);
	db.Close();
}

void AccountBookForm::Load() {
	CString sql = "SELECT date, sumUp, amount, note, code FROM Account;";
	CDatabase db;
	CRecordset rs(&db);
	Account* account;
	CDBVariant date;
	CString sumUp;
	Currency amount_;
	CString amount;
	CString note;
	CString code;
	CString alphabet;
	Currency balance;
	Currency totalBalance;
	int year;
	Month month;
	int day;



	//1. sql 날린다.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	//2. rs의 끝이 아닌동안 반복한다.
	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, date);
		rs.GetFieldValue((short)1, sumUp);
		rs.GetFieldValue((short)2, amount);
		rs.GetFieldValue((short)3, note);
		rs.GetFieldValue((short)4, code);

		//2.0. 문자열 amount를 Currency로 변환해준다.
		amount_ = _tstof(amount);

		//2.1. 해당 코드가 수입코드인지 지출코드인지 확인한다.
		alphabet = code.Right(1);

		//2.2. 해당 장부의 balance 구한다.
		balance = 0.0;
		if (this->accountBook->GetLength() > 0) {
			account = this->accountBook->GetAt(this->accountBook->GetLength() - 1);
			balance = account->GetBalance();
		}
		if (alphabet == "O") {
			amount_ = amount_ * -1;
		}
		totalBalance = balance + amount_;

		//2.3. 날짜를 Date 형식으로 재정의한다.
		year = date.m_pdate->year;
		month = static_cast<Month>(date.m_pdate->month);
		day = date.m_pdate->day;

		Date date_(year, month, day);

		//2.4. 가계부에 기재한다.
		this->accountBook->Record(date_, (LPCTSTR)sumUp, amount_, totalBalance, (LPCTSTR)note);

		rs.MoveNext();
	}
	rs.Close();
	db.Close();
}

void AccountBookForm::Save() {
	CString sql = "SELECT code FROM Account;";
	CDatabase db;
	CRecordset rs(&db);
	CString code;
	Long i = 0;
	Account* account;
	Date date;
	int year;
	Month month;
	int day;
	CString date_;
	Currency amount;
	CString amount_;

	//1. sql 날린다.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	//2. 전체 데이터 지운다.
	db.ExecuteSQL("DELETE FROM Account");

	//3. 가계부의 처음부터 끝까지 반복한다.
	while (!rs.IsEOF() && i < this->accountBook->GetLength()) {
		//3.1. code구한다.
		rs.GetFieldValue((short)0, code);

		//3.2. date를 문자열 형식(2021-05-27)으로 바꿔준다.
		account = this->accountBook->GetAt(i);
		
		date = account->GetDate();
		year = date.GetYear();
		month = date.GetMonth();
		day = date.GetDay();
		date_.Format("%04d-%02d-%02d", year, month, day);

		//3.3. amount를 문자열 형식으로 바꿔준다.
		amount = account->GetAmount();
		amount_.Format("%.1lf", amount);

		//3.3. 장부 Insert 한다.
		sql.Format("INSERT INTO Account(code, date, sumUp, amount, note) VALUES('%s','%s','%s','%s','%s');",
			(LPCTSTR)code, (LPCTSTR)date_, account->GetSumUp().c_str(), (LPCTSTR)amount_,
			account->GetNote().c_str());
		
		db.ExecuteSQL(sql);

		rs.MoveNext();
		i++;
	}
	rs.Close();
	db.Close();
}

void AccountBookForm::Modify(Long index) {
	Account* account;
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT code FROM Account;";
	CString code;
	Long i = 0;
	Currency amount;
	CString amount_;

	//1. index번째의 코드를 구한다.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	while (i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}
	//2. 해당 코드를 Update한다.
	account = this->accountBook->GetAt(index);
	
	//먼저 amount를 문자열로 변환해준다.
	amount = account->GetAmount();
	amount_.Format("%.1lf", amount);

	sql.Format("UPDATE Account SET amount='%s', note='%s' WHERE code='%s';", (LPCTSTR)amount_,
		account->GetNote().c_str(), (LPCTSTR)code);
	db.ExecuteSQL(sql);

	rs.Close();
	db.Close();
}

BOOL AccountBookForm::OnInitDialog() {
	CDialog::OnInitDialog();//오버라이딩

	Account* account;
	Long count;
	Date date;
	int year;
	Month month;
	int day;
	CString date_;
	CString sumUp;
	Currency amount;
	CString amount_;
	Currency balance;
	CString balance_;
	CString note;
	Long i = 0;

	//1. 윈도우가 생성될 때
	  //1.1. 가계부를 생성한다.
	  this->accountBook = new AccountBook;

	  //1.2.리스트뷰컨트롤의 스타일을 지정한다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	  //1.3. 리스트뷰 컨트롤의 헤더를 만든다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(0, "일자", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(1, "적요", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(2, "금액 (원)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(3, "잔액 (원)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(4, "비고", LVCFMT_LEFT, 100);

	  //1.4. 수익 라디오버튼을 체크해놓는다.
	  ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_CHECKED);

	  //1.5. 가계부에 적재한다.
	  Load();
	      
	  //1.6. 적재한 장부의 개수만큼 반복한다.
	  while (i < this->accountBook->GetLength()) {
		  account = this->accountBook->GetAt(i);

		  date = account->GetDate();

		  year = date.GetYear();
		  month = date.GetMonth();
		  day = date.GetDay();
		  date_.Format("%04d-%02d-%02d", year, month, day);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(i, date_);

		  sumUp = CString(account->GetSumUp().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 1, sumUp);

		  amount = account->GetAmount();
		  amount_.Format("%.1lf", amount);
		  //소수점 추가------------------------------------------------------
		  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
		  TCHAR szBuffer[64];
		  ::GetNumberFormat(NULL, NULL, amount_, &fmt, szBuffer, sizeof(szBuffer));
		  amount_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount_);
		  //-----------------------------------------------------------------


		  balance = account->GetBalance();
		  balance_.Format("%.1lf", balance);
		  //소수점 추가------------------------------------------------------
		  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
		  balance_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 3, balance_);
		  //-----------------------------------------------------------------


		  note = CString(account->GetNote().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 4, note);
		  
		  i++;
	  }


	  return FALSE;
}

void AccountBookForm::OnAmountKillFocus() {
	Currency amount;
	CString amount_;
	Currency balance;
	CString balance_;
	int retIncome;
	int retOutcome;

	//6. 금액에서 포커스를 잃을 때
	  //6.1. 금액을 읽는다.
	  GetDlgItem(IDC_EDIT_AMOUNT)->GetWindowText(amount_);

	  if (amount_ !="") { // 공백이 아니면
		  amount = _tstof(amount_);

		  //6.2. 수익 버튼을 읽는다.
		  retIncome = ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->GetCheck();

		  //6.3. 지출 버튼을 읽는다.
		  retOutcome = ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->GetCheck();

		  //6.4. 수익인지 지출인지에 따라 잔액을 구한다.
		  if (retIncome == TRUE) {
			  balance = this->accountBook->GetBalance(amount);
		  }
		  else if (retOutcome == TRUE) {
			  balance = this->accountBook->GetBalance(amount * -1);
		  }
		  //6.5. 잔액칸에 잔액을 적는다.
		  balance_.Format("%.lf", balance);
		  //소수점 추가------------------------------------------------------
		  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
		  TCHAR szBuffer[64];
		  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
		  balance_ = szBuffer;

		  GetDlgItem(IDC_EDIT_BALANCE)->SetWindowText(balance_);
		  //-----------------------------------------------------------------

	  }
	  else {//공백이면 잔액칸을 공백처리한다.
		  GetDlgItem(IDC_EDIT_BALANCE)->SetWindowText("");
	  }


}

void AccountBookForm::OnRecordButtonClicked() {
	int retIncome;
	int retOutcome;
	CTime timeTime;
	CString sumUp;
	CString amount;
	CString balance;
	CString note;
	Currency amount_;
	Currency balance_;
	int year;
	Month month;
	int day;
	Long index;
	Account* account;
	CString sDate;

	//2. 기재하기 버튼을 클릭했을 때
	  //2.1. 수익 버튼을 읽는다.
	  retIncome = ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->GetCheck();
	  
	  //2.2. 지출 버튼을 읽는다.
	  retOutcome = ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->GetCheck();

	  //2.3. 날짜, 적요, 금액, 잔액, 비고를 읽는다.
	  //날짜를 읽는다.
	  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DATE))->GetTime(timeTime);

	  year = timeTime.GetYear();
	  month = static_cast<Month>(timeTime.GetMonth());
	  day = timeTime.GetDay();

	  Date date(year,month,day);
	  // 적요를 읽는다.
	  GetDlgItem(IDC_EDIT_SUMUP)->GetWindowText(sumUp);
	  // 금액을 읽는다.
	  GetDlgItem(IDC_EDIT_AMOUNT)->GetWindowText(amount);
	  amount.Replace(",", "");
	  amount_ = _tstof(amount); //CString->Currency 변환
	  // 잔액을 읽는다.
	  GetDlgItem(IDC_EDIT_BALANCE)->GetWindowText(balance);
	  balance.Replace(",", "");
	  balance_ = _tstof(balance);//CString->Currency 변환
	  //노트를 읽는다.
	  GetDlgItem(IDC_EDIT_NOTE)->GetWindowText(note);

	  //2.4. 수익인지 지출인지에 따라 가계부에 기재한다.
	  if (retIncome == TRUE) {
		  index = this->accountBook->Record(date, (LPCTSTR)sumUp, amount_, balance_, (LPCTSTR)note);
	  }
	  else if (retOutcome == TRUE) {
		  index= this->accountBook->Record(date, (LPCTSTR)sumUp, amount_*-1, balance_, (LPCTSTR)note);
	  }
	  //DB에 Insert한다.
	  Insert(index);



	  //2.5. 기재한 내역을 가계부에서 불러온다.
	  account = this->accountBook->GetAt(index);

	  //2.6. 불러온 장부를 리스트뷰컨트롤에 추가한다.
	  Date date_ = account->GetDate();
	  sDate.Format("%04d-%02d-%02d", date_.GetYear(), date_.GetMonth(), date_.GetDay());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(index, sDate);

	  sumUp = CString(account->GetSumUp().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 1, sumUp);

	  amount_ = account->GetAmount();
	  amount.Format("%.lf", amount_);
	  //소수점 추가------------------------------------------------------
	  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
	  TCHAR szBuffer[64];
	  ::GetNumberFormat(NULL, NULL, amount, &fmt, szBuffer, sizeof(szBuffer));
	  amount = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 2, amount);
	  //-----------------------------------------------------------------

	  balance_ = account->GetBalance();
	  balance.Format("%.lf", balance_);
	  //소수점 추가------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, balance, &fmt, szBuffer, sizeof(szBuffer));
	  balance = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 3, balance);
	  //-----------------------------------------------------------------


	  note = CString(account->GetNote().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 4, note);
}

void AccountBookForm::OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	Long index;
	Account* account;
	CString date;
	CString sumUp;
	CString amount;
	CString balance;
	CString note;
	CString year;
	CString month;
	CString day;
	int year_;
	int month_;
	int day_;

	//5. 리스트뷰 컨트롤 더블클릭 했을 때
	  //5.1. 리스트뷰 컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetSelectionMark();

	  //5.2. 수입인지 지출인지 확인한다.
	  account = this->accountBook->GetAt(index);
	  if (dynamic_cast<Income*>(account)) {// 수입이면 수입 라디오버튼 체크한다.
		  ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_UNCHECKED);
	  }
	  else if (dynamic_cast<Outcome*>(account)) {//지출이면 지출 라디오버튼 체크한다.
		  ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_UNCHECKED);
	  }
	  //5.3. 일자를 읽어와서 DatePicker에 set 한다.
	  date = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 0); //2021-05-26

	  year = date.Left(4); //2021
	  month = date.Mid(5, 2); //05
	  day = date.Right(2); //26

	  //CString에서 int로 변환
	  year_ = _ttoi(year);
	  month_ = _ttoi(month);
	  day_ = _ttoi(day);

	  //CTime 생성
	  CTime timeTime(year_, month_, day_, 0, 0, 0);
	  //Set해주기
	  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DATE))->SetTime(&timeTime);

	  //적요, 금액, 잔액, 비고를 읽어와서 장부에 set한다.
	  sumUp = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 1);
	  amount = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 2);
	  //콤마 없애기-----------------------------------------------------
	  amount.Replace(",", "");
	  //----------------------------------------------------------------
	  balance = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 3);

	  note = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 4);

	  GetDlgItem(IDC_EDIT_SUMUP)->SetWindowText(sumUp);
	  GetDlgItem(IDC_EDIT_AMOUNT)->SetWindowText(amount);
	  GetDlgItem(IDC_EDIT_BALANCE)->SetWindowText(balance);
	  GetDlgItem(IDC_EDIT_NOTE)->SetWindowText(note);

}

void AccountBookForm::OnCorrectButtonClicked() {
	Long index;
	Account* account;
	CString amount;
	Currency amount_;
	CString balance;
	Currency balance_;
	CString note;

	//4. 고치기 버튼을 클릭했을 때
	  //4.1. 현재 리스트뷰 컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetSelectionMark();

	  //4.2. 금액, 비고 읽는다.
	  GetDlgItem(IDC_EDIT_AMOUNT)->GetWindowText(amount);
	  amount.Replace(",", "");
	  amount_ = _tstof(amount);//CString->Currency
	  GetDlgItem(IDC_EDIT_NOTE)->GetWindowText(note);

	  //4.3. 가계부에서 해당 장부 고친다.
	  index = this->accountBook->Correct(index, amount_, (LPCTSTR)note);

	  //db에서 고친다.
	  Modify(index);

	  //4.4. 고친 내역을 가계부에서 불러온다.
	  account = this->accountBook->GetAt(index);

	  //4.5. 불러온 내역을 리스트뷰컨트롤에서 고친다.
	  amount_ = account->GetAmount();
	  amount.Format("%.lf", amount_);
	  //소수점 추가------------------------------------------------------
	  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
	  TCHAR szBuffer[64];
	  ::GetNumberFormat(NULL, NULL, amount, &fmt, szBuffer, sizeof(szBuffer));
	  amount = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 2, amount);
	  //-----------------------------------------------------------------

	  balance_ = account->GetBalance();
	  balance.Format("%.lf", balance_);
	  //소수점 추가------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, balance, &fmt, szBuffer, sizeof(szBuffer));
	  balance = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 3, balance);
	  //-----------------------------------------------------------------


	  note = CString(account->GetNote().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 4, note);

	  //4.6. 해당 장부 이후의 장부가 있을 경우, balance를 리스트뷰 컨트롤에서 고친다.
	  index++;
	  while (index < this->accountBook->GetLength()) {
		  account = this->accountBook->GetAt(index);

		  balance_ = account->GetBalance();
		  balance.Format("%.lf", balance_);
		  //소수점 추가------------------------------------------------------
		  ::GetNumberFormat(NULL, NULL, balance, &fmt, szBuffer, sizeof(szBuffer));
		  balance = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 3, balance);
		  //-----------------------------------------------------------------

		  index++;
	  }
}

void AccountBookForm::OnFindButtonClicked() {
	//3. 찾기 버튼 클릭했을 때
	this->findButton = TRUE;
	this->calculateButton = FALSE;

	  //3.1. 찾기윈도우를 만든다.
	  FindingForm findingForm;
	  //3.2. 찾기윈도우를 출력한다.
	  findingForm.DoModal();


}

void AccountBookForm::OnCalculateButtonClicked() {
	//4. 정산하기 버튼 클릭했을 때
	this->calculateButton = TRUE;
	this->findButton = FALSE;

	  //4.1. 찾기 윈도우를 만든다.
	  FindingForm findingForm;
	  //4.2. 찾기윈도우를 출력한다.
	  findingForm.DoModal();


}

void AccountBookForm::OnClose() {

	//1. 닫기 버튼을 클릭했을 때
	  //1.1. 가계부를 없앤다.
	  if (this->accountBook != NULL) {
		  //저장한다.
		  Save();

		delete this->accountBook;
		this->accountBook = NULL;
	  }
	  //1.2. 가계부윈도우를 종료한다.
	  EndDialog(0);
}















