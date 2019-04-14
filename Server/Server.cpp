
// Server.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CServerApp construction

CServerApp::CServerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CServerApp object

CServerApp theApp;


// CServerApp initialization

BOOL CServerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

bool CServerApp::SendFileToRemoteRecipient(CString fName)
{
	/***************************
	// listens for a connection from a remote client and uploads a file to it
	// the remote client must be running a counterpart GetFileFromRemoteSender function
	// Input: CString fName = name of local file which will be uploaded to remote client
	// Output: BOOL return value indicates success or failure of the upload
	***************************/

	// create socket and listen on pre-designated port

///	AfxSocketInit(NULL);	// make certain this is done somewhere in each thread (usually in InitInstance for main thread)
	CSocket sockSrvr;
	sockSrvr.Create(PORT);	// Creates our server socket
	sockSrvr.Listen();					// Start listening for the client at PORT
	CSocket sockConnection;
	sockSrvr.Accept(sockConnection);	// Use another CSocket to accept the connection

	// local variables used in file transfer (declared here to avoid "goto skips definition"-style compiler errors)

	BOOL bRet = TRUE;				// return value

	int fileLength, cbLeftToSend;	// used to monitor the progress of a sending operation

	BYTE* sendData = NULL;			// pointer to buffer for sending data (memory is allocated after sending file size)

	CFile sourceFile;
	CFileException fe;
	BOOL bFileIsOpen = FALSE;

	if (!(bFileIsOpen = sourceFile.Open(fName, CFile::modeRead | CFile::typeBinary, &fe)))
	{
		TCHAR strCause[256];
		fe.GetErrorMessage(strCause, 255);
		TRACE("SendFileToRemoteRecipient encountered an error while opening the local file\n"
			"\tFile name = %s\n\tCause = %s\n\tm_cause = %d\n\tm_IOsError = %d\n",
			fe.m_strFileName, strCause, fe.m_cause, fe.m_lOsError);

		/* you should handle the error here */

		bRet = FALSE;
		goto PreReturnCleanup;
	}


	// first send length of file

	fileLength = sourceFile.GetLength();
	fileLength = htonl(fileLength);

	cbLeftToSend = sizeof(fileLength);

	do
	{
		int cbBytesSent;
		BYTE* bp = (BYTE*)(&fileLength) + sizeof(fileLength) - cbLeftToSend;
		cbBytesSent = sockConnection.Send(bp, cbLeftToSend);

		// test for errors and get out if they occurred
		if (cbBytesSent == SOCKET_ERROR)
		{
			int iErr = ::GetLastError();
			TRACE("SendFileToRemoteRecipient returned a socket error while sending file length\n"
				"\tNumber of Bytes sent = %d\n"
				"\tGetLastError = %d\n", cbBytesSent, iErr);

			/* you should handle the error here */

			bRet = FALSE;
			goto PreReturnCleanup;
		}

		// data was successfully sent, so account for it with already-sent data
		cbLeftToSend -= cbBytesSent;
	} while (cbLeftToSend > 0);


	// now send the file's data

	sendData = new BYTE[SEND_BUFFER_SIZE];

	cbLeftToSend = sourceFile.GetLength();

	do
	{
		// read next chunk of SEND_BUFFER_SIZE bytes from file

		int sendThisTime, doneSoFar, buffOffset;

		sendThisTime = sourceFile.Read(sendData, SEND_BUFFER_SIZE);
		buffOffset = 0;

		do
		{
			doneSoFar = sockConnection.Send(sendData + buffOffset, sendThisTime);

			// test for errors and get out if they occurred
			if (doneSoFar == SOCKET_ERROR)
			{
				int iErr = ::GetLastError();
				TRACE("SendFileToRemoteRecipient returned a socket error while sending chunked file data\n"
					"\tNumber of Bytes sent = %d\n"
					"\tGetLastError = %d\n", doneSoFar, iErr);

				/* you should handle the error here */

				bRet = FALSE;
				goto PreReturnCleanup;
			}

			/***************************
			  un-comment this code and put a breakpoint here to prove to yourself that sockets can send fewer bytes than requested

						if ( doneSoFar != sendThisTime )
						{
							int ii = 0;
						}
			****************************/

			// data was successfully sent, so account for it with already-sent data

			buffOffset += doneSoFar;
			sendThisTime -= doneSoFar;
			cbLeftToSend -= doneSoFar;
		} while (sendThisTime > 0);

	} while (cbLeftToSend > 0);


PreReturnCleanup:		// labelled goto destination

	// free allocated memory
	// if we got here from a goto that skipped allocation, delete of NULL pointer
	// is permissible under C++ standard and is harmless
	delete[] sendData;

	if (bFileIsOpen)
		sourceFile.Close();		// only close file if it's open (open might have failed above)

	sockConnection.Close();

	return bRet;
}
