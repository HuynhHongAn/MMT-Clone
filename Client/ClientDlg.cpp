
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, userName(_T(""))
	, passWord(_T(""))
	, contentGroupChat(_T(""))
	, contentPrivateChat(_T(""))
	, StringIP(_T("127.0.0.1"))
	, onlineList(_T(""))
	, notifi(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CClientDlg::ShowAllComponent()
{
	GetDlgItem(IDC_ContentGroupChat2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_GroupSEND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_Invite)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_BUTTON_Leave2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_editMessage)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ContentPrivateChat2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_PrivateSEND)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ContentGroupChat2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_Attach)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_Logout)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LIST2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_G)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_P)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICP2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICP3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICCli)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICNoti)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_editUserName)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTONSignUp)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTONLOGIN)->ShowWindow(SW_HIDE);
	GetDlgItem(Static_UserName)->ShowWindow(SW_HIDE);
	GetDlgItem(Static_PassWord)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_editPassWord)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_StringIP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SIP)->ShowWindow(SW_HIDE);
}

void CClientDlg::HideAllComponent()
{
	// Chat Area

	GetDlgItem(IDC_ContentGroupChat2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_GroupSEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_Invite)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_Leave2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_editMessage)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ContentPrivateChat2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_PrivateSEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ContentGroupChat2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_Attach)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_Logout)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_G)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_P)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICP2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICP3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICCli)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICNoti)->ShowWindow(SW_HIDE);

	// Login Area
	GetDlgItem(IDC_editUserName)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTONSignUp)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTONLOGIN)->ShowWindow(SW_SHOW);
	GetDlgItem(Static_UserName)->ShowWindow(SW_SHOW);
	GetDlgItem(Static_PassWord)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_editPassWord)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_StringIP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SIP)->ShowWindow(SW_SHOW);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_editUserName, userName);
	DDX_Text(pDX, IDC_editPassWord, passWord);
	DDX_Text(pDX, IDC_ContentGroupChat2, contentGroupChat);
	DDX_Text(pDX, IDC_ContentPrivateChat2, contentPrivateChat);
	DDX_Text(pDX, IDC_StringIP, StringIP);
	DDX_Text(pDX, IDC_STATICCli, onlineList);
	DDX_Text(pDX, IDC_STATICNoti, notifi);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BUTTONLOGIN, &CClientDlg::OnBnClickedButtonlogin)
	ON_BN_CLICKED(IDC_BUTTONSignUp, &CClientDlg::OnBnClickedButtonsignup)
	ON_BN_CLICKED(IDC_BUTTON_GroupSEND, &CClientDlg::OnBnClickedButtonGroupsend)
	ON_BN_CLICKED(IDC_BUTTON_PrivateSEND, &CClientDlg::OnBnClickedButtonPrivatesend)
	ON_BN_CLICKED(IDC_BUTTON_Attach, &CClientDlg::OnBnClickedButtonAttach)
	ON_BN_CLICKED(IDC_BUTTON_Invite, &CClientDlg::OnBnClickedButtonInvite)
	ON_BN_CLICKED(IDC_BUTTON_Leave2, &CClientDlg::OnBnClickedButtonLeave2)
	ON_BN_CLICKED(IDC_BUTTON_Logout, &CClientDlg::OnBnClickedButtonLogout)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HideAllComponent();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

char * CClientDlg::ConvertToChar(const CString & s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CClientDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	p2 = src.Find(_T("\r\n"), p1 + 1);
	des[1] = src.Mid(p1 + 2, p2 - (p1 + 2));
}

void CClientDlg::mSend(CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CClientDlg::mRecv(CString & Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sClient, (char*)&buffLength, sizeof(int), 0);
	recv(sClient, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

LRESULT CClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		CString temp;
		if (mRecv(temp) < 0) {
			break;
		}
		UpdateData(FALSE);
		Split(temp, strResult);
		int flag1 = _ttoi(strResult[0]);

		if (flag1 == 0) {
			CString t = temp;
			notifi += strResult[1];
			notifi += "\r\n";

			int p = t.Find(strResult[1], 0) + strResult[1].GetLength() + 2;
			onlineList = t.Mid(p);
			UpdateData(FALSE);
			break;
		}
		int flag2 = _ttoi(strResult[1]);
		switch (flag1)
		{
		case 1: {
			if (flag2 == 1)
			{
				MessageBox(_T("Login Successfully!\r\n"));
				SetWindowText(userName);
				ShowAllComponent();
			}
			if (flag2 == 0)
			{
				MessageBox(_T("Username has been used\r\n"));
			}
			if (flag2 == 2)
			{
				MessageBox(_T("Wrong password\r\n"));
			}
			if (flag2 == 3)
			{
				MessageBox(_T("Wrong username\r\n"));
			}
			UpdateData(FALSE);
			break;
		}
		case 2: {
			if (flag2 == 0)
			{
				MessageBox(_T("Sign Up failed\r\nUsername has been used\r\n"));
			}
			if (flag2 == 1)
			{
				MessageBox(_T("Sign up Successfully!"));
				SetWindowText(userName);
			}
			if (flag2 == 2)
			{
				MessageBox(_T("Sign Up failed\r\nUsername has already existed\r\n"));

			}

			break;
		}
		case 3: { // Required logout
			MessageBox(_T("Logout Successfully!\n"));
			closesocket(sClient);
			HideAllComponent();
			break;
		}
		case 4: {
			contentGroupChat += strResult[1];
			contentGroupChat += "\r\n";
			//	GetDlgItem(IDC_ContentGroupChat2)->SetWindowTextW(strResult[1]);
			UpdateData(FALSE);
			break;
		}
		case 5: {


			if (flag2 == 0) {
				contentPrivateChat = "Your partner is not exist or online\r\n";
			}
			if (flag2 == 1) {
				contentPrivateChat = "Your partner is on another private chat\r\n";
			}
			if (flag2 == 2) { // 9\r\n :
				contentPrivateChat = "Ready for a new private chat\r\n";
				GetDlgItem(IDC_BUTTON_Invite)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_Leave2)->ShowWindow(SW_SHOW);

			}
			UpdateData(FALSE);
			break;
		}
		case 6: {
			contentPrivateChat += "The conversation with your partner ended\r\n";
			GetDlgItem(IDC_BUTTON_Invite)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT2)->SetWindowTextW(L"");

			GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_Leave2)->ShowWindow(SW_HIDE);

			contentPrivateChat = "";
			//UpdateData(FALSE);
			break;
		}
		case 9: {
			contentPrivateChat += strResult[1];
			contentPrivateChat += "\r\n";
			UpdateData(FALSE);

			break;
		}
		case 10:
		{
			contentPrivateChat += strResult[1];
			contentPrivateChat += "\r\n";
			UpdateData(FALSE);
			break;
		}
				break;
		}
		UpdateData(FALSE);
		break;
	}
	case FD_CLOSE:
	{
		closesocket(sClient);
		if (_ttoi(strResult[0]) != 3) {
			MessageBox(_T("Server close the connection\r\n"));
		}
		
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}

std::string CClientDlg::toString(CString s)
{
	return std::string();
}

int CClientDlg::sendFileToServer(CString fileDirect)
{
	//FILE* fp = fopen(toString(fileDirect).c_str(), "rb");
	FILE *fp = fopen("aow_drv.log", "rb");
	if (fp == NULL) {
		perror("Can't find file");
		return 0;
	}

	// command send header
	Command = "10\r\n";
	Command += fileDirect;
	Command += "\r\n";
	CString number;
	number.Format(L"%d", countFile++);
	Command += number;
	Command += "\r\nNone\r\n";

	mSend(Command);

	// command send data
	Command = "10\r\n";
	Command += number;
	Command += "\r\n";
	
	char sendbuffer[100];
	int b;
	while ((b = fread(sendbuffer, 1, sizeof(sendbuffer), fp)) > 0) {
		CString data = Command + CString(sendbuffer);
		mSend(Command + CString(sendbuffer));
	}

	return 1;
}

//Catch the event when click  Button Login in first dialog
void CClientDlg::OnBnClickedButtonlogin()
{
	UpdateData(TRUE);

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}
	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);
	//CString IP("127.0.0.1");
	char* cIP = ConvertToChar(StringIP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(StringIP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Cannot connect to server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
			host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		MessageBox(_T("Fail to connect to server."), _T("ERROR"), 0);
		return;
	}


	CString user, password;
	GetDlgItemText(IDC_editUserName, user);
	GetDlgItemText(IDC_editPassWord, password);
	if (!user.GetLength() || !password.GetLength())
	{
		MessageBox(_T("Username or password cannot be blank"));
		return;
	}

	Command = "1\r\n";
	Command += user + '/' + password;
	Command += "\r\n";

	mSend(Command);


	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	UpdateData(FALSE);
}

// Catch the event when click Button SignUp in first dialog
void CClientDlg::OnBnClickedButtonsignup()
{
	UpdateData(TRUE);

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}
	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);
	//CString IP("127.0.0.1");
	char* cIP = ConvertToChar(StringIP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(StringIP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Cannot connect to server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
			host->h_length);
		return;
	}

	// Phan ket noi dialog voi server
	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));

	if (err == SOCKET_ERROR) {
		MessageBox(_T("Connect failed"), _T("ERROR"), 0);
		return;
	}


	CString user, password;
	GetDlgItemText(IDC_editUserName, user);
	GetDlgItemText(IDC_editPassWord, password);
	if (!user.GetLength() || !password.GetLength())
	{
		MessageBox(_T("Username or password cannot be blank"));
		return;
	}


	Command = "2\r\n";
	Command += user + '/' + password;
	Command += "\r\n";

	mSend(Command);



	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}


void CClientDlg::OnBnClickedButtonGroupsend()
{
	CString msgTemp;
	GetDlgItemText(IDC_EDIT1, msgTemp);
	Command = "4\r\n";
	Command += msgTemp;
	Command += "\r\n";
	mSend(Command);
	contentGroupChat += "You: ";
	contentGroupChat += msgTemp;
	contentGroupChat += "\r\n";
	SetDlgItemText(IDC_EDIT1, L"");
	UpdateData(FALSE);

}


void CClientDlg::OnBnClickedButtonPrivatesend()
{

	CString msgTemp;
	GetDlgItemText(IDC_editMessage, msgTemp);
	Command = "9\r\n";
	Command += msgTemp;
	Command += "\r\n";
	mSend(Command);
	contentPrivateChat += "You: ";
	contentPrivateChat += msgTemp;
	contentPrivateChat += "\r\n";
	SetDlgItemText(IDC_editMessage, L"");
	UpdateData(FALSE);
}


void CClientDlg::OnBnClickedButtonAttach()
{
	// TODO: Add your attach file code here
	CFileDialog dlgFile(TRUE);
	/*const int c_cMaxFiles = 100;
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(c_cbBuffSize);
	dlgFile.GetOFN().nMaxFile = c_cbBuffSize;
	dlgFile.DoModal();
	fileName.ReleaseBuffer();*/

	if (dlgFile.DoModal() == IDOK)
	{
		CString pathName = dlgFile.GetPathName();
		CString fileName = dlgFile.GetFileName();
		sendFileToServer(pathName);
		// Do something with 'PathName'
		Command = "10\r\n";
		Command += fileName;
		Command += "\r\n";
		mSend(Command);
		contentPrivateChat += "You sent a file: ";
		contentPrivateChat += pathName;
		contentPrivateChat += "\r\n";
		SetDlgItemText(IDC_editMessage, L"");
		UpdateData(FALSE);

		
	}
	

	//GetDlgItemText(IDC_editMessage, fileName);
	

}


void CClientDlg::OnBnClickedButtonInvite()
{
	// TODO: Add your control notification handler code here
	CString userPartner;
	GetDlgItemText(IDC_EDIT2, userPartner);
	Command = "5\r\n";
	Command += userPartner;
	Command += "\r\n";
	mSend(Command);
	UpdateData(FALSE);
}


void CClientDlg::OnBnClickedButtonLeave2()
{
	Command = "6\r\n0\r\n";
	mSend(Command);
}


void CClientDlg::OnBnClickedButtonLogout()
{
	Command = "3\r\n1\r\n";
	mSend(Command);
	HideAllComponent();
	SetDlgItemText(IDC_EDIT1, L"");
	SetDlgItemText(IDC_editMessage, L"");
	SetDlgItemText(IDC_EDIT2, L"");
	contentGroupChat = "";
	contentPrivateChat = "";
	UpdateData(FALSE);
}
// "9\r\n\Msg\r\n