//FindingForm.h
#ifndef _FINDINGFORM_H
#define _FINDINGFORM_H
#include<afxwin.h>
#include"resource.h"
typedef signed long int Long;

class FindingForm :public CDialog {
public:
	enum{IDD=IDD_FINDINGFORM};
public:
	virtual BOOL OnInitDialog();
	FindingForm(CWnd* parent = NULL);

	

protected:
	afx_msg void OnFindButtonClicked();
	afx_msg void OnCalculateButtonClicked();
	afx_msg void OnListViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
private:
	Long(*indexes);

};

#endif//_FINDINGFORM_H

