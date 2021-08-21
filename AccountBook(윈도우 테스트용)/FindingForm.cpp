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

	//1. �����찡 ������ ��

	  //1.0. ����Ʈ����Ʈ���� ��Ÿ�� �����Ѵ�.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	  //1.1. �Ⱓ üũ�ڽ��� üũ�Ѵ�.
	  ((CButton*)GetDlgItem(IDC_BUTTON_DATE))->SetCheck(BST_CHECKED);

	  //1.2. ����Ʈ�� ��Ʈ�� ����� �����.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(0, "����", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(1, "����", LVCFMT_LEFT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(2, "�ݾ� (��)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(3, "�ܾ� (��)", LVCFMT_RIGHT, 100);
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertColumn(4, "���", LVCFMT_LEFT, 100);


	  //1.3. ����� ������ ã�´�.
	  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "�����");

	  //1.4. ã�� ��ư�� Ŭ������ ��� ����, ����, ������ ��ĭ���� �ʱ�ȭ �����ش�.
	  if (accountBookForm->findButton == TRUE) {
		  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("");
		  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("");

		  GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);
	  }
	  //1.5. �����ϱ� ��ư�� Ŭ������ ��� ����, ����, ������ ����Ѵ�.
	  else if (accountBookForm->calculateButton == TRUE) {
		  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("����");
		  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("����");
		  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("����");
		  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("����");

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
	
       //2. ã�� ��ư Ŭ������ ��

		  //2.1. �Ⱓ üũ�ڽ��� �д´�.
		  retDate = ((CButton*)GetDlgItem(IDC_BUTTON_DATE))->GetCheck();

		  //2.2. ���� üũ�ڽ��� �д´�.
		  retSumUp = ((CButton*)GetDlgItem(IDC_BUTTON_SUMUP))->GetCheck();

		  //2.3. ��������, ��������, ���� �д´�.
		  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_BEGINDATE))->GetTime(beginDate);
		  //CTime�� Date�������� �ٲ��ش�.
		  year = beginDate.GetYear();
		  month = static_cast<Month>(beginDate.GetMonth());
		  day = beginDate.GetDay();
		  Date beginDate_(year, month, day);

		  ((CDateTimeCtrl*)GetDlgItem(IDC_EDIT_ENDDATE))->GetTime(endDate);
		  //CTime�� Date�������� �ٲ��ش�.
		  year = endDate.GetYear();
		  month = static_cast<Month>(endDate.GetMonth());
		  day = endDate.GetDay();
		  Date endDate_(year, month, day);

		  GetDlgItem(IDC_EDIT_SUMUP)->GetWindowText(sumUp);

		  //2.4. ����� ������ ã�´�.
		  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "�����");


		  //2.5. ����� �������� ����ο��� ���ǿ� ���� ã�´�.
		  if (this->indexes != NULL) {
			  delete[] this->indexes;
		  }
		  // �Ⱓ�� üũ���� ���,
		  if (retDate == BST_CHECKED && retSumUp == BST_UNCHECKED) {
			  accountBookForm->accountBook->FindByDates(beginDate_, endDate_, &this->indexes, &count);
		  }
		  //  ���丸 üũ���� ���,
		  else if (retDate == BST_UNCHECKED && retSumUp == BST_CHECKED) {
			  accountBookForm->accountBook->FindBySumUp((LPCTSTR)sumUp, &this->indexes, &count);
		  }
		  //�Ⱓ, ���� �Ѵ� üũ���� ���,
		  else if (retDate == BST_CHECKED && retSumUp == BST_CHECKED) {
			  accountBookForm->accountBook->FindBySumUpAndDates(beginDate_, endDate_, (LPCTSTR)sumUp, &this->indexes, &count);
		  }
		  //2.6. ����Ʈ����Ʈ���� ��� ������ ���ش�.
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->DeleteAllItems();

		  //2.7. ã�� ������ŭ ����Ʈ����Ʈ�ѿ� �߰��Ѵ�.
		  while (i < count) {
			  account = accountBookForm->accountBook->GetAt(this->indexes[i]);

			  //�����߰�
			  date = account->GetDate();
			  year = date.GetYear();
			  month = date.GetMonth();
			  day = date.GetDay();
			  date_.Format("%04d-%02d-%02d", year, month, day);

			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(i, date_);

			  //�����߰�
			  sumUp = CString(account->GetSumUp().c_str());
			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 1, sumUp);

			  //�ݾ��߰�
			  amount = account->GetAmount();
			  amount_.Format("%.1f", amount);
			  //�Ҽ��� �߰�------------------------------------------------------
			  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
			  TCHAR szBuffer[64];
			  ::GetNumberFormat(NULL, NULL, amount_, &fmt, szBuffer, sizeof(szBuffer));
			  amount_ = szBuffer;

			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount_);
			  //-----------------------------------------------------------------


			  //�ܾ��߰�
			  balance = account->GetBalance();
			  balance_.Format("%.1f", balance);
			  //�Ҽ��� �߰�------------------------------------------------------
			  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
			  balance_ = szBuffer;

			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 3, balance_);
			  //-----------------------------------------------------------------


			  //����߰�
			  note = CString(account->GetNote().c_str());
			  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 4, note);

			  i++;
		  }

		  // ����, ����, ������ ��ĭ���� �ʱ�ȭ �����ش�.
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
	
	//3. �����ϱ� ��ư Ŭ������ ��
	  //3.1. ��������, �������� �д´�.
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

	  //3.2. ����� ������ ã�´�.
	  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "�����");

	  //3.3. ����� �������� ����ο��� �ش� �Ⱓ �� ��θ� ã�´�.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
	  }
	  accountBookForm->accountBook->FindByDates(beginDate_, endDate_, &this->indexes, &count);

	  //3.4. ����Ʈ����Ʈ���� ��� ���� ���ش�.
	  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->DeleteAllItems();

	  //3.5. ã�� ������ŭ ����Ʈ����Ʈ�ѿ� �߰��Ѵ�.
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
		  //�Ҽ��� �߰�------------------------------------------------------
		  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
		  TCHAR szBuffer[64];
		  ::GetNumberFormat(NULL, NULL, amount_, &fmt, szBuffer, sizeof(szBuffer));
		  amount_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount_);
		  //-----------------------------------------------------------------

		  balance = account->GetBalance();
		  balance_.Format("%.1f", balance);
		  //�Ҽ��� �߰�------------------------------------------------------
		  ::GetNumberFormat(NULL, NULL, balance_, &fmt, szBuffer, sizeof(szBuffer));
		  balance_ = szBuffer;

		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 3, balance_);
		  //-----------------------------------------------------------------


		  note = CString(account->GetNote().c_str());
		  ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 4, note);

		  i++;
	  }
	  //3.6. ����� �������� ����ο��� �����Ѵ�.
	  accountBookForm->accountBook->Calculate(beginDate_, endDate_, &income, &outcome, &profit);

	  //3.7. ����, ����, ������ ���꿡 �����Ѵ�.
	  income_.Format("%.1f", income);
	  //�Ҽ��� �߰�------------------------------------------------------
	  NUMBERFMT fmt = { 0, 0, 3, ".", ",", 1 };
	  TCHAR szBuffer[64];
	  ::GetNumberFormat(NULL, NULL, income_, &fmt, szBuffer, sizeof(szBuffer));
	  income_ = szBuffer;

	  GetDlgItem(IDC_EDIT_INCOME)->SetWindowText(income_);
	  //-----------------------------------------------------------------

	  outcome_.Format("%.1f", outcome);
	  //�Ҽ��� �߰�------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, outcome_, &fmt, szBuffer, sizeof(szBuffer));
	  outcome_ = szBuffer;

	  GetDlgItem(IDC_EDIT_OUTCOME)->SetWindowText(outcome_);
	  //-----------------------------------------------------------------

	  profit_.Format("%.1f", profit);
	  //�Ҽ��� �߰�------------------------------------------------------
	  ::GetNumberFormat(NULL, NULL, profit_, &fmt, szBuffer, sizeof(szBuffer));
	  profit_ = szBuffer;

	  GetDlgItem(IDC_EDIT_PROFIT)->SetWindowText(profit_);
	  //-----------------------------------------------------------------


	  //3.8. �� ���� static �����Ѵ�.
	  GetDlgItem(IDC_STATIC_INCOME)->SetWindowText("����");
	  GetDlgItem(IDC_STATIC_OUTCOME)->SetWindowText("����");
	  GetDlgItem(IDC_STATIC_PROFIT)->SetWindowText("����");
	  GetDlgItem(IDC_STATIC_CALCULATE)->SetWindowText("����");

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
	
	//4. ����Ʈ������� ����Ŭ�� ���� ��
	  //4.1. ����Ʈ����Ʈ���� ��ȣ�� �д´�.
	  index = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetSelectionMark();
	  
	  //4.2. ����, ����, �ݾ�, �ܾ�, ��� �д´�.
	  date = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 0);
	  sumUp = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 1);
	  amount = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 2);
	  //�޸� ���ֱ�-----------------------------------------------------
	  amount.Replace(",", "");
	  //----------------------------------------------------------------
	  balance = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 3);
	  note = ((CListCtrl*)GetDlgItem(IDC_LIST_ACCOUNTS))->GetItemText(index, 4);

	  //4.3. ����� ������ ã�´�.
	  accountBookForm = (AccountBookForm*)CWnd::FindWindow("#32770", "�����");

	  //4.4. ����� �������� ��ο� ���´�.
	  //�������� �������� Ȯ���� �� �ش� ��ư�� üũ�Ѵ�.
	  account = accountBookForm->accountBook->GetAt(this->indexes[index]);

	  if (dynamic_cast<Income*>(account)) {
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_UNCHECKED);
	  }
	  else if (dynamic_cast<Outcome*>(account)) {
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_OUTCOME))->SetCheck(BST_CHECKED);
		  ((CButton*)accountBookForm->GetDlgItem(IDC_BUTTON_INCOME))->SetCheck(BST_UNCHECKED);
	  }
	  //Date�� set�Ѵ�.
	  //���ڿ��� ���ڷ� �ٲ��ش�.
	  year = date.Left(4);
	  month = date.Mid(5, 2);
	  day = date.Right(2);

	  year_ = _ttoi(year);
	  month_ = _ttoi(month);
	  day_ = _ttoi(day);

	  CTime date_(year_, month_, day_, 0, 0, 0);
	  ((CDateTimeCtrl*)accountBookForm->GetDlgItem(IDC_EDIT_DATE))->SetTime(&date_);

	  //����,�ݾ�,�ܾ�,��� set�Ѵ�.
	  accountBookForm->GetDlgItem(IDC_EDIT_SUMUP)->SetWindowText(sumUp);
	  accountBookForm->GetDlgItem(IDC_EDIT_AMOUNT)->SetWindowText(amount);

	  accountBookForm->GetDlgItem(IDC_EDIT_BALANCE)->SetWindowText(balance);
	  accountBookForm->GetDlgItem(IDC_EDIT_NOTE)->SetWindowText(note);

	  //4.5. ����� �������� ����Ʈ����Ʈ�ѿ� �ش� �׸��� ���õ� ���·� ó���Ѵ�.
	  ((CListCtrl*)accountBookForm->GetDlgItem(IDC_LIST_ACCOUNTS))->SetSelectionMark(this->indexes[index]);

	  //4.6. ã�� �����츦 �����Ѵ�.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  EndDialog(0);
}


void FindingForm::OnClose() {
	//5. �ݱ� ��ư Ŭ������ ��
	if (this->indexes != NULL) {
		delete[] this->indexes;
		this->indexes = NULL;
	}

	EndDialog(0);
}
















