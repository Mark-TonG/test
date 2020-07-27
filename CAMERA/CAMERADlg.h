
// CAMERADlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "GeneralDef.h"

// CCAMERADlg �Ի���
class CCAMERADlg : public CDialogEx
{
// ����
public:
	CCAMERADlg(CWnd* pParent = NULL);	// ��׼���캯��

public:
	BOOL m_bIsLogin1;		//�ж��Ƿ��ѵ�¼
	BOOL m_bIsLogin2;		//�ж��Ƿ��ѵ�¼
	BOOL m_bIsPlaying1;		//�ж��Ƿ����ڲ���
	BOOL m_bIsPlaying2;		//�ж��Ƿ����ڲ���
	BOOL DoLogin1();			//���е�¼1
	BOOL DoLogin2();			//���е�¼2
	void DoGetDeviceResourceCfg1();	//��ȡ�豸ͨ����Դ
	void DoGetDeviceResourceCfg2();	//��ȡ�豸ͨ����Դ
	void CreateDeviceTree1();		//����ͨ����
	void CreateDeviceTree2();		//����ͨ����
	void StartPlay1(int iChanIndex);	//��ʼ����1
	void StartPlay2(int iChanIndex);	//��ʼ����2
	void StopPlay1();				//ֹͣ����1
	void StopPlay2();				//ֹͣ����2
	HTREEITEM m_hDevItem1;
	HTREEITEM m_hDevItem2;
	LOCAL_DEVICE_INFO m_struDeviceInfo1;
	LOCAL_DEVICE_INFO m_struDeviceInfo2;
	int m_iCurChanIndex;	//��ǰͨ��������������
	int GetCurChanIndex();	//��ȡͨ����
	LONG m_lPlayHandle1;	//��Ƶ����1
	LONG m_lPlayHandle2;	//��Ƶ����2

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLogin1();
	CIPAddressCtrl m_IP1;
	CIPAddressCtrl m_IP2;
	UINT m_Port1;
	UINT m_Port2;
	CString m_User;
	CString m_Password;
	afx_msg void OnBnClickedLogin2();
	CTreeCtrl m_ctrlTreeChan;
	afx_msg void OnBnClickedButtonPlay1();
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPlay2();
	afx_msg void OnBnClickedButtonCapture1();
	afx_msg void OnBnClickedButtonCapture2();
	afx_msg void OnBnClickedButtonCaptureall();
};
