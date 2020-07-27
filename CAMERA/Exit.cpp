// Exit.cpp : 实现文件
//

#include "stdafx.h"
#include "CAMERA.h"
#include "Exit.h"
#include "afxdialogex.h"


// CExit 对话框

IMPLEMENT_DYNAMIC(CExit, CDialogEx)

CExit::CExit(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXIT_DIALOG, pParent)
{

}

CExit::~CExit()
{
}

void CExit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExit, CDialogEx)
END_MESSAGE_MAP()


// CExit 消息处理程序
