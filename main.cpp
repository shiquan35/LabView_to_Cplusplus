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

static HWND ImaqSmplHwnd;
static HWND         HSnap, HQuit, HIntfName;
static Int32 AcqWinWidth;
static Int32 AcqWinHeight;
static Int32 CanvasTop = 10;     // top of the display area
static Int32 CanvasLeft = 10;    // left of the display area


// OnGrab function consists of IMAQ functions that will be called under CALLBACK
int OnGrab(void)
{
	INTERFACE_ID interfaceID;

	SESSION_ID sessionID;

	unsigned int bitsPerPixel, plotFlag;

	// 1. imgInterfaceOpen (DONE)
	// 2. imgSessionOpen (DONE)
	// 3. imgGrabSetup (DONE)
	// 4. imgSessionStartAcquisition (DONE)
	// 5. imgGrab (DONE)
	// 6. User specific functions
	// 7. imgSessionStop Acquisition (DONE)
	// 8. imgClose (DONE)


// 1. Open interface
	imgInterfaceOpen("img0", &interfaceID);
	// 2. Open session
	imgSessionOpen(interfaceID, &sessionID);


	// 3. imgGrabSetup
	imgGrabSetup(sessionID, TRUE); //manually start acquisition with imgSessionStartAcquisition

	// 4. imgSessionStartAcquisition
	imgSessionStartAcquisition(sessionID);

	// 5. imgGrab (sessionID, buffer address, uint32 waitForNext)
	imgGrab(sessionID, NULL, TRUE);

	// 6a. imgSessionSaveBufferEx to save a buffer of a session to PNG
	imgSessionSaveBufferEx(sessionID, NULL, reinterpret_cast<Int8*>(const_cast<char*>("test.png")));



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

	// 6d. display using imgPlot (1. GUIHNDL window, 2. void* buffer, 3. uInt32 leftBufOffset, 4. uInt32 topBufOffset, 5. uInt32 xsize, 6. uInt32 ysize, 
	//7. uInt32 xpos, 8. uInt32 ypos, 9. uInt32 flags)


	//            1          2   3  4      5              6           7           8          9
	imgPlot2(ImaqSmplHwnd, NULL, 0, 0, AcqWinWidth, AcqWinHeight, CanvasLeft, CanvasTop, plotFlag); // ImaqSmplHwnd not established

	return 0;
}

// 2.  this is the window event callback function from line 15  , first paramater is window handler
//LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
LRESULT CALLBACK ImaqSmplProc(HWND, UINT, WPARAM, LPARAM);

HWND textfield, button; // handles

/* Make the class name into a global variable */
char szClassName[] = "IMAQ";

// this is the main function
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{

	HWND hwnd;	// this is the handle for our window
	MSG messages; // Messages to the application are saved here
	WNDCLASSEX wincl; //Data structure for the windowclass

	/* window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = ImaqSmplProc; // this function is called by windows
	wincl.style = CS_DBLCLKS; // catch double-clicks
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL; // no menu
	wincl.cbClsExtra = 0; // no extra bytes after the window class
	wincl.cbWndExtra = 0; // structure or the window instance
	/* use window's default colour as background */
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;


	/* register the window class, and if it fails, quit */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* the class is registered, now create the program*/
	hwnd = CreateWindowEx(
		0,						// extended possibilities for variation
		szClassName,			// classname
		"Weendows App",			// title
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

	/* make window visible on screen */
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);  //maybe dont need, taken from IMAQ example

	/* run the message loop. runs until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* translate virtual-key messages into char messages */
		TranslateMessage(&messages);
		/* send meesage to WindowProcedure*/
		DispatchMessage(&messages);
	}

	/* program return value is 0: value that PostQuitMessage() gave */
	return messages.wParam;
	//return (int)(msg.wParam);



	/* KEEP FOR REFERENCE - making of buttons
	// 3a.  Create the window
	ImaqSmplHwnd = CreateWindow(ImaqSmplClassName, "snap imgplot", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 680, 480, NULL, NULL, hInstance, NULL);

	// 3b. Create Snap button
	if (!(HSnap = CreateWindow("Button", "Snap", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER,
		550, 72, 80, 40, ImaqSmplHwnd, (HMENU)1, hInstance, NULL)))
		return(FALSE);

	// 3c. Create the interface name label
	if (!(hTemp = CreateWindow("Static", "Interface name", ES_LEFT | WS_CHILD | WS_VISIBLE,
		540, 14, 100, 20, ImaqSmplHwnd, (HMENU)-1, hInstance, NULL)))
		return(FALSE);

	// 3d. Create the interface name edit box
	if (!(HIntfName = CreateWindow("Edit", "img0", ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER,
		540, 34, 100, 20, ImaqSmplHwnd, (HMENU)-1, hInstance, NULL)))
		return(FALSE);

	// 3e. Create quit application button
	if (!(HQuit = CreateWindow("Button", "Quit", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE,
		550, 112, 80, 40, ImaqSmplHwnd, (HMENU)2, hInstance, NULL)))
		return(FALSE);
	*/


}

// 2. WM_CREATE to add buttons, WM_COMMAND to link button to an action
LRESULT CALLBACK ImaqSmplProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE: // ADD ELEMENTS LIKE BUTTONS HERE
		textfield = CreateWindow("STATIC",
			"Testing of imgGrab!",
			WS_VISIBLE | WS_CHILD,
			20, 20, 100, 30,
			hwnd, NULL, NULL, NULL, NULL);

		button = CreateWindow("BUTTON",
			"imgGrab",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			30, 70, 200, 20,
			hwnd, (HMENU)1, NULL, NULL); // 3rd last parameter for actions

		break;

	case WM_COMMAND: // action command for button

		switch (LOWORD(wParam))
		{
		case 1:
			OnGrab();
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

	/*
	WORD            wmId;

	switch (iMessage)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		switch (wmId)
		{

		case 2:
			PostQuitMessage(0);
			break;
		case 1:
			OnGrab();
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);

	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
		break;
	}
	return 0;
	*/
}


//////////////////////////////////////////////////////////////////////////////////////

// run IMAQ in x86

/*
int main()

{
	INTERFACE_ID interfaceID;

	SESSION_ID sessionID;

	unsigned int bitsPerPixel, plotFlag;

	if (imgInterfaceOpen("img0", &interfaceID) == IMG_ERR_GOOD) // "img0" is interfacename, opens an interface named img0

	{
		std::cout << "Interface is open" << std::endl;
		if (imgSessionOpen(interfaceID, &sessionID) == IMG_ERR_GOOD)

		{
			std::cout << "Session is open" << std::endl;

			// 30 sept 2021: try GRAB
			// 1. imgInterfaceOpen (DONE)
			// 2. imgSessionOpen (DONE)
			// 3. imgGrabSetup (DONE)
			// 4. imgSessionStartAcquisition (DONE)
			// 5. imgGrab (DONE)
			// 6. User specific functions
			// 7. imgSessionStop Acquisition (DONE)
			// 8. imgClose (DONE)

			//3. imgGrabSetup
			imgGrabSetup(sessionID, TRUE); //manually start acquisition with imgSessionStartAcquisition

			//4. imgSessionStartAcquisition
			imgSessionStartAcquisition(sessionID);

			//5. imgGrab (sessionID, buffer address, uint32 waitForNext)
			imgGrab(sessionID, NULL, TRUE);

			// imgSessionSaveBufferEx to save a buffer of a session to PNG
			imgSessionSaveBufferEx(sessionID, NULL, reinterpret_cast<Int8*>(const_cast<char*>("test.png")));
			std::cout << "Image saved" << std::endl;



			// get attributes to get pixel depth of camera
			imgGetAttribute(sessionID, IMG_ATTR_BITSPERPIXEL, &bitsPerPixel); // obtain pixel depth, then store in bitsPerPixel
			std::cout << "Get Attribute" << std::endl;
			// declare plotFlag according to pixel depth, bitsPerPixel
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
			std::cout << "plotFlag obtained" << std::endl;
			//6. functions or display using imgPlot (1. GUIHNDL window, 2. void* buffer, 3. uInt32 leftBufOffset, 4. uInt32 topBufOffset, 5. uInt32 xsize, 6. uInt32 ysize,
			// 7. uInt32 xpos, 8. uInt32 ypos, 9. uInt32 flags)


			//            1          2   3  4      5              6           7           8          9
			imgPlot2(ImaqSmplHwnd, NULL, 0, 0, AcqWinWidth, AcqWinHeight, CanvasLeft, CanvasTop, plotFlag);
			std::cout << "imgPlot2 done" << std::endl;

			//7. imgSessionStop Acquisition
			imgSessionStopAcquisition(sessionID);
			std::cout << "Stop acquisition" << std::endl;

			imgClose(sessionID, FALSE);	// closes the session using the imgClose function
			std::cout << "Session is closed" << std::endl;

		}

		imgClose(interfaceID, FALSE);	// closes the interface using the imgClose function
		std::cout << "Interface is closed" << std::endl;

	}
}

*/
//////////////////////////////////////////////////////////////////////////////////////



/*LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY: //user pressed cross button at top right corner
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}
*/