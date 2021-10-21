// THIS IS GITHUB

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include <iomanip>
#include <windows.h>
#include "NIIMAQ/niimaq.h"
#include "NIIMAQ/niimaq_errors.h"
#include "NIIMAQ/niimaq_errors_legacy.h"
#define _NIWIN 
// number of buffers used in grab
#define NUM_GRAB_BUFFERS 3

// Win32 CALLBACK
LRESULT CALLBACK ImaqSmplProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam);
// Error display function
void DisplayIMAQError(Int32 error);

int OnGrab(void);
DWORD ImaqThread(LPDWORD lpdwParam);
DWORD StopThread(LPDWORD lpdwParam);


// Thread objects
static HANDLE HThread;
static HANDLE HStopThread, HStopEvent;


// windows GUI globals
static HINSTANCE    hInst;
static HWND    	    ImaqSmplHwnd;
static HWND         HStop, HGrab, HQuit, HIntfName, HFrameRate;


// IMAQ globals
static SESSION_ID   Sid = 0;
static BUFLIST_ID   Bid = 0;
static INTERFACE_ID Iid = 0;
static void* ImaqBuffers[NUM_GRAB_BUFFERS];	//acquisition buffers
static Int8* CopyBuffer = NULL;   // copied acquisition buffer
static Int32        CanvasWidth = 512;  // width of the display area
static Int32        CanvasHeight = 384; // height of the display area
static Int32        CanvasTop = 10;     // top of the display area
static Int32        CanvasLeft = 10;    // left of the display area
static Int32        AcqWinWidth;
static Int32        AcqWinHeight;
static BOOL			StopGrab = FALSE;
static unsigned int plotFlag;

/*
static HWND hwnd;
static Int32 AcqWinWidth;
static Int32 AcqWinHeight;
static Int32 CanvasTop = 10;     // top of the display area
static Int32 CanvasLeft = 10;    // left of the display area
static Int32 CanvasWidth = 512;  // width of the display area
static Int32 CanvasHeight = 384; // height of the display area


static Int8* ImaqBuffers[NUM_RING_BUFFERS];   // acquisiton buffer
static BUFLIST_ID   Bid = 0;
*/

//unsigned int bufCmd, bufSize, bytesPerPixel;
// unsigned int bitsPerPixel, plotFlag;




// 2.  this is the window event callback function from line 15  , first paramater is window handler
//LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
//LRESULT CALLBACK ImaqSmplProc(HWND, UINT, WPARAM, LPARAM);



HWND textfield; // handles
// HWND button;

/* Make the class name into a global variable */
char szClassName[] = "IMAQ";

// this is the main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	CHAR        ImaqSmplClassName[] = "Imaq Sample";
	WNDCLASS  	ImaqSmplClass;  //Data structure for the windowclass
	MSG			msg;  // Messages to the application are saved here
	//HWND        hTemp;  // this is the handle for our window

	//HWND hwnd;	// this is the handle for our window
	//MSG messages; // Messages to the application are saved here
	//WNDCLASSEX wincl; //Data structure for the windowclass

	// window structure 
	/*
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = ImaqSmplProc; // this function is called by windows
	wincl.style = CS_DBLCLKS; // catch double-clicks
	wincl.cbSize = sizeof(WNDCLASSEX);

	// use default icon and mouse-pointer
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL; // no menu
	wincl.cbClsExtra = 0; // no extra bytes after the window class
	wincl.cbWndExtra = 0; // structure or the window instance

	// use window's default colour as background
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	*/


	// register main window
	hInst = hInstance;
	ImaqSmplClass.style = CS_HREDRAW | CS_VREDRAW;
	ImaqSmplClass.lpfnWndProc = (WNDPROC)ImaqSmplProc;
	ImaqSmplClass.cbClsExtra = 0;
	ImaqSmplClass.cbWndExtra = 0;
	ImaqSmplClass.hInstance = hInstance;

	// use default icon and mouse-pointer
	ImaqSmplClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	ImaqSmplClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	ImaqSmplClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND; // use window's default colour as background
	ImaqSmplClass.lpszMenuName = 0;
	ImaqSmplClass.lpszClassName = ImaqSmplClassName;

	if (!RegisterClass(&ImaqSmplClass))
		return (0);

	/*
	// register the window class, and if it fails, quit
	if (!RegisterClassEx(&wincl))
		return 0;
	*/

	/*
	// the class is registered, now create the program
	hwnd = CreateWindowEx(
		0,						// extended possibilities for variation
		szClassName,			// classname
		"IMAQ",			// title
		WS_OVERLAPPEDWINDOW,	// default window
		CW_USEDEFAULT,			// windows decides the position
		CW_USEDEFAULT,			// window location
		500,					// window width
		500,					// window height
		HWND_DESKTOP,			// window is a child-window to desktop
		NULL,					// no menu
		hThisInstance,			// program instance handler
		NULL					// no window creation data
	);
	*/


	// create main window
	ImaqSmplHwnd = CreateWindow(ImaqSmplClassName, "Low_Level_Grab", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 680, 440, HWND_DESKTOP, NULL, hInstance, NULL);
	// changed NULL to HWND_DESKTOP

	// try including STOP BUTTON according to example - CASE 3
	HStop = CreateWindow("Button", "Stop", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER,
		550, 112, 80, 40, ImaqSmplHwnd, (HMENU)3, hInstance, NULL);

	// try including QUIT button according to example - CASE 2
	HQuit = CreateWindow("Button", "Quit", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE,
		550, 152, 80, 40, ImaqSmplHwnd, (HMENU)2, hInstance, NULL);



	EnableWindow(HStop, TRUE); //changed false to true


	/*
	// make window visible on screen
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);  //maybe dont need, taken from IMAQ example
	*/


	// Display main window
	ShowWindow(ImaqSmplHwnd, SW_SHOW);
	UpdateWindow(ImaqSmplHwnd);

	/*
	// run the message loop. runs until GetMessage() returns 0
	while (GetMessage(&messages, NULL, 0, 0))
	{
		// translate virtual-key messages into char messages
		TranslateMessage(&messages);
		// send meesage to WindowProcedure
		DispatchMessage(&messages);
	}
	*/

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// program return value is 0: value that PostQuitMessage() gave 
	//return messages.wParam;

	return (int)(msg.wParam);

}

// 2. WM_CREATE to add buttons, WM_COMMAND to link button to an action
LRESULT CALLBACK ImaqSmplProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD            wmId;

	switch (message)
	{
	case WM_CREATE: // ADD ELEMENTS LIKE BUTTONS HERE
		textfield = CreateWindow("STATIC",
			"Testing of imgGrab",
			WS_VISIBLE | WS_CHILD,
			20, 20, 100, 30,
			hwnd, NULL, NULL, NULL, NULL);

		HGrab = CreateWindow("BUTTON",
			"imgGrab",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			30, 70, 200, 20,
			hwnd, (HMENU)1, NULL, NULL); // 3rd last parameter for actions

		break;

	case WM_COMMAND: // action command for button
		wmId = LOWORD(wParam);
		switch (LOWORD(wParam))
		{
		case 1:
			OnGrab();
			break;
		case 2:
			PostQuitMessage(0);
			break;
		case 3:
			SetEvent(HStopEvent);
			break;
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

// OnGrab function consists of IMAQ functions that will be called under CALLBACK
int OnGrab(void)
{
	//INTERFACE_ID interfaceID;

	//SESSION_ID sessionID;
	int 	        error, bufSize, bytesPerPixel, i, bufCmd;

	unsigned int	bitsPerPixel;
	DWORD			dwThreadId;


	// Create the event that needs to be signaled when we
// wish to stop the acquisition.
	HStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!HStopEvent)
		return 0;


	// Create the thread that is responsible for shutting
	// down the acquisition
	HStopThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StopThread, (LPDWORD)&HStopEvent, 0, &dwThreadId);
	if (!HStopThread)
		return 0;

	/*
	unsigned int bitsPerPixel, plotFlag;
	int i;
	*/

	// 1. imgInterfaceOpen (DONE)
	// 2. imgSessionOpen (DONE)
	// 3. imgGrabSetup (DONE)
	// 4. imgSessionStartAcquisition (DONE)
	// 5. imgGrab (DONE)
	// 6. User specific functions
	// 7. imgSessionStop Acquisition (DONE)
	// 8. imgClose (DONE)


	// Open an interface and a session
	imgInterfaceOpen("img0", &Iid);
	imgSessionOpen(Iid, &Sid);


	/*
	// 1. Open interface
	imgInterfaceOpen("img0", &interfaceID);
	// 2. Open session
	imgSessionOpen(interfaceID, &sessionID);
	*/

	// Let's check that the Acquisition window is not smaller than the Canvas
	imgGetAttribute(Sid, IMG_ATTR_ROI_WIDTH, &AcqWinWidth);
	imgGetAttribute(Sid, IMG_ATTR_ROI_HEIGHT, &AcqWinHeight);

	if (CanvasWidth < AcqWinWidth)
		AcqWinWidth = CanvasWidth;
	if (CanvasHeight < AcqWinHeight)
		AcqWinHeight = CanvasHeight;


	// get pixel depth of camera
	imgGetAttribute(Sid, IMG_ATTR_BITSPERPIXEL, &bitsPerPixel); // obtain pixel depth, then store in bitsPerPixel

	switch (bitsPerPixel)
	{
	case 10:
		plotFlag = IMGPLOT_MONO_10;
		break;
	case 12:
		plotFlag = IMGPLOT_MONO_12;
		break;
	case 14:
		plotFlag = IMGPLOT_MONO_14;
		break;
	case 16:
		plotFlag = IMGPLOT_MONO_16;
		break;
	case 24:
	case 32:
		// assumes that a 24 bits camera is a color camera.
		// in this mode, even if the camera is 24 bits the board returns 32 bits values
		plotFlag = IMGPLOT_COLOR_RGB32;
		break;
	default:
		plotFlag = IMGPLOT_MONO_8;
		break;
	}

	// Set the ROI to the size of the Canvas so that it will fit nicely
	imgSetAttribute2(Sid, IMG_ATTR_ROI_WIDTH, AcqWinWidth);
	imgSetAttribute2(Sid, IMG_ATTR_ROI_HEIGHT, AcqWinHeight);
	imgSetAttribute2(Sid, IMG_ATTR_ROWPIXELS, AcqWinWidth);

	// create a buffer list with one element
	imgCreateBufList(NUM_GRAB_BUFFERS, &Bid);

	// compute the size of the required buffer
	imgGetAttribute(Sid, IMG_ATTR_BYTESPERPIXEL, &bytesPerPixel);
	bufSize = AcqWinWidth * AcqWinHeight * bytesPerPixel;

	// alloc our own buffer for storing copy
	CopyBuffer = (Int8*)malloc(bufSize * sizeof(Int8));


	for (i = 0; i < NUM_GRAB_BUFFERS; i++)
	{	// imgCreateBuffer(SESSION_ID sid, uInt32 where, uInt32 bufferSize, 
		//void** bufPtrAddr);
		imgCreateBuffer(Sid, IMG_HOST_FRAME, bufSize, &ImaqBuffers[i]);
		imgSetBufferElement2(Bid, i, IMG_BUFF_ADDRESS, ImaqBuffers[i]);
		imgSetBufferElement2(Bid, i, IMG_BUFF_SIZE, bufSize);
		bufCmd = (i == (NUM_GRAB_BUFFERS - 1)) ? IMG_CMD_LOOP : IMG_CMD_NEXT;
		imgSetBufferElement2(Bid, i, IMG_BUFF_COMMAND, bufCmd);
	}

	// lock down the buffers contained in the buffer list
	imgMemLock(Bid);

	// configure the session to use this buffer list
	imgSessionConfigure(Sid, Bid);

	// added imgGrabSetup, example didnt have this
	imgGrabSetup(Sid, FALSE);

	// start the acquisition, asynchronous
	imgSessionAcquire(Sid, TRUE, NULL);

	StopGrab = FALSE;

	// Start the acquisition thread
	HThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ImaqThread, (LPDWORD*)&StopGrab, 0, &dwThreadId);
	if (HThread == NULL)
		return 0;

	EnableWindow(HGrab, TRUE);

	/*
	// 3. imgGrabSetup
	imgGrabSetup(sessionID, TRUE); //manually start acquisition with imgSessionStartAcquisition

	// 4. imgSessionStartAcquisition
	imgSessionStartAcquisition(sessionID);

	// 5. imgGrab (sessionID, buffer address, uint32 waitForNext)
	imgGrab(sessionID, NULL, 0);

	// 6a. imgSessionSaveBufferEx to save a buffer of a session to PNG
	//imgSessionSaveBufferEx(sessionID, NULL, reinterpret_cast<Int8*>(const_cast<char*>("test.png")));
	//imgSessionSaveBufferEx(sessionID, NULL, reinterpret_cast<Int8*>(0x00000010));
	imgSessionSaveBufferEx(sessionID, NULL, "test.png");

	// 6b. get attributes to get pixel depth of camera
	imgGetAttribute(sessionID, IMG_ATTR_BITSPERPIXEL, &bitsPerPixel); // obtain pixel depth, then store in bitsPerPixel

	// 6c.declare plotFlag according to pixel depth, bitsPerPixel
	switch (bitsPerPixel)
	{
	case 10:
		plotFlag = IMGPLOT_MONO_10;
		break;
	case 12:
		plotFlag = IMGPLOT_MONO_12;
		break;
	case 14:
		plotFlag = IMGPLOT_MONO_14;
		break;
	case 16:
		plotFlag = IMGPLOT_MONO_16;
		break;
	case 24:
	case 32:
		// assumes that a 24 bits camera is a color camera.
		// in this mode, even if the camera is 24 bits the board returns 32 bits values
		plotFlag = IMGPLOT_COLOR_RGB32;
		break;
	default:
		plotFlag = IMGPLOT_MONO_8;
		break;
	}

	// 6d. display using imgPlot (1. void* hwnd, 2. void* buffer, 3. uInt32 leftBufOffset, 4. uInt32 topBufOffset, 5. uInt32 xsize, 6. uInt32 ysize,
	//7. uInt32 xpos, 8. uInt32 ypos, 9. uInt32 flags)
	*/
	//plotFlag = IMGPLOT_MONO_16;

	//         1     2   3  4      5              6           7           8          9
	//imgPlot2(hwnd, NULL, 0, 0, AcqWinWidth, AcqWinHeight, CanvasLeft, CanvasTop, plotFlag); // ImaqSmplHwnd not established

	return 0;
}


DWORD ImaqThread(LPDWORD lpdwParam)
{
	static int nbFrame = 0, error = 0;
	static int currBufNum = 0;
	static uInt32 t1, t2;
	static uInt32 actualCopiedBuffer = 0, prevCopiedBuffer = 0;
	char    buffer[32];

	// Create a pointer to the stop boolean. This needs to be
	// volatile because the value can change at any time.
	BOOL* volatile stop = (BOOL*)lpdwParam;

	// the thread stop when StopRing goes to TRUE or there is an error
	while (!*stop && !error)
	{
		t2 = GetTickCount64();

		// Wait at least for the first valid frame
		imgGetAttribute(Sid, IMG_ATTR_LAST_VALID_FRAME, &currBufNum);
		currBufNum++;

		// Copy the last valid buffer
		imgSessionCopyBufferByNumber(Sid, currBufNum, CopyBuffer, IMG_OVERWRITE_GET_NEWEST, &actualCopiedBuffer, NULL);

		// Display it using imgPlot
		// Note that if you are using a board or camera with a bitdepth greater
		// that 8 bits, you need to set the flag parameter of imgPlot to match
		// the bit depth of the camera. See the "snap imgPlot" sample. 
		imgPlot2(ImaqSmplHwnd, CopyBuffer, 0, 0, AcqWinWidth, AcqWinHeight,
			CanvasLeft, CanvasTop, plotFlag);
		imgSessionSaveBufferEx(Sid, CopyBuffer, "test.png"); //now it works WOW

		// Calculate the number of frame per seconds every 10 frames
		if ((actualCopiedBuffer - prevCopiedBuffer) > 10)
		{
			sprintf_s(buffer, "%.2f", 1000.0 * (double)(actualCopiedBuffer - prevCopiedBuffer) / (double)(t2 - t1));
			SetWindowText(HFrameRate, buffer);
			t1 = t2;
			prevCopiedBuffer = actualCopiedBuffer;
		}

	}

	return 0;
}


// Waits for the stop event to occur, then stops the acquisition.
DWORD StopThread(LPDWORD lpdwParam) {
	DWORD dwResult;
	int i;

	// Get a handle to the stop event
	HANDLE event = *((HANDLE*)lpdwParam);

	// Wait for the done event to occur
	dwResult = WaitForSingleObject(event, INFINITE);
	if (dwResult != WAIT_FAILED) {
		CloseHandle(event);
		event = NULL;
	}

	// Stop the thread
	StopGrab = TRUE;

	// Wait for the thread to end and kill it otherwise
	dwResult = WaitForSingleObject(HThread, 2000);
	if (dwResult == WAIT_TIMEOUT)
		TerminateThread(HThread, 0);

	// stop the acquisition
	imgSessionAbort(Sid, NULL);

	// unlock the buffers in the buffer list
	if (Bid != 0)
		imgMemUnlock(Bid);

	// dispose of the buffers 
	for (i = 0; i < NUM_GRAB_BUFFERS; i++)
		if (ImaqBuffers[i] != NULL)
			imgDisposeBuffer(ImaqBuffers[i]);

	// close this buffer list
	if (Bid != 0)
		imgDisposeBufList(Bid, FALSE);

	// free our copy buffer
	if (CopyBuffer != NULL)
		free(CopyBuffer);

	// Close the interface and the session
	if (Sid != 0)
		imgClose(Sid, TRUE);
	if (Iid != 0)
		imgClose(Iid, TRUE);

	EnableWindow(HStop, FALSE);
	EnableWindow(HGrab, TRUE);
	EnableWindow(HQuit, TRUE);

	return 0;
}



// in case of error this function will display a dialog box
// with the error message
void DisplayIMAQError(Int32 error)
{
	static Int8 ErrorMessage[256];

	memset(ErrorMessage, 0x00, sizeof(ErrorMessage));

	// converts error code to a message
	imgShowError(error, ErrorMessage);

	MessageBox(ImaqSmplHwnd, ErrorMessage, "Imaq Sample", MB_OK);
}
