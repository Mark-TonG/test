
// CAMERADlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCAMERADlg �Ի���



CCAMERADlg::CCAMERADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERA_DIALOG, pParent)
	, m_Port1(8000)									//Port1����ֵ
	, m_Port2(8000)									//Port2����ֵ
	, m_User(_T("admin"))							//�û�������ֵ
	, m_Password(_T("abcd1234"))					//���븳��ֵ
{
	m_bIsLogin1 = FALSE;							//��¼��������ֵ
	m_bIsLogin2 = FALSE;							//��¼��������ֵ
	m_bIsPlaying1 = FALSE;							//���Ų�������ֵ
	m_bIsPlaying2 = FALSE;							//���Ų�������ֵ
	m_lPlayHandle1 = -1;							//��Ƶ��������ֵ
	m_lPlayHandle2 = -1;							//��Ƶ��������ֵ
	m_iCurChanIndex = -1;							//ͨ��������ֵ
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


// CCAMERADlg ��Ϣ�������

BOOL CCAMERADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_IP1.SetAddress(192, 168, 1, 65);		//Ĭ��IP1��ַ
	m_IP2.SetAddress(192, 168, 1, 66);		//Ĭ��IP2��ַ


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCAMERADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCAMERADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*************************************************
������:    	OnButtonLogin
��������:	ע��/ע�� ��ť
�������:
�������:
����ֵ:
**************************************************/
void CCAMERADlg::OnBnClickedLogin1()
{
	if (!m_bIsLogin1)				//��¼
	{
		if (!DoLogin1())			//��¼�ж�
			return;
		MessageBox("��¼�ɹ���\n");
		DoGetDeviceResourceCfg1();	//��ȡ�豸��Դ��Ϣ
		CreateDeviceTree1();		//����ͨ����
		GetDlgItem(IDC_BUTTON_LOGIN1)->SetWindowText("ID1ע��");
		m_bIsLogin1 = TRUE;
	}
	else
	{
		if (m_bIsPlaying1)
		{
			MessageBox("����ֹͣ���ţ�\n");
			return;
		}
		NET_DVR_Logout_V30(m_struDeviceInfo1.lLoginID);
		GetDlgItem(IDC_BUTTON_LOGIN1)->SetWindowText("ID1��¼");
		m_ctrlTreeChan.DeleteItem(m_hDevItem1);
		m_struDeviceInfo1.lLoginID = -1;
		m_bIsLogin1 = FALSE;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCAMERADlg::OnBnClickedLogin2()
{
	if (!m_bIsLogin2)				//��¼
	{
		if (!DoLogin2())			//��¼�ж�
			return;
		MessageBox("��¼�ɹ���\n");
		DoGetDeviceResourceCfg2();	//��ȡ�豸��Դ��Ϣ
		CreateDeviceTree2();		//����ͨ����
		GetDlgItem(IDC_BUTTON_LOGIN2)->SetWindowText("ID2ע��");
		m_bIsLogin2 = TRUE;
	}
	else
	{
		if (m_bIsPlaying2)
		{
			MessageBox("����ֹͣ���ţ�\n");
			return;
		}
		NET_DVR_Logout_V30(m_struDeviceInfo2.lLoginID);
		GetDlgItem(IDC_BUTTON_LOGIN2)->SetWindowText("ID2��¼");
		m_ctrlTreeChan.DeleteItem(m_hDevItem2);
		m_struDeviceInfo2.lLoginID = -1;
		m_bIsLogin2 = FALSE;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

/*************************************************
������:    	DoLogin1
��������:	���豸ע��
�������:
�������:
����ֵ:
**************************************************/
BOOL CCAMERADlg::DoLogin1()
{
	UpdateData(TRUE);
	CString DeviceIp1;											//IP1�ַ���
	BYTE nField1_0, nField1_1, nField1_2, nField1_3;			//IP1��ַ
	m_IP1.GetAddress(nField1_0, nField1_1, nField1_2, nField1_3);					//��nField��ֵ
	DeviceIp1.Format("%d.%d.%d.%d", nField1_0, nField1_1, nField1_2, nField1_3);	//����IP��ַ�ַ���
	
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
	CString DeviceIp2;											//IP2�ַ���
	BYTE nField2_0, nField2_1, nField2_2, nField2_3;			//IP2��ַ
	m_IP2.GetAddress(nField2_0, nField2_1, nField2_2, nField2_3);					//��nField��ֵ
	DeviceIp2.Format("%d.%d.%d.%d", nField2_0, nField2_1, nField2_2, nField2_3);	//����IP��ַ�ַ���
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
������:    	DoGetDeviceResoureCfg
��������:	��ȡ�豸��ͨ����Դ
�������:
�������:
����ֵ:
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
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = i + m_struDeviceInfo1.iStartChan;						//ͨ����
				m_struDeviceInfo1.struChanInfo[i].bEnable = TRUE;
			}
			else
			{
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = -1;														//ͨ����
				m_struDeviceInfo1.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "");
			}
		}
	}
	else		//֧��IP���룬9000�豸
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)		//ģ��ͨ��
		{
			if (i < m_struDeviceInfo1.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo1.iStartChan);
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = i + m_struDeviceInfo1.iStartChan;						//ͨ����
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
				m_struDeviceInfo1.struChanInfo[i].iChanIndex = -1;														//ͨ����
				m_struDeviceInfo1.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo1.struChanInfo[i].chChanName, "");
			}
		}
		for (i = 0; i < MAX_IP_CHANNEL; i++)			//����ͨ��
		{
			if (IpAccessCfg1.struStreamMode[i].uGetStream.struChanInfo.byEnable)		//ipͨ������
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
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = i + m_struDeviceInfo2.iStartChan;						//ͨ����
				m_struDeviceInfo2.struChanInfo[i].bEnable = TRUE;
			}
			else
			{
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = -1;														//ͨ����
				m_struDeviceInfo2.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "");
			}
		}
	}
	else		//֧��IP���룬9000�豸
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)		//ģ��ͨ��
		{
			if (i < m_struDeviceInfo2.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo2.iStartChan);
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = i + m_struDeviceInfo2.iStartChan;						//ͨ����
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
				m_struDeviceInfo2.struChanInfo[i].iChanIndex = -1;														//ͨ����
				m_struDeviceInfo2.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo2.struChanInfo[i].chChanName, "");
			}
		}
		for (i = 0; i < MAX_IP_CHANNEL; i++)			//����ͨ��
		{
			if (IpAccessCfg2.struStreamMode[i].uGetStream.struChanInfo.byEnable)		//ipͨ������
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
������:    	CreateDeviceTree
��������:	����ͨ����
�������:
�������:
����ֵ:
**************************************************/
void CCAMERADlg::CreateDeviceTree1()
{
	m_hDevItem1 = m_ctrlTreeChan.InsertItem("Dev");
	m_ctrlTreeChan.SetItemData(m_hDevItem1, DEVICETYPE * 1000);
	for (int i = 0; i<MAX_CHANNUM_V30; i++)
	{
		if (m_struDeviceInfo1.struChanInfo[i].bEnable)  //ͨ����Ч������ͨ����
		{
			HTREEITEM ChanItem = m_ctrlTreeChan.InsertItem(m_struDeviceInfo1.struChanInfo[i].chChanName, m_hDevItem1);
			m_ctrlTreeChan.SetItemData(ChanItem, CHANNELTYPE * 1000 + i);   //Data��Ӧͨ���������е�����
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
		if (m_struDeviceInfo2.struChanInfo[i].bEnable)  //ͨ����Ч������ͨ����
		{
			HTREEITEM ChanItem = m_ctrlTreeChan.InsertItem(m_struDeviceInfo1.struChanInfo[i].chChanName, m_hDevItem2);
			m_ctrlTreeChan.SetItemData(ChanItem, CHANNELTYPE * 1000 + i);   //Data��Ӧͨ���������е�����
		}
	}
	m_ctrlTreeChan.Expand(m_hDevItem2, TVE_EXPAND);
}

/*************************************************
������:    	StartPlay1
��������:	��ʼһ·����
�������:   ChanIndex-ͨ����
�������:
����ֵ:
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
		m_csErr.Format("���ų����������%d", err);
		MessageBox(m_csErr);
	}*/
	m_bIsPlaying1 = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText("ֹͣ����");
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
	m_csErr.Format("���ų����������%d", err);
	MessageBox(m_csErr);
	}*/
	m_bIsPlaying2 = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText("ֹͣ����");
}

/*************************************************
������:    	StopPlay1
��������:	ֹͣ����
�������:
�������:
����ֵ:
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
������:    	GetCurChanIndex
��������:	��ȡ��ǰѡ�е�ͨ����
�������:
�������:
����ֵ:		��ǰѡ�е�ͨ����
**************************************************/
int CCAMERADlg::GetCurChanIndex()
{
	return m_iCurChanIndex;
}

/*************************************************
������:    	OnButtonPlay
��������:	����ѡ��ͨ��
�������:
�������:
����ֵ:
**************************************************/
void CCAMERADlg::OnBnClickedButtonPlay1()
{
	if (m_iCurChanIndex == -1)
	{
		MessageBox("ѡ��һ��ͨ��");
		return;
	}
	if (!m_bIsPlaying1)
	{
		StartPlay1(m_iCurChanIndex);		//��ʼ����
		m_bIsPlaying1 = TRUE;
		GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText("ֹͣ����");
	}
	else
	{
		StopPlay1();						//ֹͣ����
		m_bIsPlaying1 = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText("����");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCAMERADlg::OnBnClickedButtonPlay2()
{
	if (m_iCurChanIndex == -1)
	{
		MessageBox("ѡ��һ��ͨ��");
		return;
	}
	if (!m_bIsPlaying2)
	{
		StartPlay2(m_iCurChanIndex);		//��ʼ����
		m_bIsPlaying2 = TRUE;
		GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText("ֹͣ����");
	}
	else
	{
		StopPlay2();						//ֹͣ����
		m_bIsPlaying2 = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText("����");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

/*************************************************
������:    	OnButtonCapture
��������:	BMPץͼ
�������:
�������:
����ֵ:
**************************************************/
void CCAMERADlg::OnBnClickedButtonCapture1()
{
	if (m_lPlayHandle1 == -1)
	{
		MessageBox("��ͨ��δ���ţ�");
		return;
	}
	UpdateData(TRUE);		//��ȡ��ʱ�̵ĸ�������

	char PicName1[256] = { 0 };
	//bmpץͼ
	CTime CurTime = CTime::GetCurrentTime();
	sprintf(PicName1, "%04d%02d%02d_%02d%02d%02d_ch%02d_L.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
		CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo1.struChanInfo[GetCurChanIndex()].iChanIndex);
	if (NET_DVR_CapturePicture(m_lPlayHandle1, PicName1))
	{
		MessageBox("ץͼ�ɹ���");
	}
	else
	{
		MessageBox("ץͼʧ�ܣ�");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCAMERADlg::OnBnClickedButtonCapture2()
{
	if (m_lPlayHandle2 == -1)
	{
		MessageBox("��ͨ��δ���ţ�");
		return;
	}
	UpdateData(TRUE);		//��ȡ��ʱ�̵ĸ�������

	char PicName2[256] = { 0 };
	//bmpץͼ
	CTime CurTime = CTime::GetCurrentTime();
	sprintf(PicName2, "%04d%02d%02d_%02d%02d%02d_ch%02d_R.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
		CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo2.struChanInfo[GetCurChanIndex()].iChanIndex);
	if (NET_DVR_CapturePicture(m_lPlayHandle2, PicName2))
	{
		MessageBox("ץͼ�ɹ���");
	}
	else
	{
		MessageBox("ץͼʧ�ܣ�");
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CCAMERADlg::OnBnClickedButtonCaptureall()
{
	if (m_lPlayHandle1 == -1 && m_lPlayHandle2 != -1)
	{
		MessageBox("��ͨ��δ���ţ�");
		return;
	}
	else if (m_lPlayHandle1 != -1 && m_lPlayHandle2 == -1)
	{
		MessageBox("��ͨ��δ���ţ�");
		return;
	}
	else if (m_lPlayHandle1 == -1 && m_lPlayHandle2 == -1)
	{
		MessageBox("����ͨ����δ���ţ�");
		return;
	}
	else
	{
		UpdateData(TRUE);		//��ȡ��ʱ�̵ĸ�������

		char PicName1[256] = { 0 };
		char PicName2[256] = { 0 };
		//bmpץͼ
		CTime CurTime = CTime::GetCurrentTime();
		sprintf(PicName1, "%04d%02d%02d_%02d%02d%02d_ch%02d_L.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo1.struChanInfo[GetCurChanIndex()].iChanIndex);
		sprintf(PicName2, "%04d%02d%02d_%02d%02d%02d_ch%02d_R.bmp", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo2.struChanInfo[GetCurChanIndex()].iChanIndex);
		if (NET_DVR_CapturePicture(m_lPlayHandle1, PicName1) && NET_DVR_CapturePicture(m_lPlayHandle2, PicName2))
		{
			/*string PATH = "D:/�о���/CAMERA/x64/Debug/";
			string Name1 = PATH + PicName1;
			string Name2 = PATH + PicName2;
			Mat Img1 = imread(Name1);
			Mat Img2 = imread(Name2);
			string Path = "C:/Users/le'v'o'n/Desktop/ʵ��ͼƬ/";
			string PicPath1 = Path + PicName1;
			string PicPath2 = Path + PicName2;
			imwrite(PicPath1, Img1);
			imwrite(PicPath2, Img2);*/
			MessageBox("ץͼ�ɹ���");
		}
		else
		{
			MessageBox("ץͼʧ�ܣ�");
		}
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

/*************************************************
������:    	OnSelchangedTreeChan
��������:	��ȡѡ�е�ͨ����
�������:
�������:
����ֵ:
**************************************************/
void CCAMERADlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
