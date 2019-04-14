
// ClientDlg.h : header file
//

#pragma once
#define PORT 25000
#define WM_SOCKET WM_USER+2
#define RECV_BUFFER_SIZE 4096

#include <string>
// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor
	//void HideAllComponent();
	void ShowAllComponent();
	void HideAllComponent();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:

	char* ConvertToChar(const CString &s);
	void Split(CString src, CString des[2]);
	void mSend(CString Command);
	int mRecv(CString &Command);
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);

	SOCKET sClient;
	CString Command;

	sockaddr_in servAdd;
	int	buffLength;
	CString strResult[2];
	CString userOnline;

// upload - download file
	std::string toString(CString s);
	int sendFileToServer(CString fileDirect);
	int countFile = 0;

public:
	afx_msg void OnBnClickedButtonlogin();
	afx_msg void OnBnClickedButtonsignup();
	CString userName;
	CString passWord;
	afx_msg void OnBnClickedButtonGroupsend();
	afx_msg void OnBnClickedButtonPrivatesend();
	afx_msg void OnBnClickedButtonAttach();
	afx_msg void OnBnClickedButtonInvite();
	afx_msg void OnBnClickedButtonLeave2();
	afx_msg void OnBnClickedButtonLogout();
	CString contentGroupChat;
	CString contentPrivateChat;
	CString StringIP;
	CString onlineList;
	CString notifi;
};
