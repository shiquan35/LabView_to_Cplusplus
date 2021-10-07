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


static HWND ImaqSmplHwnd;
static Int32 AcqWinWidth;
static Int32 AcqWinHeight;
static Int32 CanvasTop = 10;     // top of the display area
static Int32 CanvasLeft = 10;    // left of the display area



// THIS IS LOCAL, NOT GITHUB
// run IMAQ in x86


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
