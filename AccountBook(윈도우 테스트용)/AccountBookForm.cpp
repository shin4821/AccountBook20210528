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

	//date�� ���ڿ��� �ٲ��ش�.
	Date today = account->GetDate();

	today_.Format("%04d-%02d-%02d", today.GetYear(), today.GetMonth(), today.GetDay()); //2021-05-27
	sql.Format("SELECT code FROM Account WHERE date='%s' ORDER BY code DESC;", (LPCTSTR)today_);

	//1. �⺻�ڵ��� ��¥ ���� ������ش� (210527)
	CString year = today_.Mid(2, 2);
	basicToday.Format("%s%02d%02d", year, today.GetMonth(), today.GetDay()); //210527

	//2. db, sql�� �����Ѵ�.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	//3. ������ ���,
	if (dynamic_cast<Income*>(account)) {
		//3.1. �⺻ �ڵ带 ����� ���´�.
		code.Format("%s001I", basicToday); //210527001I
		//3.2. rs�� ������ �ִ� ���,
		if (!rs.IsEOF()) {

			rs.GetFieldValue((short)0, code_);
			alphabet = code_.Right(1);

			if (alphabet != "I") {// ù��° �ڵ尡 I�� �ƴҰ�� ���ο� �����ڵ� �ִ��� ã�´�.
				rs.MoveNext();
				while (alphabet != "I" && !rs.IsEOF()) {
					rs.GetFieldValue((short)0, code_);
					alphabet = code_.Right(1);
					rs.MoveNext();
				}
			}

			//3.2.2. ���� �ڵ尡 �ִ� ���,
			if (alphabet == "I") {
				//3.2.2.1. �ش� �ڵ尡 ���� ��¥�� ���, 001�κ��� 1�� ������ �� �ٽ� ������.
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
	//4. ������ ���,
	else if (dynamic_cast<Outcome*>(account)) {
		//4.1. �⺻ �ڵ带 ����� ���´�.
		code.Format("%s001O", basicToday);
		//4.2. rs�� ������ �ִ� ���,
		if (!rs.IsEOF()) {

			rs.GetFieldValue((short)0, code_);
			alphabet = code_.Right(1);

			if (alphabet != "O") {// ù��° �ڵ尡 O�� �ƴҰ�� ���ο� �����ڵ� �ִ��� ã�´�.
				rs.MoveNext();
				while (alphabet != "O" && !rs.IsEOF()) {
					rs.GetFieldValue((short)0, code_);
					alphabet = code_.Right(1);
					rs.MoveNext();
				}
			}


			//4.2.2. ���� �ڵ尡 �ִ� ���,
			if (alphabet == "O") {
				//4.2.2.1. �ش� �ڵ尡 ���� ��¥�� ���, 001�κ� 1�� ���Ͽ� ������.
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
	//5. �ڵ带 ����Ѵ�.
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

	//1. code�� ���Ѵ�.
	code = GetCode(index);

	//2. date�� ���ڿ��� ��ȯ�Ѵ�.
	account = this->accountBook->GetAt(index);
	Date date = account->GetDate();
	date_.Format("%04d-%02d-%02d", date.GetYear(), date.GetMonth(), date.GetDay()); //2021-05-27

	//amount�� ���ڿ��� ��ȯ�Ѵ�.
	amount = account->GetAmount();
	amount_.Format("%.1lf", amount);


	//3. Insert�Ѵ�.
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



	//1. sql ������.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	//2. rs�� ���� �ƴѵ��� �ݺ��Ѵ�.
	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, date);
		rs.GetFieldValue((short)1, sumUp);
		rs.GetFieldValue((short)2, amount);
		rs.GetFieldValue((short)3, note);
		rs.GetFieldValue((short)4, code);

		//2.0. ���ڿ� amount�� Currency�� ��ȯ���ش�.
		amount_ = _tstof(amount);

		//2.1. �ش� �ڵ尡 �����ڵ����� �����ڵ����� Ȯ���Ѵ�.
		alphabet = code.Right(1);

		//2.2. �ش� ����� balance ���Ѵ�.
		balance = 0.0;
		if (this->accountBook->GetLength() > 0) {
			account = this->accountBook->GetAt(this->accountBook->GetLength() - 1);
			balance = account->GetBalance();
		}
		if (alphabet == "O") {
			amount_ = amount_ * -1;
		}
		totalBalance = balance + amount_;

		//2.3. ��¥�� Date �������� �������Ѵ�.
		year = date.m_pdate->year;
		month = static_cast<Month>(date.m_pdate->month);
		day = date.m_pdate->day;

		Date date_(year, month, day);

		//2.4. ����ο� �����Ѵ�.
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

	//1. sql ������.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	//2. ��ü ������ �����.
	db.ExecuteSQL("DELETE FROM Account");

	//3. ������� ó������ ������ �ݺ��Ѵ�.
	while (!rs.IsEOF() && i < this->accountBook->GetLength()) {
		//3.1. code���Ѵ�.
		rs.GetFieldValue((short)0, code);

		//3.2. date�� ���ڿ� ����(2021-05-27)���� �ٲ��ش�.
		account = this->accountBook->GetAt(i);
		
		date = account->GetDate();
		year = date.GetYear();
		month = date.GetMonth();
		day = date.GetDay();
		date_.Format("%04d-%02d-%02d", year, month, day);

		//3.3. amount�� ���ڿ� �������� �ٲ��ش�.
		amount = account->GetAmount();
		amount_.Format("%.1lf", amount);

		//3.3. ��� Insert �Ѵ�.
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

	//1. index��°�� �ڵ带 ���Ѵ�.
	db.OpenEx("DSN=AccountBook;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	while (i <= index) {
		rs.GetFieldValue((short)0, code);
		rs.MoveNext();
		i++;
	}
	//2. �ش� �ڵ带 Update�Ѵ�.
	account = this->accountBook->GetAt(index);
	
	//���� amount�� ���ڿ��� ��ȯ���ش�.
	amount = account->GetAmount();
	amount_.Format("%.1lf", amount);

	sql.Format("UPDATE Account SET amount='%s', note='%s' WHERE code='%s';", (LPCTSTR)amount_,
		account->GetNote().c_str(), (LPCTSTR)code);
	db.ExecuteSQL(sql);

	rs.Close();
	db.Close();
}

BOOL AccountBookForm::OnInitDialog() {
	CDialog::OnInitDialog();//�������̵�

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

	//1. �����찡 ������ ��
	  //1.1. ����θ� �����Ѵ�.
	  this->accountBook = new AccountBook;

	  //1.2.����Ʈ����Ʈ���� ��Ÿ���� �����Ѵ�.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	  //1.3. ����Ʈ�� ��Ʈ���� ����� �����.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(0, "����", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(1, "����", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(2, "�ݾ� (��)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(3, "�ܾ� (��)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(4, "���", LVCFMT_LEFT, 100);

	  //1.4. ���� ������ư�� üũ�س��´�.
	  ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_CHECKED);

	  //1.5. ����ο� �����Ѵ�.
	  Load();
	      
	  //1.6. ������ ����� ������ŭ �ݺ��Ѵ�.
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
		  //�Ҽ��� �߰�------------------------------------------------------
		  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
		  TCHAR szBuffer[64];
		  ::GetNumberFormat(NULL, NULL, amount_, &fmt, szBuffer, sizeof(szBuffer));
		  amount_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount_);
		  //-----------------------------------------------------------------


		  balance = account->GetBalance();
		  balance_.Format("%.1lf", balance);
		  //�Ҽ��� �߰�------------------------------------------------------
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

	//6. �ݾ׿��� ��Ŀ���� ���� ��
	  //6.1. �ݾ��� �д´�.
	  GetDlgItem(IDC_EDIT_AMOUNT)->GetWindowText(amount_);

	  if (amount_ !="") { // ������ �ƴϸ�
		  amount = _tstof(amount_);

		  //6.2. ���� ��ư�� �д´�.
		  retIncome = ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->GetCheck();

		  //6.3. ���� ��ư�� �д´�.
		  retOutcome = ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->GetCheck();

		  //6.4. �������� ���������� ���� �ܾ��� ���Ѵ�.
		  if (retIncome == TRUE) {
			  balance = this->accountBook->GetBalance(amount);
		  }
		  else if (retOutcome == TRUE) {
			  balance = this->accountBook->GetBalance(amount * -1);
		  }
		  //6.5. �ܾ�ĭ�� �ܾ��� ���´�.
		  balance_.Format("%.lf", balance);
		  //�Ҽ��� �߰�------------------------------------------------------
		  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
		  TCHAR szBuffer[64];
		  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
		  balance_ = szBuffer;

		  GetDlgItem(IDC_EDIT_BALANCE)->SetWindowText(balance_);
		  //-----------------------------------------------------------------

	  }
	  else {//�����̸� �ܾ�ĭ�� ����ó���Ѵ�.
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

	//2. �����ϱ� ��ư�� Ŭ������ ��
	  //2.1. ���� ��ư�� �д´�.
	  retIncome = ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->GetCheck();
	  
	  //2.2. ���� ��ư�� �д´�.
	  retOutcome = ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->GetCheck();

	  //2.3. ��¥, ����, �ݾ�, �ܾ�, ��� �д´�.
	  //��¥�� �д´�.
	  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DATE))->GetTime(timeTime);

	  year = timeTime.GetYear();
	  month = static_cast<Month>(timeTime.GetMonth());
	  day = timeTime.GetDay();

	  Date date(year,month,day);
	  // ���並 �д´�.
	  GetDlgItem(IDC_EDIT_SUMUP)->GetWindowText(sumUp);
	  // �ݾ��� �д´�.
	  GetDlgItem(IDC_EDIT_AMOUNT)->GetWindowText(amount);
	  amount.Replace(",", "");
	  amount_ = _tstof(amount); //CString->Currency ��ȯ
	  // �ܾ��� �д´�.
	  GetDlgItem(IDC_EDIT_BALANCE)->GetWindowText(balance);
	  balance.Replace(",", "");
	  balance_ = _tstof(balance);//CString->Currency ��ȯ
	  //��Ʈ�� �д´�.
	  GetDlgItem(IDC_EDIT_NOTE)->GetWindowText(note);

	  //2.4. �������� ���������� ���� ����ο� �����Ѵ�.
	  if (retIncome == TRUE) {
		  index = this->accountBook->Record(date, (LPCTSTR)sumUp, amount_, balance_, (LPCTSTR)note);
	  }
	  else if (retOutcome == TRUE) {
		  index= this->accountBook->Record(date, (LPCTSTR)sumUp, amount_*-1, balance_, (LPCTSTR)note);
	  }
	  //DB�� Insert�Ѵ�.
	  Insert(index);



	  //2.5. ������ ������ ����ο��� �ҷ��´�.
	  account = this->accountBook->GetAt(index);

	  //2.6. �ҷ��� ��θ� ����Ʈ����Ʈ�ѿ� �߰��Ѵ�.
	  Date date_ = account->GetDate();
	  sDate.Format("%04d-%02d-%02d", date_.GetYear(), date_.GetMonth(), date_.GetDay());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(index, sDate);

	  sumUp = CString(account->GetSumUp().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 1, sumUp);

	  amount_ = account->GetAmount();
	  amount.Format("%.lf", amount_);
	  //�Ҽ��� �߰�------------------------------------------------------
	  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
	  TCHAR szBuffer[64];
	  ::GetNumberFormat(NULL, NULL, amount, &fmt, szBuffer, sizeof(szBuffer));
	  amount = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 2, amount);
	  //-----------------------------------------------------------------

	  balance_ = account->GetBalance();
	  balance.Format("%.lf", balance_);
	  //�Ҽ��� �߰�------------------------------------------------------
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

	//5. ����Ʈ�� ��Ʈ�� ����Ŭ�� ���� ��
	  //5.1. ����Ʈ�� ��Ʈ���� ��ȣ�� �д´�.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetSelectionMark();

	  //5.2. �������� �������� Ȯ���Ѵ�.
	  account = this->accountBook->GetAt(index);
	  if (dynamic_cast<Income*>(account)) {// �����̸� ���� ������ư üũ�Ѵ�.
		  ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_UNCHECKED);
	  }
	  else if (dynamic_cast<Outcome*>(account)) {//�����̸� ���� ������ư üũ�Ѵ�.
		  ((CButton*)GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_UNCHECKED);
	  }
	  //5.3. ���ڸ� �о�ͼ� DatePicker�� set �Ѵ�.
	  date = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 0); //2021-05-26

	  year = date.Left(4); //2021
	  month = date.Mid(5, 2); //05
	  day = date.Right(2); //26

	  //CString���� int�� ��ȯ
	  year_ = _ttoi(year);
	  month_ = _ttoi(month);
	  day_ = _ttoi(day);

	  //CTime ����
	  CTime timeTime(year_, month_, day_, 0, 0, 0);
	  //Set���ֱ�
	  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_DATE))->SetTime(&timeTime);

	  //����, �ݾ�, �ܾ�, ��� �о�ͼ� ��ο� set�Ѵ�.
	  sumUp = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 1);
	  amount = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 2);
	  //�޸� ���ֱ�-----------------------------------------------------
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

	//4. ��ġ�� ��ư�� Ŭ������ ��
	  //4.1. ���� ����Ʈ�� ��Ʈ���� ��ȣ�� �д´�.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetSelectionMark();

	  //4.2. �ݾ�, ��� �д´�.
	  GetDlgItem(IDC_EDIT_AMOUNT)->GetWindowText(amount);
	  amount.Replace(",", "");
	  amount_ = _tstof(amount);//CString->Currency
	  GetDlgItem(IDC_EDIT_NOTE)->GetWindowText(note);

	  //4.3. ����ο��� �ش� ��� ��ģ��.
	  index = this->accountBook->Correct(index, amount_, (LPCTSTR)note);

	  //db���� ��ģ��.
	  Modify(index);

	  //4.4. ��ģ ������ ����ο��� �ҷ��´�.
	  account = this->accountBook->GetAt(index);

	  //4.5. �ҷ��� ������ ����Ʈ����Ʈ�ѿ��� ��ģ��.
	  amount_ = account->GetAmount();
	  amount.Format("%.lf", amount_);
	  //�Ҽ��� �߰�------------------------------------------------------
	  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
	  TCHAR szBuffer[64];
	  ::GetNumberFormat(NULL, NULL, amount, &fmt, szBuffer, sizeof(szBuffer));
	  amount = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 2, amount);
	  //-----------------------------------------------------------------

	  balance_ = account->GetBalance();
	  balance.Format("%.lf", balance_);
	  //�Ҽ��� �߰�------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, balance, &fmt, szBuffer, sizeof(szBuffer));
	  balance = szBuffer;

	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 3, balance);
	  //-----------------------------------------------------------------


	  note = CString(account->GetNote().c_str());
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 4, note);

	  //4.6. �ش� ��� ������ ��ΰ� ���� ���, balance�� ����Ʈ�� ��Ʈ�ѿ��� ��ģ��.
	  index++;
	  while (index < this->accountBook->GetLength()) {
		  account = this->accountBook->GetAt(index);

		  balance_ = account->GetBalance();
		  balance.Format("%.lf", balance_);
		  //�Ҽ��� �߰�------------------------------------------------------
		  ::GetNumberFormat(NULL, NULL, balance, &fmt, szBuffer, sizeof(szBuffer));
		  balance = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(index, 3, balance);
		  //-----------------------------------------------------------------

		  index++;
	  }
}

void AccountBookForm::OnFindButtonClicked() {
	//3. ã�� ��ư Ŭ������ ��
	this->findButton = TRUE;
	this->calculateButton = FALSE;

	  //3.1. ã�������츦 �����.
	  FindingForm findingForm;
	  //3.2. ã�������츦 ����Ѵ�.
	  findingForm.DoModal();


}

void AccountBookForm::OnCalculateButtonClicked() {
	//4. �����ϱ� ��ư Ŭ������ ��
	this->calculateButton = TRUE;
	this->findButton = FALSE;

	  //4.1. ã�� �����츦 �����.
	  FindingForm findingForm;
	  //4.2. ã�������츦 ����Ѵ�.
	  findingForm.DoModal();


}

void AccountBookForm::OnClose() {

	//1. �ݱ� ��ư�� Ŭ������ ��
	  //1.1. ����θ� ���ش�.
	  if (this->accountBook != NULL) {
		  //�����Ѵ�.
		  Save();

		delete this->accountBook;
		this->accountBook = NULL;
	  }
	  //1.2. ����������츦 �����Ѵ�.
	  EndDialog(0);
}















