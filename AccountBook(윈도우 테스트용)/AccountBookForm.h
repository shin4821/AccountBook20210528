//AccountBookForm.h
#ifndef _ACCOUNTBOOKFORM_H
#define _ACCOUNTBOOKFORM_H
#include<afxwin.h>
#include"resource.h"
typedef signed long int Long;

class AccountBook;
class AccountBookForm:public CDialog {
public:
	enum{IDD=IDD_ACCOUNTBOOKFORM};
public:
	virtual BOOL OnInitDialog();
	AccountBookForm(CWnd* parent = NULL);
public:
	AccountBook* accountBook;
	BOOL findButton;
	BOOL calculateButton;
public:
	CString GetCode(Long index);
	void Insert(Long index);
	void Load();
	void Save();
	void Modify(Long index);


protected:
	afx_msg void OnRecordButtonClicked();
	afx_msg void OnCalculateButtonClicked();
	afx_msg void OnAmountKillFocus();
	afx_msg void OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCorrectButtonClicked();
	afx_msg void OnFindButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()



};

#endif//_ACCOUNTBOOKFORM_H

