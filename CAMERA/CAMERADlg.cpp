
// CAMERADlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CAMERA.h"
#include "CAMERADlg.h"
#include "Exit.h"
#include "E:\opencv3.3.0\build\include\opencv2\opencv.hpp"
//#include <sstream>

using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCAMERADlg 对话框



CCAMERADlg::CCAMERADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERA_DIALOG, pParent)
	, m_Port1(8000)									//Port1赋初值
	, m_Port2(8000)									//Port2赋初值
	, m_User(_T("admin"))							//用户名赋初值
	, m_Password(_T("abcd1234"))					//密码赋初值
{
	m_bIsLogin1 = FALSE;							//登录参数赋初值
	m_bIsLogin2 = FALSE;							//登录参数赋初值
	m_bIsPlaying1 = FALSE;							//播放参数赋初值
	m_bIsPlaying2 = FALSE;							//播放参数赋初值
	m_lPlayHandle1 = -1;							//视频变量赋初值
	m_lPlayHandle2 = -1;							//视频变量赋初值
	m_iCurChanIndex = -1;							//通道数赋初值
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCAMERADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP1);
	DDX_Control(pDX, IDC_IPADDRESS2, m_IP2);
	DDX_Text(pDX, IDC_PORT1, m_Port1);
	DDX_Text(pDX, IDC_PORT2, m_Port2);
	DDX_Text(pDX, IDC_USER, m_User);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_TREE, m_ctrlTreeChan);
}

BEGIN_MESSAGE_MAP(CCAMERADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CCAMERADlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN1, &CCAMERADlg::OnBnClickedLogin1)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN2, &CCAMERADlg::OnBnClickedLogin2)
	ON_BN_CLICKED(IDC_BUTTON_PLAY1, &CCAMERADlg::OnBnClickedButtonPlay1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CCAMERADlg::OnTvnSelchangedTree)
	ON_BN_CLICKED(IDC_BUTTON_PLAY2, &CCAMERADlg::OnBnClickedButtonPlay2)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE1, &CCAMERADlg::OnBnClickedButtonCapture1)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE2, &CCAMERADlg::OnBnClickedButtonCapture2)
	ON_BN_CLICKED(IDC_BUTTON_CAPTUREALL, &CCAMERADlg::OnBnClickedButtonCaptureall)
END_MESSAGE_MAP()


// CCAMERADlg 消息处理程序

BOOL CCAMERADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_IP1.SetAddress(192, 168, 1, 65);		//默认IP1地址
	m_IP2.SetAddress(192, 168, 1, 66);		//默认IP2地址


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCAMERADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCAMERADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCAMERADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*************************************************
函数名:    	OnButtonLogin
函数描述:	注册/注销 按钮
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::OnBnClickedLogin1()
{
	if (!m_bIsLogin1)				//登录
	{
		if (!DoLogin1())			//登录判断
			return;
		MessageBox("登录成功！\n");
		DoGetDeviceResourceCfg1();	//获取设备资源信息
		CreateDeviceTree1();		//创建通道树
		GetDlgItem(IDC_BUTTON_LOGIN1)->SetWindowText("ID1注销");
		m_bIsLogin1 = TRUE;
	}
	else
	{
		if (m_bIsPlaying1)
		{
			MessageBox("请先停止播放！\n");
			return;
		}
		NET_DVR_Logout_V30(m_struDeviceInfo1.lLoginID);
		GetDlgItem(IDC_BUTTON_LOGIN1)->SetWindowText("ID1登录");
		m_ctrlTreeChan.DeleteItem(m_hDevItem1);
		m_struDeviceInfo1.lLoginID = -1;
		m_bIsLogin1 = FALSE;
	}

	// TODO: 在此添加控件通知处理程序代码
}

void CCAMERADlg::OnBnClickedLogin2()
{
	if (!m_bIsLogin2)				//登录
	{
		if (!DoLogin2())			//登录判断
			return;
		MessageBox("登录成功！\n");
		DoGetDeviceResourceCfg2();	//获取设备资源信息
		CreateDeviceTree2();		//创建通道树
		GetDlgItem(IDC_BUTTON_LOGIN2)->SetWindowText("ID2注销");
		m_bIsLogin2 = TRUE;
	}
	else
	{
		if (m_bIsPlaying2)
		{
			MessageBox("请先停止播放！\n");
			return;
		}
		NET_DVR_Logout_V30(m_struDeviceInfo2.lLoginID);
		GetDlgItem(IDC_BUTTON_LOGIN2)->SetWindowText("ID2登录");
		m_ctrlTreeChan.DeleteItem(m_hDevItem2);
		m_struDeviceInfo2.lLoginID = -1;
		m_bIsLogin2 = FALSE;
	}

	// TODO: 在此添加控件通知处理程序代码
}

/*************************************************
函数名:    	DoLogin1
函数描述:	向设备注册
输入参数:
输出参数:
返回值:
**************************************************/
BOOL CCAMERADlg::DoLogin1()
{
	UpdateData(TRUE);
	CString DeviceIp1;											//IP1字符串
	BYTE nField1_0, nField1_1, nField1_2, nField1_3;			//IP1地址
	m_IP1.GetAddress(nField1_0, nField1_1, nField1_2, nField1_3);					//给nField赋值
	DeviceIp1.Format("%d.%d.%d.%d", nField1_0, nField1_1, nField1_2, nField1_3);	//生成IP地址字符串
	
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp1;
	memset(&DeviceInfoTmp1, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID1 = NET_DVR_Login_V30(DeviceIp1.GetBuffer(DeviceIp1.GetLength()), m_Port1, \
		m_User.GetBuffer(m_User.GetLength()), m_Password.GetBuffer(m_Password.GetLength()), &DeviceInfoTmp1);
	if (lLoginID1 == -1)
	{
		MessageBox("Login to Device failed!\n");
		return FALSE;
	}
	m_struDeviceInfo1.lLoginID = lLoginID1;
	m_struDeviceInfo1.iDeviceChanNum = DeviceInfoTmp1.byChanNum;
	m_struDeviceInfo1.iIPChanNum = DeviceInfoTmp1.byIPChanNum;
	m_struDeviceInfo1.iIPStartChan = DeviceInfoTmp1.byStartChan;
	m_struDeviceInfo1.iIPStartChan = DeviceInfoTmp1.byStartDChan;
	return TRUE;
}

BOOL CCAMERADlg::DoLogin2()
{
	UpdateData(TRUE);
	CString DeviceIp2;											//IP2字符串
	BYTE nField2_0, nField2_1, nField2_2, nField2_3;			//IP2地址
	m_IP2.GetAddress(nField2_0, nField2_1, nField2_2, nField2_3);					//给nField赋值
	DeviceIp2.Format("%d.%d.%d.%d", nField2_0, nField2_1, nField2_2, nField2_3);	//生成IP地址字符串
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp2;
	memset(&DeviceInfoTmp2, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID2 = NET_DVR_Login_V30(DeviceIp2.GetBuffer(DeviceIp2.GetLength()), m_Port2, \
		m_User.GetBuffer(m_User.GetLength()), m_Password.GetBuffer(m_Password.GetLength()), &DeviceInfoTmp2);
	if (lLoginID2 == -1)
	{
		MessageBox("Login to Device failed!\n");
		return FALSE;
	}
	m_struDeviceInfo2.lLoginID = lLoginID2;
	m_struDeviceInfo2.iDeviceChanNum = DeviceInfoTmp2.byChanNum;
	m_struDeviceInfo2.iIPChanNum = DeviceInfoTmp2.byIPChanNum;
	m_struDeviceInfo2.iIPStartChan = DeviceInfoTmp2.byStartChan;
	m_struDeviceInfo2.iIPStartChan = DeviceInfoTmp2.byStartDChan;
	return TRUE;
	return 0;
}

/*************************************************
函数名:    	DoGetDeviceResoureCfg
函数描述:	获取设备的通道资源
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::DoGetDeviceResourceCfg1()
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg1;
	memset(&IpAccessCfg1, 0, sizeof(IpAccessCfg1));
	DWORD dwReturned;

	m_struDeviceInfo1.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo1.lLoginID, \
		NET_DVR_GET_IPPARACFG_V40, 0, &IpAccessCfg1, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned);

	int i;
	if (!m_struDeviceInfo1.bIPRet)
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo1.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo1.iStartChan);
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = i + m_struDeviceInfo1.iStartChan;						//通道号
				m_struDeviceInfo1.struChanInfo[i].bEnable = TRUE;
			}
			else
			{
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = -1;														//通道号
				m_struDeviceInfo1.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "");
			}
		}
	}
	else		//支持IP接入，9000设备
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)		//模拟通道
		{
			if (i < m_struDeviceInfo1.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo1.iStartChan);
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = i + m_struDeviceInfo1.iStartChan;						//通道号
				if (IpAccessCfg1.byAnalogChanEnable[i])
				{
					m_struDeviceInfo1.struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					m_struDeviceInfo1.struChanInfo[i].bEnable = FALSE;
				}
			}
			else
			{
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = -1;														//通道号
				m_struDeviceInfo1.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "");
			}
		}
		for (i = 0; i < MAX_IP_CHANNEL; i++)			//数字通道
		{
			if (IpAccessCfg1.struStreamMode[i].uGetStream.struChanInfo.byEnable)		//ip通道在线
			{
				m_struDeviceInfo1.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable = TRUE;
				m_struDeviceInfo1.struChanInfo[i + MAX_ANALOG_CHANNUM].iChanIndex = i + IpAccessCfg1.dwStartDChan;
				sprintf(m_struDeviceInfo1.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName, "IP Camera %d", i + 1);

			}
			else
			{
				m_struDeviceInfo1.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable = FALSE;
				m_struDeviceInfo1.struChanInfo[i + MAX_ANALOG_CHANNUM].iChanIndex = -1;
			}
		}
	}
}

void CCAMERADlg::DoGetDeviceResourceCfg2()
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg2;
	memset(&IpAccessCfg2, 0, sizeof(IpAccessCfg2));
	DWORD dwReturned;

	m_struDeviceInfo2.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo2.lLoginID, \
		NET_DVR_GET_IPPARACFG_V40, 0, &IpAccessCfg2, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned);

	int i;
	if (!m_struDeviceInfo2.bIPRet)
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo2.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo2.iStartChan);
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = i + m_struDeviceInfo2.iStartChan;						//通道号
				m_struDeviceInfo2.struChanInfo[i].bEnable = TRUE;
			}
			else
			{
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = -1;														//通道号
				m_struDeviceInfo2.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "");
			}
		}
	}
	else		//支持IP接入，9000设备
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)		//模拟通道
		{
			if (i < m_struDeviceInfo2.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo2.iStartChan);
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = i + m_struDeviceInfo2.iStartChan;						//通道号
				if (IpAccessCfg2.byAnalogChanEnable[i])
				{
					m_struDeviceInfo2.struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					m_struDeviceInfo2.struChanInfo[i].bEnable = FALSE;
				}
			}
			else
			{
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = -1;														//通道号
				m_struDeviceInfo2.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "");
			}
		}
		for (i = 0; i < MAX_IP_CHANNEL; i++)			//数字通道
		{
			if (IpAccessCfg2.struStreamMode[i].uGetStream.struChanInfo.byEnable)		//ip通道在线
			{
				m_struDeviceInfo2.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable = TRUE;
				m_struDeviceInfo2.struChanInfo[i + MAX_ANALOG_CHANNUM].iChanIndex = i + IpAccessCfg2.dwStartDChan;
				sprintf(m_struDeviceInfo2.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName, "IP Camera %d", i + 1);

			}
			else
			{
				m_struDeviceInfo2.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable = FALSE;
				m_struDeviceInfo2.struChanInfo[i + MAX_ANALOG_CHANNUM].iChanIndex = -1;
			}
		}
	}
}

/*************************************************
函数名:    	CreateDeviceTree
函数描述:	创建通道树
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::CreateDeviceTree1()
{
	m_hDevItem1 = m_ctrlTreeChan.InsertItem("Dev");
	m_ctrlTreeChan.SetItemData(m_hDevItem1, DEVICETYPE * 1000);
	for (int i = 0; i<MAX_CHANNUM_V30; i++)
	{
		if (m_struDeviceInfo1.struChanInfo[i].bEnable)  //通道有效，插入通道树
		{
			HTREEITEM ChanItem = m_ctrlTreeChan.InsertItem(m_struDeviceInfo1.struChanInfo[i].chChanName, m_hDevItem1);
			m_ctrlTreeChan.SetItemData(ChanItem, CHANNELTYPE * 1000 + i);   //Data对应通道在数组中的索引
		}
	}
	m_ctrlTreeChan.Expand(m_hDevItem1, TVE_EXPAND);
}

void CCAMERADlg::CreateDeviceTree2()
{
	m_hDevItem2 = m_ctrlTreeChan.InsertItem("Dev");
	m_ctrlTreeChan.SetItemData(m_hDevItem2, DEVICETYPE * 1000);
	for (int i = 0; i<MAX_CHANNUM_V30; i++)
	{
		if (m_struDeviceInfo2.struChanInfo[i].bEnable)  //通道有效，插入通道树
		{
			HTREEITEM ChanItem = m_ctrlTreeChan.InsertItem(m_struDeviceInfo1.struChanInfo[i].chChanName, m_hDevItem2);
			m_ctrlTreeChan.SetItemData(ChanItem, CHANNELTYPE * 1000 + i);   //Data对应通道在数组中的索引
		}
	}
	m_ctrlTreeChan.Expand(m_hDevItem2, TVE_EXPAND);
}

/*************************************************
函数名:    	StartPlay1
函数描述:	开始一路播放
输入参数:   ChanIndex-通道号
输出参数:
返回值:
**************************************************/
void CCAMERADlg::StartPlay1(int iChanIndex)
{
	NET_DVR_CLIENTINFO ClientInfo1;
	ClientInfo1.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY1)->m_hWnd;
	ClientInfo1.lChannel = m_iCurChanIndex + 1;
	ClientInfo1.lLinkMode = 0;
	ClientInfo1.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n", ClientInfo1.lChannel);
	m_lPlayHandle1 = NET_DVR_RealPlay_V30(m_struDeviceInfo1.lLoginID, &ClientInfo1, NULL, NULL, TRUE);
	/*if (-1 == m_lPlayHandle)
	{
		DWORD err = NET_DVR_GetLastError();
		CString m_csErr;
		m_csErr.Format("播放出错，错误代码%d", err);
		MessageBox(m_csErr);
	}*/
	m_bIsPlaying1 = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText("停止播放");
}

void CCAMERADlg::StartPlay2(int iChanIndex)
{
	NET_DVR_CLIENTINFO ClientInfo2;
	ClientInfo2.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY2)->m_hWnd;
	ClientInfo2.lChannel = m_iCurChanIndex + 1;
	ClientInfo2.lLinkMode = 0;
	ClientInfo2.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n", ClientInfo2.lChannel);
	m_lPlayHandle2 = NET_DVR_RealPlay_V30(m_struDeviceInfo2.lLoginID, &ClientInfo2, NULL, NULL, TRUE);
	/*if (-1 == m_lPlayHandle)
	{
	DWORD err = NET_DVR_GetLastError();
	CString m_csErr;
	m_csErr.Format("播放出错，错误代码%d", err);
	MessageBox(m_csErr);
	}*/
	m_bIsPlaying2 = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText("停止播放");
}

/*************************************************
函数名:    	StopPlay1
函数描述:	停止播放
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::StopPlay1()
{
	if (m_lPlayHandle1 != -1)
	{
		NET_DVR_StopRealPlay(m_lPlayHandle1);
		m_lPlayHandle1 = -1;
		m_bIsPlaying1 = FALSE;
		GetDlgItem(IDC_STATIC_PLAY1)->Invalidate();
	}
}

void CCAMERADlg::StopPlay2()
{
	if (m_lPlayHandle2 != -1)
	{
		NET_DVR_StopRealPlay(m_lPlayHandle2);
		m_lPlayHandle2 = -1;
		m_bIsPlaying2 = FALSE;
		GetDlgItem(IDC_STATIC_PLAY2)->Invalidate();
	}
}

/*************************************************
函数名:    	GetCurChanIndex
函数描述:	获取当前选中的通道号
输入参数:
输出参数:
返回值:		当前选中的通道号
**************************************************/
int CCAMERADlg::GetCurChanIndex()
{
	return m_iCurChanIndex;
}

/*************************************************
函数名:    	OnButtonPlay
函数描述:	播放选中通道
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::OnBnClickedButtonPlay1()
{
	if (m_iCurChanIndex == -1)
	{
		MessageBox("选择一个通道");
		return;
	}
	if (!m_bIsPlaying1)
	{
		StartPlay1(m_iCurChanIndex);		//开始播放
		m_bIsPlaying1 = TRUE;
		GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText("停止播放");
	}
	else
	{
		StopPlay1();						//停止播放
		m_bIsPlaying1 = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText("播放");
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CCAMERADlg::OnBnClickedButtonPlay2()
{
	if (m_iCurChanIndex == -1)
	{
		MessageBox("选择一个通道");
		return;
	}
	if (!m_bIsPlaying2)
	{
		StartPlay2(m_iCurChanIndex);		//开始播放
		m_bIsPlaying2 = TRUE;
		GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText("停止播放");
	}
	else
	{
		StopPlay2();						//停止播放
		m_bIsPlaying2 = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText("播放");
	}
	// TODO: 在此添加控件通知处理程序代码
}

/*************************************************
函数名:    	OnButtonCapture
函数描述:	BMP抓图
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::OnBnClickedButtonCapture1()
{
	if (m_lPlayHandle1 == -1)
	{
		MessageBox("左通道未播放！");
		return;
	}
	UpdateData(TRUE);		//获取这时刻的各个变量

	char PicName1[256] = { 0 };
	//bmp抓图
	CTime CurTime = CTime::GetCurrentTime();
	sprintf(PicName1, "%04d%02d%02d_%02d%02d%02d_ch%02d_L.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
		CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo1.struChanInfo[GetCurChanIndex()].iChanIndex);
	if (NET_DVR_CapturePicture(m_lPlayHandle1, PicName1))
	{
		MessageBox("抓图成功！");
	}
	else
	{
		MessageBox("抓图失败！");
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CCAMERADlg::OnBnClickedButtonCapture2()
{
	if (m_lPlayHandle2 == -1)
	{
		MessageBox("右通道未播放！");
		return;
	}
	UpdateData(TRUE);		//获取这时刻的各个变量

	char PicName2[256] = { 0 };
	//bmp抓图
	CTime CurTime = CTime::GetCurrentTime();
	sprintf(PicName2, "%04d%02d%02d_%02d%02d%02d_ch%02d_R.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
		CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo2.struChanInfo[GetCurChanIndex()].iChanIndex);
	if (NET_DVR_CapturePicture(m_lPlayHandle2, PicName2))
	{
		MessageBox("抓图成功！");
	}
	else
	{
		MessageBox("抓图失败！");
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CCAMERADlg::OnBnClickedButtonCaptureall()
{
	if (m_lPlayHandle1 == -1 && m_lPlayHandle2 != -1)
	{
		MessageBox("左通道未播放！");
		return;
	}
	else if (m_lPlayHandle1 != -1 && m_lPlayHandle2 == -1)
	{
		MessageBox("右通道未播放！");
		return;
	}
	else if (m_lPlayHandle1 == -1 && m_lPlayHandle2 == -1)
	{
		MessageBox("左右通道均未播放！");
		return;
	}
	else
	{
		UpdateData(TRUE);		//获取这时刻的各个变量

		char PicName1[256] = { 0 };
		char PicName2[256] = { 0 };
		//bmp抓图
		CTime CurTime = CTime::GetCurrentTime();
		sprintf(PicName1, "%04d%02d%02d_%02d%02d%02d_ch%02d_L.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo1.struChanInfo[GetCurChanIndex()].iChanIndex);
		sprintf(PicName2, "%04d%02d%02d_%02d%02d%02d_ch%02d_R.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo2.struChanInfo[GetCurChanIndex()].iChanIndex);
		if (NET_DVR_CapturePicture(m_lPlayHandle1, PicName1) && NET_DVR_CapturePicture(m_lPlayHandle2, PicName2))
		{
			/*string PATH = "D:/研究生/CAMERA/x64/Debug/";
			string Name1 = PATH + PicName1;
			string Name2 = PATH + PicName2;
			Mat Img1 = imread(Name1);
			Mat Img2 = imread(Name2);
			string Path = "C:/Users/le'v'o'n/Desktop/实验图片/";
			string PicPath1 = Path + PicName1;
			string PicPath2 = Path + PicName2;
			imwrite(PicPath1, Img1);
			imwrite(PicPath2, Img2);*/
			MessageBox("抓图成功！");
		}
		else
		{
			MessageBox("抓图失败！");
		}
	}

	// TODO: 在此添加控件通知处理程序代码
}

/*************************************************
函数名:    	OnSelchangedTreeChan
函数描述:	获取选中的通道号
输入参数:
输出参数:
返回值:
**************************************************/
void CCAMERADlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();
	if (NULL == hSelected)
		return;
	DWORD itemData = m_ctrlTreeChan.GetItemData(hSelected);
	HTREEITEM hParent = NULL;
	int iType = itemData / 1000;
	int iIndex = itemData % 1000;

	switch (iType)
	{
	case DEVICETYPE:
		m_iCurChanIndex = m_iCurChanIndex - 1;
		break;
	case CHANNELTYPE:
		m_iCurChanIndex = iIndex;
		/**/
		TRACE("select chan index %d\n", iIndex);
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CCAMERADlg::OnBnClickedCancel()
{
	INT_PTR nRes;
	CExit exitDlg;
	nRes = exitDlg.DoModal();
	if (IDCANCEL == nRes)
		return;
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
