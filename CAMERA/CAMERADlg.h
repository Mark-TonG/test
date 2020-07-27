
// CAMERADlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "GeneralDef.h"

// CCAMERADlg 对话框
class CCAMERADlg : public CDialogEx
{
// 构造
public:
	CCAMERADlg(CWnd* pParent = NULL);	// 标准构造函数

public:
	BOOL m_bIsLogin1;		//判断是否已登录
	BOOL m_bIsLogin2;		//判断是否已登录
	BOOL m_bIsPlaying1;		//判断是否正在播放
	BOOL m_bIsPlaying2;		//判断是否正在播放
	BOOL DoLogin1();			//进行登录1
	BOOL DoLogin2();			//进行登录2
	void DoGetDeviceResourceCfg1();	//获取设备通道资源
	void DoGetDeviceResourceCfg2();	//获取设备通道资源
	void CreateDeviceTree1();		//创建通道树
	void CreateDeviceTree2();		//创建通道树
	void StartPlay1(int iChanIndex);	//开始播放1
	void StartPlay2(int iChanIndex);	//开始播放2
	void StopPlay1();				//停止播放1
	void StopPlay2();				//停止播放2
	HTREEITEM m_hDevItem1;
	HTREEITEM m_hDevItem2;
	LOCAL_DEVICE_INFO m_struDeviceInfo1;
	LOCAL_DEVICE_INFO m_struDeviceInfo2;
	int m_iCurChanIndex;	//当前通道在数组中索引
	int GetCurChanIndex();	//获取通道数
	LONG m_lPlayHandle1;	//视频变量1
	LONG m_lPlayHandle2;	//视频变量2

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
