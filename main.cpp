#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include <iomanip>

#include "NIIMAQ/niimaq.h"
#include "NIIMAQ/niimaq_errors.h"
#include "NIIMAQ/niimaq_errors_legacy.h"

#include "fftw/fftw3.h"

#include <pbPlots.hpp>
#include <supportLib.hpp>
#define N 16

/* IMAQ runs in x86
   fftw runs in x64(?)
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* REOPEN LINE 49
int main()
{
	
	// structure to hold the image
	RGBABitmapImageReference* imageRef = CreateRGBABitmapImageReference();
	RGBABitmapImageReference* imageRef2 = CreateRGBABitmapImageReference();
	// imageRef3 = CreateRGBABitmapImageReference();
	std::vector <double> a; // x coordinate of FFT
	std::vector <double> b; // y coordinate of FFT
	std::vector <double> c; // x coordinate of signal
	std::vector <double> d; // y coordinate of signal
	


	
	// CORRECT COSINE WAVE 16 SEPT
	//std::cout << 'x' << "\t";
	//std::cout << 'y' << "\t" << std::endl;
	
	for (int f = 0; f < 361; ++f)
	{
		c.push_back(f); // c: 0-360
		d.push_back(cos(f * M_PI / 180)); // d: -1 to 1

		//std::cout << c[f] << "\t";
		//std::cout << d[f] << std::endl;
	}
	*/
	/*
	std::cout << "Cosine complete" << std::endl;
	DrawScatterPlot(imageRef, 600, 400, &c, &d); // width, height

	//convert plot to PNG file
	vector<double>* pngdata = ConvertToPNG(imageRef->image);
	WriteToFile(pngdata, "cosine3.png");
	std::cout << "Plot Created" << std::endl;
	DeleteImage(imageRef->image);
	*/

	/* REOPEN LINE 71
	// beginning of FFTW
	fftw_complex in[N], out[N], in2[N];
	fftw_plan p, q;
	int i;

	// prepare a cosine wave
	for (i = 0; i < N; i++) {
		in[i][0] = cos(3 * 2 * M_PI * i / N);
		in[i][1] = 0;
	}
	*/


	/*
	// plot cosine
	for (int j = 0; j < 15; ++j)
	{
		c.push_back(j); // c: 0-360
		d.push_back(in[j][0]); // d: -1 to 1

	}

	DrawScatterPlot(imageRef, 600, 400, &c, &d); // width, height
	//convert plot to PNG file
	vector<double>* pngdata2 = ConvertToPNG(imageRef2->image);
	WriteToFile(pngdata2, "cosine4.png");
	std::cout << "Plot Created" << std::endl;
	DeleteImage(imageRef2->image);
	*/

	/* REOPEN LINE 145
	// forward Fourier transform, save the result in 'out'
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	for (i = 0; i < N; i++)
	{
		printf("freq: %3d %+9.5f %+9.5f I\n", i, out[i][0], out[i][1]);
	}
		
	
	fftw_destroy_plan(p);

	std::cout << "out[3][0] is: " << out[3][0] << std::endl;

	// plot FFT
	std::cout << "test" << std::endl;
	for (int f = 0; f < 16; f++)
	{
		a.push_back(f);
		b.push_back(out[f][0]);

		std::cout << a[f] << "\t";
		std::cout << b[f] << std::endl;
		
	}
	std::cout << "test end" << std::endl;

	//convert FFT plot to PNG file BARPLOT
	imageRef->image = DrawBarPlot(800.0, 600.0, &b);
	vector<double>* pngdata = ConvertToPNG(imageRef->image);
	WriteToFile(pngdata, "FFTBARcosine.png");
	std::cout << "Bar Plot Created" << std::endl;
	DeleteImage(imageRef->image);

	
	// backward Fourier transform, save the result in 'in2'
	printf("\nInverse transform:\n");
	q = fftw_plan_dft_1d(N, out, in2, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(q);
	// normalize
	for (i = 0; i < N; i++) {
		in2[i][0] *= 1. / N;
		in2[i][1] *= 1. / N;
	}
	for (i = 0; i < N; i++)
		printf("recover: %3d %+9.5f %+9.5f I vs. %+9.5f %+9.5f I\n",
			i, in[i][0], in[i][1], in2[i][0], in2[i][1]);
	fftw_destroy_plan(q);

	fftw_cleanup();
	return 0;
	
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// run IMAQ in x86
int main()

{
	INTERFACE_ID interfaceID;

	SESSION_ID sessionID;

	if (imgInterfaceOpen("img0", &interfaceID) == IMG_ERR_GOOD) // "img0" is interfacename, opens an interface named img0

	{
		std::cout << "Interface is open" << std::endl;
		if (imgSessionOpen(interfaceID, &sessionID) == IMG_ERR_GOOD)

		{
			std::cout << "Session is open" << std::endl;
			/*
			uInt32 top, left, height, width, rowpixels;

			std::cout << "Acquisition Window TOP: ";
			std::cin >> top;
			std::cout << "\n";

			std::cout << "Acquisition Window LEFT: ";
			std::cin >> left;
			std::cout << "\n";

			std::cout << "Acquisition Window HEIGHT: ";
			std::cin >> height;
			std::cout << "\n";

			std::cout << "Acquisition Window WIDTH: ";
			std::cin >> width;
			std::cout << "\n";

			std::cout << "Acquisition Window ROW PIXELS: ";
			std::cin >> rowpixels;
			std::cout << "\n";
			*/

			//imgSnapArea(sessionID, NULL, top, left, height, width, rowpixels);
			//if (imgSnapArea(sessionID, NULL, top, left, height, width, rowpixels))
			//{
			//	std::cout << "imgSnapArea success!" << std::endl;
			//}


			// establish window (1st argument) in imgPlot		declared on 31 august,	continue on 1sept 2021
			//uInt32 left_buff_offset, top_buff_offset, xsize, ysize, xpos, ypos, flags = 1000;
			//imgPlot(window, NULL, left_buff_offset, top_buff_offset, xsize, ysize, xpos, ypos, flags);


			// 30 sept 2021: try GRAB
			// 1. imgInterfaceOpen ()
			// 2. imgSessionOpen ()
			// 3. imgGrabSetup ()
			// 4. imgSessionStartAcquisition ()
			// 5. imgGrab (X)
			// 6. User specific functions
			// 7. imgSessionStop Acquisition (X)
			// 8. imgClose ()

			//3. imgGrabSetup
			imgGrabSetup(sessionID, TRUE); //manually start acquisition with imgSessionStartAcquisition

			//4. imgSessionStartAcquisition
			imgSessionStartAcquisition(sessionID);

			//5. imgGrab (sessionID, buffer address, uint32 waitForNext)
			imgGrab(sessionID, NULL, TRUE);


			//6. functions or display using imgPlot


			//7. imgSessionStop Acquisition
			imgSessionStopAcquisition(sessionID);

			imgClose(sessionID, FALSE);	// closes the session using the imgClose function
			std::cout << "Session is closed" << std::endl;

		}

		imgClose(interfaceID, FALSE);	// closes the interface using the imgClose function
		std::cout << "Interface is closed" << std::endl;

	}
}
