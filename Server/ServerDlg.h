
// ServerDlg.h : header file
//

#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

#define PORT 25000
#define WM_SOCKET WM_USER+1
#define SEND_BUFFER_SIZE 4096

// CServerDlg dialog
class CServerDlg : public CDialogEx
{
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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
private:
	//CStatic log;
	//CStatic listOnline;
	//	CString m_msgString;
	SOCKET sockServer, sockClient, flag, sclient;
	struct sockaddr_in serverAdd;
	int msgType;
	int buffLength, t, lenguser, flagsend, kq, count_sock;
	int number_Socket;
	int first_send;
	struct SockName
	{

		SOCKET sockClient;
		char Name[200];
	};
	vector<int> convAr;
	SockName *pSock;
	CString strResult[100];
	CString Command;
 // Private methods
	std::string convertFromCString(CString);
	CString convertFromString(std::string);
	vector<CString> findAccount(CString username);
	void addAccount(CString account);


	int FullSplit(CString src, CString des[], CString token);
	std::string toString(CString s);
	std::map<pair<int, int>, FILE*> fileMapping;
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	char* ConvertToChar(const CString &s);
	void Split(CString src, CString des[2]);
	void mSend(SOCKET sk, CString message);
	int mRecv(SOCKET sk, CString &message);
	void CServerDlg::mSendToAllExcept(SOCKET sk, CString Command);

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();


	CString m_msgString;

};
