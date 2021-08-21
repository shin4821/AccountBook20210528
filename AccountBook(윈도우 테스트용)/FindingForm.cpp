//FindingForm.cpp
#include"FindingForm.h"
#include"AccountBookForm.h"
#include"AccountBook.h"
#include"Income.h"
#include"Outcome.h"
#include<afxcmn.h>//CListCtrl
#include<afxdtctl.h>//CDateTimeCtrl

BEGIN_MESSAGE_MAP(FindingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, OnCalculateButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACCOUNTS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FindingForm::FindingForm(CWnd* parent)
	:CDialog(FindingForm::IDD, parent) {
	this->indexes = NULL;

}


BOOL FindingForm::OnInitDialog() {
	AccountBookForm* accountBookForm;

	//1. 윈도우가 생성될 때

	  //1.0. 리스트뷰컨트롤의 스타일 설정한다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.1. 기간 체크박스에 체크한다.
	  ((CButton*)GetDlgItem(IDC_BUTTON_DATE))->SetCheck(BST_CHECKED);

	  //1.2. 리스트뷰 컨트롤 헤더를 만든다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(0, "일자", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(1, "적요", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(2, "금액 (원)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(3, "잔액 (원)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(4, "비고", LVCFMT_LEFT, 100);


	  //1.3. 가계부 윈도우 찾는다.
	  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "가계부");

	  //1.4. 찾기 버튼을 클릭했을 경우 수입, 지출, 차액을 빈칸으로 초기화 시켜준다.
	  if (accountBookForm->findButton == TRUE) {
		  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("");

		  GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);
	  }
	  //1.5. 정산하기 버튼을 클릭했을 경우 수입, 지출, 차액을 출력한다.
	  else if (accountBookForm->calculateButton == TRUE) {
		  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("수익");
		  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("지출");
		  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("차액");
		  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("정산");

		  GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(FALSE);
	  }


	  return FALSE;
}

void FindingForm::OnFindButtonClicked() {
	AccountBookForm* accountBookForm;
	int retDate;
	int retSumUp;
	CTime beginDate;
	CTime endDate;
	CString sumUp;
	Long count;
	Long i = 0;
	Date date;
	CString date_;
	int year;
	Month month;
	int day;
	Currency amount;
	CString amount_;
	Currency balance;
	CString balance_;
	CString note;
	Account* account;
	
       //2. 찾기 버튼 클릭했을 때

		  //2.1. 기간 체크박스를 읽는다.
		  retDate = ((CButton*)GetDlgItem(IDC_BUTTON_DATE))->GetCheck();

		  //2.2. 적요 체크박스를 읽는다.
		  retSumUp = ((CButton*)GetDlgItem(IDC_BUTTON_SUMUP))->GetCheck();

		  //2.3. 시작일자, 종료일자, 적요 읽는다.
		  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_BEGINDATE))->GetTime(beginDate);
		  //CTime을 Date형식으로 바꿔준다.
		  year = beginDate.GetYear();
		  month = static_cast<Month>(beginDate.GetMonth());
		  day = beginDate.GetDay();
		  Date beginDate_(year, month, day);

		  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_ENDDATE))->GetTime(endDate);
		  //CTime을 Date형식으로 바꿔준다.
		  year = endDate.GetYear();
		  month = static_cast<Month>(endDate.GetMonth());
		  day = endDate.GetDay();
		  Date endDate_(year, month, day);

		  GetDlgItem(IDC_EDIT_SUMUP)->GetWindowText(sumUp);

		  //2.4. 가계부 윈도우 찾는다.
		  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "가계부");


		  //2.5. 가계부 윈도우의 가계부에서 조건에 따라 찾는다.
		  if (this->indexes != NULL) {
			  delete[] this->indexes;
		  }
		  // 기간만 체크했을 경우,
		  if (retDate == BST_CHECKED && retSumUp == BST_UNCHECKED) {
			  accountBookForm->accountBook->FindByDates(beginDate_, endDate_, &this->indexes, &count);
		  }
		  //  적요만 체크했을 경우,
		  else if (retDate == BST_UNCHECKED && retSumUp == BST_CHECKED) {
			  accountBookForm->accountBook->FindBySumUp((LPCTSTR)sumUp, &this->indexes, &count);
		  }
		  //기간, 적요 둘다 체크했을 경우,
		  else if (retDate == BST_CHECKED && retSumUp == BST_CHECKED) {
			  accountBookForm->accountBook->FindBySumUpAndDates(beginDate_, endDate_, (LPCTSTR)sumUp, &this->indexes, &count);
		  }
		  //2.6. 리스트뷰컨트롤의 모든 내역을 없앤다.
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->DeleteAllItems();

		  //2.7. 찾은 개수만큼 리스트뷰컨트롤에 추가한다.
		  while (i < count) {
			  account = accountBookForm->accountBook->GetAt(this->indexes[i]);

			  //일자추가
			  date = account->GetDate();
			  year = date.GetYear();
			  month = date.GetMonth();
			  day = date.GetDay();
			  date_.Format("%04d-%02d-%02d", year, month, day);

			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(i, date_);

			  //적요추가
			  sumUp = CString(account->GetSumUp().c_str());
			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 1, sumUp);

			  //금액추가
			  amount = account->GetAmount();
			  amount_.Format("%.1f", amount);
			  //소수점 추가------------------------------------------------------
			  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
			  TCHAR szBuffer[64];
			  ::GetNumberFormat(NULL, NULL, amount_, &fmt, szBuffer, sizeof(szBuffer));
			  amount_ = szBuffer;

			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount_);
			  //-----------------------------------------------------------------


			  //잔액추가
			  balance = account->GetBalance();
			  balance_.Format("%.1f", balance);
			  //소수점 추가------------------------------------------------------
			  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
			  balance_ = szBuffer;

			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 3, balance_);
			  //-----------------------------------------------------------------


			  //비고추가
			  note = CString(account->GetNote().c_str());
			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 4, note);

			  i++;
		  }

		  // 수입, 지출, 차액을 빈칸으로 초기화 시켜준다.
		  GetDlgItem(IDC_EDIT_INCOME)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_OUTCOME)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_PROFIT)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("");
	  
}

void FindingForm::OnCalculateButtonClicked() {
	AccountBookForm* accountBookForm;
	CTime beginDate;
	CTime endDate;
	Long count;
	Account* account;
	Long i = 0;
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
	Currency profit;
	Currency income;
	Currency outcome;
	CString profit_;
	CString income_;
	CString outcome_;
	
	//3. 정산하기 버튼 클릭했을 때
	  //3.1. 시작일자, 종료일자 읽는다.
	  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_BEGINDATE))->GetTime(beginDate);
	  year = beginDate.GetYear();
	  month = static_cast<Month>(beginDate.GetMonth());
	  day = beginDate.GetDay();
	  Date beginDate_(year, month, day);

	  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_ENDDATE))->GetTime(endDate);
	  year = endDate.GetYear();
	  month = static_cast<Month>(endDate.GetMonth());
	  day = endDate.GetDay();
	  Date endDate_(year, month, day);

	  //3.2. 가계부 윈도우 찾는다.
	  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "가계부");

	  //3.3. 가계부 윈도우의 가계부에서 해당 기간 내 장부를 찾는다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
	  }
	  accountBookForm->accountBook->FindByDates(beginDate_, endDate_, &this->indexes, &count);

	  //3.4. 리스트뷰컨트롤의 모든 내역 없앤다.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->DeleteAllItems();

	  //3.5. 찾은 개수만큼 리스트뷰컨트롤에 추가한다.
	  while (i < count) {
		  account = accountBookForm->accountBook->GetAt(this->indexes[i]);

		  date = account->GetDate();
		  year = date.GetYear();
		  month =date.GetMonth();
		  day = date.GetDay();

		  date_.Format("%04d-%02d-%02d", year, month, day);
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(i, date_);

		  sumUp = CString(account->GetSumUp().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 1, sumUp);

		  amount = account->GetAmount();
		  amount_.Format("%.1f", amount);
		  //소수점 추가------------------------------------------------------
		  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
		  TCHAR szBuffer[64];
		  ::GetNumberFormat(NULL, NULL, amount_, &fmt, szBuffer, sizeof(szBuffer));
		  amount_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount_);
		  //-----------------------------------------------------------------

		  balance = account->GetBalance();
		  balance_.Format("%.1f", balance);
		  //소수점 추가------------------------------------------------------
		  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
		  balance_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 3, balance_);
		  //-----------------------------------------------------------------


		  note = CString(account->GetNote().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 4, note);

		  i++;
	  }
	  //3.6. 가계부 윈도우의 가계부에서 정산한다.
	  accountBookForm->accountBook->Calculate(beginDate_, endDate_, &income, &outcome, &profit);

	  //3.7. 수익, 지출, 차액을 정산에 기재한다.
	  income_.Format("%.1f", income);
	  //소수점 추가------------------------------------------------------
	  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
	  TCHAR szBuffer[64];
	  ::GetNumberFormat(NULL, NULL, income_, &fmt, szBuffer, sizeof(szBuffer));
	  income_ = szBuffer;

	  GetDlgItem(IDC_EDIT_INCOME)->SetWindowText(income_);
	  //-----------------------------------------------------------------

	  outcome_.Format("%.1f", outcome);
	  //소수점 추가------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, outcome_, &fmt, szBuffer, sizeof(szBuffer));
	  outcome_ = szBuffer;

	  GetDlgItem(IDC_EDIT_OUTCOME)->SetWindowText(outcome_);
	  //-----------------------------------------------------------------

	  profit_.Format("%.1f", profit);
	  //소수점 추가------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, profit_, &fmt, szBuffer, sizeof(szBuffer));
	  profit_ = szBuffer;

	  GetDlgItem(IDC_EDIT_PROFIT)->SetWindowText(profit_);
	  //-----------------------------------------------------------------


	  //3.8. 그 외의 static 기재한다.
	  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("수입");
	  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("지출");
	  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("차액");
	  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("정산");

}

void FindingForm::OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	AccountBookForm* accountBookForm;
	Account* account;
	Long index;
	CString date;
	CString year;
	CString month;
	CString day;
	int year_;
	int month_;
	int day_;
	CString sumUp;
	CString amount;
	CString balance;
	CString note;
	
	//4. 리스트뷰아이템 더블클릭 했을 때
	  //4.1. 리스트뷰컨트롤의 번호를 읽는다.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetSelectionMark();
	  
	  //4.2. 일자, 적요, 금액, 잔액, 비고를 읽는다.
	  date = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 0);
	  sumUp = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 1);
	  amount = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 2);
	  //콤마 없애기-----------------------------------------------------
	  amount.Replace(",", "");
	  //----------------------------------------------------------------
	  balance = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 3);
	  note = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 4);

	  //4.3. 가계부 윈도우 찾는다.
	  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "가계부");

	  //4.4. 가계부 윈도우의 장부에 적는다.
	  //지출인지 수입인지 확인한 후 해당 버튼에 체크한다.
	  account = accountBookForm->accountBook->GetAt(this->indexes[index]);

	  if (dynamic_cast<Income*>(account)) {
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_UNCHECKED);
	  }
	  else if (dynamic_cast<Outcome*>(account)) {
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_UNCHECKED);
	  }
	  //Date를 set한다.
	  //문자열을 숫자로 바꿔준다.
	  year = date.Left(4);
	  month = date.Mid(5, 2);
	  day = date.Right(2);

	  year_ = _ttoi(year);
	  month_ = _ttoi(month);
	  day_ = _ttoi(day);

	  CTime date_(year_, month_, day_, 0, 0, 0);
	  ((CDateTimeCtrl*)accountBookForm->GetDlgItem(IDC_EDIT_DATE))->SetTime(&date_);

	  //적요,금액,잔액,비고를 set한다.
	  accountBookForm->GetDlgItem(IDC_EDIT_SUMUP)->SetWindowText(sumUp);
	  accountBookForm->GetDlgItem(IDC_EDIT_AMOUNT)->SetWindowText(amount);

	  accountBookForm->GetDlgItem(IDC_EDIT_BALANCE)->SetWindowText(balance);
	  accountBookForm->GetDlgItem(IDC_EDIT_NOTE)->SetWindowText(note);

	  //4.5. 가계부 윈도우의 리스트뷰컨트롤에 해당 항목을 선택된 상태로 처리한다.
	  ((CListCtrl*)accountBookForm->GetDlgItem(IDC_LIST_ACCOUNTS))->SetSelectionMark(this->indexes[index]);

	  //4.6. 찾기 윈도우를 종료한다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  EndDialog(0);
}


void FindingForm::OnClose() {
	//5. 닫기 버튼 클릭했을 때
	if (this->indexes != NULL) {
		delete[] this->indexes;
		this->indexes = NULL;
	}

	EndDialog(0);
}
















