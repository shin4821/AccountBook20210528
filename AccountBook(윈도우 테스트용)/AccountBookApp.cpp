//AccountBookApp.cpp
#include"AccountBookApp.h"
#include"AccountBookForm.h"

BOOL AccountBookApp::InitInstance() {
	AccountBookForm accountBookForm; //메인함수로 만들어주기

	this->m_pMainWnd = &accountBookForm; //메인윈도우 연결

	accountBookForm.DoModal(); //메인윈도우 출력

	return TRUE;
}

AccountBookApp accountBookApp;
