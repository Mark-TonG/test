#pragma once


// CExit �Ի���

class CExit : public CDialogEx
{
	DECLARE_DYNAMIC(CExit)

public:
	CExit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExit();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
