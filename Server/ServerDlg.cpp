
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg dialog



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
	, m_msgString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_log, m_msgString);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(IDC_BUTTON_Start, &CServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_Stop, &CServerDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("Admin"));
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Private methods

std::string CServerDlg::convertFromCString(CString p)
{
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(p);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);
	return strStd;
}

CString CServerDlg::convertFromString(std::string p)
{
	return CString(p.c_str());
}
/*
	Each line is an account, format is: username/password
*/
// If username is exist, return a vector CString include username and its password,
// Else return a vector CString has no element
vector<CString> CServerDlg::findAccount(CString username)
{
	vector<CString> res;
	fstream data;
	data.open(".\\Data.txt", ios::in);
	string user = convertFromCString(username);
	while (!data.eof()) {
		string line;
		getline(data, line);
		if (line.find(user) != string::npos)
		{
			int p = line.find("/");
			res.push_back(convertFromString(line.substr(0, p))); // Add CString username to result
			res.push_back(convertFromString(line.substr(p + 1))); // Add CString password to result
			break;
		}
	}
	data.close();
	return res;
}

// A CString account : username/password
void CServerDlg::addAccount(CString account)
{
	fstream data;
	data.open(".\\Data.txt", ios::app);
	data << convertFromCString(account);
	data << endl;
	data.close();
}


LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{

		pSock[number_Socket].sockClient = accept(wParam, NULL, NULL);
		GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
		break;
	}
	case FD_READ:
	{
		int post = -1, dpos = -1;

		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		CString temp;
		if (mRecv(wParam, temp) < 0)
			break;
		int nArg = FullSplit(temp, strResult, L"\r\n");
		int flag = _ttoi(strResult[0]);
		if (flag == 1 || flag == 2)
		{

			int local = strResult[1].Find('/');
			CString CtempUser = strResult[1].Mid(0, local);
			CString CtempPass = strResult[1].Mid(local + 1);
			if (flag == 1) //Login required : "1\r\n_Username/Password_\r\n"
			{			// Return : "1\r\n_number_\r\nUserName\r\n" : 1 : success |  0, 2, 3 : failed
				int t = 0;
				if (number_Socket > 0)
				{
					for (int i = 0; i < number_Socket; i++)
					{
						if (strcmp(ConvertToChar(CtempUser), pSock[i].Name) == 0) //Trung ten user da online
						{
							t = 1;
							break;
						}
					}
				}

				if (t == 0)
				{
					vector<CString> Account = findAccount(CtempUser);
					if (Account.size() > 0) {
						if (Account[1] == CtempPass) {
							strcpy(pSock[number_Socket].Name, ConvertToChar(CtempUser));

							Command = _T("1\r\n1\r\n"); // Login success
							mSend(wParam, Command);

							// Update List Online
							CListBox * listbox = (CListBox *)GetDlgItem(IDC_LIST1);
							listbox->AddString(CtempUser);
							convAr.push_back(-1);

							//  Send to all client except the sender client
							Command = _T("0\r\n ");
							Command += CtempUser;
							Command += _T(" login\r\n"); // 1 mean login
							for (int i = 0; i < listbox->GetCount(); i++)
							{
								CString te;
								listbox->GetText(i, te);
								Command += te; 
								Command += "\r\n";
							}
							mSendToAllExcept(wParam, Command);
							mSend(wParam, Command);
							//Update server's log
							m_msgString += CtempUser + _T(" login\r\n");
							UpdateData(FALSE);

							number_Socket++;

						}
						else { // Wrong password => login failed
							Command = _T("1\r\n2\r\n");
							mSend(wParam, Command);
							UpdateData(FALSE);

						}

					}
					else { // Wrong username=> login failed

						Command = _T("1\r\n3\r\n");
						mSend(wParam, Command);
						UpdateData(FALSE);
					}
				}
				else // account has been used => login failed
				{
					Command = _T("1\r\n0\r\n");

					mSend(wParam, Command);
					UpdateData(FALSE);

				}
				UpdateData(FALSE);
				break;
			}
			else	// Sign Up required: "2\r\n_Username/Password_\r\n"
			{		// Return : "2\r\n_number_\r\n" : 1 success, 0:2 failed
				MessageBox(temp);
				int t = 0;
				if (number_Socket > 0)
				{
					for (int i = 0; i < number_Socket; i++)
					{
						if (strcmp(ConvertToChar(CtempUser), pSock[i].Name) == 0) //Trung ten user da online
						{
							t = 1;
							break;
						}
					}
				}

				if (t == 0)
				{
					vector<CString> Account = findAccount(CtempUser);
					if (Account.size() == 0) {
						Command = _T("2\r\n1\r\n"); // Sign up  success

						//Update server's log
						m_msgString += CtempUser + _T(" sign up\r\n");
						UpdateData(FALSE);

						// Save account
						CString newAccount = CtempUser + '/' + CtempPass;
						addAccount(newAccount);
						mSend(wParam, Command);

					}
					else { // username has already existed
						Command = _T("2\r\n2\r\n");
						mSend(wParam, Command);
					}
				}
				else // account has been used => sign up failed
				{
					Command = _T("2\r\n0\r\n");
					mSend(wParam, Command);
				}
				UpdateData(FALSE);
				break;
			}
		}
		else switch (flag)
		{
		case 3: // Logout Required "3\r\n1\r\n" , always send to all client
		{
		
			if (convAr[post] != -1) {

				Command = "6\r\n2\r\n"; // Notification leave private
				mSend(pSock[convAr[post]].sockClient, Command);

				convAr[convAr[post]] = -1;
				convAr[post] = -1;
			}
			// Todo : cap nhat vector convAr;
			for (int i = post + 1; i < number_Socket; i++)
			{
				convAr[i - 1] = convAr[i];
			}
			for (int i = 0; i < number_Socket; i++)
			{
				if (convAr[i] > post)
				{
					convAr[i]--;
				}
			}

			///
			Command = _T("3\r\n0\r\n");
			mSend(wParam, Command);



			m_msgString += pSock[post].Name;
			m_msgString += " logout\r\n";
			UpdateData(FALSE);

			closesocket(wParam);
			CListBox * listbox = (CListBox *)GetDlgItem(IDC_LIST1);
			listbox->DeleteString(listbox->FindString(-1, CString(pSock[post].Name)));

			Command = _T("0\r\n ");
			Command += pSock[post].Name;
			Command += _T(" logout\r\n");
//			CListBox * listbox = (CListBox *)GetDlgItem(IDC_LIST1);
			for (int i = 0; i < listbox->GetCount(); i++)
			{
				CString te;
				listbox->GetText(i, te);
				Command += te;
				Command += "\r\n";
			}
			mSendToAllExcept(wParam, Command);
			mSend(wParam, Command);

			for (int j = post; j < number_Socket - 1; j++)
			{
				pSock[j].sockClient = pSock[j + 1].sockClient;
				strcpy(pSock[j].Name, pSock[j + 1].Name);
			}
			number_Socket--;

			UpdateData(FALSE);

			break;
		}
		case 4: // Required of Group Chat "4\r\n_contentMsg_\r\n"
		{
			// Update log
			m_msgString += pSock[post].Name;
			m_msgString += " sent message to Group Chat: ";
			m_msgString += strResult[1];
			m_msgString += "\r\n";
			UpdateData(FALSE);

			// Send result to Client
			Command = "4\r\n";
			Command += pSock[post].Name;
			Command += ": ";
			Command += strResult[1];
			Command += "\r\n";

			mSendToAllExcept(wParam, Command);

			break;

		}
		case 5: // Required of invite private Chat : "5\r\nUsernamePartner\r\n
		{      // 
			// Find index of this client and  Partner required
			int post2 = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].Name == strResult[1])
				{
					if (i < number_Socket)
						post2 = i;
				}
			}
		

			if (post == post2) break;
			if (post2 < 0) {
				Command = "5\r\n0\r\n"; // 0 mean username partner is not  exist
				mSend(wParam, Command);
			}
			else {
				if (convAr[post2] != -1) {
					Command = "5\r\n1\r\n"; // 1 mean partner is in another private chat
					mSend(wParam, Command);
				}
				else {
					Command = "5\r\n2\r\n"; // 2 mean partner is ready to start private chat
					mSend(wParam, Command);
					mSend(pSock[post2].sockClient, Command);
					convAr[post2] = post;
					convAr[post] = post2;

					//Update log
					m_msgString += pSock[post].Name;
					m_msgString += " started chat private with ";
					m_msgString += pSock[post2].Name;
					m_msgString += "\r\n";

					UpdateData(FALSE);

					//pSock[post].isBusy = TRUE;
					//pSock[post2].isBusy = TRUE;
				}
			}
			break;
		}
		case 6: // Required of leave private Chat : "6\r\nUsernamePartner\r\n
		{      // 
			// Update log
			if (convAr[post] == -1) break;
			m_msgString += pSock[post].Name;
			m_msgString += " stopped chat private with ";
			m_msgString += pSock[convAr[post]].Name;
			m_msgString += "\r\n";
			UpdateData(FALSE);

			Command = "6\r\n2\r\n"; // Notification leave private
			mSend(wParam, Command);
			mSend(pSock[convAr[post]].sockClient, Command);
			// Update vector convAr;
			convAr[convAr[post]] = -1;
			convAr[post] = -1;
			break;

		}
		case 7: // Required of Send private Chat : "7\r\nUsernamePartner...."
		{      // 

			break;
		}
		case 8: // Required of recieve private Chat : "7\r\nUsernamePartner...."
		{      // 
			break;
		}
		case 9: // Required of message private Chat : "7\r\nUsernamePartner-Content'\r\n"
		{      // 
			Command = "9\r\n";
			Command += pSock[post].Name;
			Command += ": ";
			Command += strResult[1];
			Command += "\r\n";
			mSend(pSock[convAr[post]].sockClient, Command);
			//mSend(pSock[post].sockClient, Command);

			break;
		}
		case 10: //Required of recieve file : "10\r\nfileName\r\n"
		{
			/*Command = "10\r\n";
			Command += pSock[post].Name;
			Command += " sent you a file: ";
			Command += strResult[1];
			Command += "\r\n";
			mSend(pSock[convAr[post]].sockClient, Command);
			break;*/
			if (nArg == 4) {		// set up
				CString fullFileName = strResult[1];
				CString fileName = fullFileName.Mid(fullFileName.ReverseFind('\\') + 1);

				Command = "10\r\n";
				Command += pSock[post].Name;
				Command += " send file ";
				CString number;
				number.Format(L"%d-", fileMapping.size());
				Command += number + fileName;
				Command += "\r\n";

				FILE* fp = fopen(toString(number + fileName).c_str(), "wb");
				fileMapping[make_pair(wParam, _ttoi(strResult[2]))] = fp;
				mSendToAllExcept(wParam, Command);
			}

			if (nArg == 3) {
				int code = _ttoi(strResult[1]);
				FILE *fp = fileMapping[make_pair(wParam, code)];

				CString data = strResult[2];
				int Len = data.GetLength();
				Len += Len;
				PBYTE sendBuff = new BYTE[1000];
				memset(sendBuff, 0, 1000);
				memcpy(sendBuff, (PBYTE)(LPCTSTR)data, Len);
				fwrite((char*)sendBuff, 1, Len, fp);
			}

			break;
		}

		}
		
		break;
	}
	case FD_CLOSE:
	{
		UpdateData();
		int post = -1;
		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}
		// Stop private chat if exist
		if (number_Socket > 1 && convAr[post] != -1) {
			m_msgString += pSock[post].Name;
			m_msgString += " stopped chat private with ";
			m_msgString += pSock[convAr[post]].Name;
			m_msgString += "\r\n";
			UpdateData(FALSE);

			Command = "6\r\n2\r\n"; // Notification leave private
			mSend(wParam, Command);
			mSend(pSock[convAr[post]].sockClient, Command);
			// Update vector convAr;
			convAr[convAr[post]] = -1;
			convAr[post] = -1;
			for (int i = post; i < number_Socket - 1; i++)
			{
				convAr[i] = convAr[i + 1];
			}
			for (int i = 0; i < number_Socket; i++)
			{
				if (convAr[i] > post)
				{
					convAr[i]--;
				}
			}
		}



		// Update log 
		m_msgString += pSock[post].Name;
		m_msgString += " logout\r\n";
		closesocket(wParam);

		CListBox * listbox = (CListBox *)GetDlgItem(IDC_LIST1);
		listbox->DeleteString(listbox->FindString(-1, CString(pSock[post].Name)));

		Command = _T("0\r\n ");
		Command += pSock[post].Name;
		Command += _T(" logout\r\n");
		//			CListBox * listbox = (CListBox *)GetDlgItem(IDC_LIST1);
		for (int i = 0; i < listbox->GetCount(); i++)
		{
			CString te;
			listbox->GetText(i, te);
			Command += te;
			Command += "\r\n";
		}
		mSendToAllExcept(wParam, Command);
		mSend(wParam, Command);

		for (int j = post; j < number_Socket - 1; j++)
		{
			pSock[j].sockClient = pSock[j + 1].sockClient;
			strcpy(pSock[j].Name, pSock[j + 1].Name);
		}

		number_Socket--;
		UpdateData(FALSE);
		break;
	}
	}

	return 0;
}

void CServerDlg::mSendToAllExcept(SOCKET sk, CString Command)
{

	for (int i = 0; i < number_Socket; i++)
	{
		if (pSock[i].sockClient != sk)
			mSend(pSock[i].sockClient, Command);
	}
}


void CServerDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	p2 = src.Find(_T("\r\n"), p1 + 1);
	des[1] = src.Mid(p1 + 2, p2 - (p1 + 2));

}

int CServerDlg::FullSplit(CString src, CString des[], CString token)
{
	int nTokenPos = 0;
	int nToken = 0;
	CString strToken = src.Tokenize(token, nTokenPos);
	while (!strToken.IsEmpty())
	{
		des[nToken++] = strToken;
		strToken = src.Tokenize(token, nTokenPos);
	}
	return nToken;
}

std::string CServerDlg::toString(CString s)
{
	CT2CA tmp(s);
	return std::string(tmp);
}

char* CServerDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CServerDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CServerDlg::mRecv(SOCKET sk, CString &Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sk, (char*)&buffLength, sizeof(int), 0);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

// Catch the event when click Button Start
void CServerDlg::OnBnClickedButtonStart()
{

	UpdateData();
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(PORT);
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockServer, (SOCKADDR*)&serverAdd, sizeof(serverAdd));
	listen(sockServer, 5);
	int err = WSAAsyncSelect(sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cannot call WSAAsyncSelect");
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);

	number_Socket = 0;
	convAr.resize(number_Socket);
	pSock = new SockName[200];
	UpdateData(FALSE);

}

// Catch the event when click Button Stop
void CServerDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
