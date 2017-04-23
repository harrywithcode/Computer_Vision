#include "opencv2/highgui.hpp"
#include < iostream >
using namespace cv;
using namespace std;
void runOnWindow(int W1, int H1, int W2, int H2, Mat inputImage, char * outName) {
	int rows = inputImage.rows;
	int cols = inputImage.cols;

	vector < Mat > i_planes;
	split(inputImage, i_planes);
	Mat iB = i_planes[0];
	Mat iG = i_planes[1];
	Mat iR = i_planes[2];

	// dynamically allocate RGB arrays of size rows x cols
	int ** R = new int *[rows];
	int ** G = new int *[rows];
	int ** B = new int *[rows];
	for (int i = 0; i < rows; i++) {
		R[i] = new int[cols];
		G[i] = new int[cols];
		B[i] = new int[cols];
	}

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			R[i][j] = iR.at < uchar >(i, j);
			G[i][j] = iG.at < uchar >(i, j);
			B[i][j] = iB.at < uchar >(i, j);
		}


	// The transformation should be based on the
	// historgram of the pixels in the W1 ,W2 ,H1 ,H2 range .
	// The following code goes over these pixels
	double maxL = 0.0;
	double minL = 100.0;
	int h[101];
	for (int i = 0; i < 101; i++) {
		h[i] = 0;
	}
	//int* h = new int[101];
	for (int i = H1; i <= H2; i++) {
		for (int j = W1; j <= W2; j++) {
			double r = R[i][j];
			double g = G[i][j];
			double b = B[i][j];
			r = r / 255;
			g = g / 255;
			b = b / 255;
			//convert non-linear RGB to linear RGB
			double linearr;
			double linearg;
			double linearb;
			if (r < 0.03928) {
				linearr = r / 12.92;
			}
			else {
				linearr = pow(((r + 0.055) / 1.055), 2.4);
			}

			if (g < 0.03928) {
				linearg = g / 12.92;
			}
			else {
				linearg = pow(((g + 0.055) / 1.055), 2.4);
			}

			if (b < 0.03928) {
				linearb = b / 12.92;
			}
			else {
				linearb = pow(((b + 0.055) / 1.055), 2.4);
			}
			//covert RGB to XYZ
			double X = 0.412453 * linearr + 0.35758 * linearg + 0.180423 * linearb;
			double Y = 0.212671 * linearr + 0.71516 * linearg + 0.072169 * linearb;
			double Z = 0.019334 * linearr + 0.119193 * linearg + 0.950227 * linearb;


			//Covert XYZ to Luv
			double Xw = 0.95;
			double Yw = 1.0;
			double Zw = 1.09;
			double uw = (4.0 * Xw) / (Xw + 15.0 * Yw + 3.0 * Zw);
			double vw = (9.0 * Yw) / (Xw + 15.0 * Yw + 3.0 * Zw);

			double L;
			double t = Y / Yw;

			if (t > 0.008856) {
				L = 116.0 * pow(t, 1.0 / 3.0) - 16.0;
			}
			else {
				L = 903.3 * t;
			}

			if (L > maxL) {
				maxL = L;
			}
			if (L < minL) {
				minL = L;
			}

			if (L < 0.0) {
				L = 0.0;
			}
			if (L > 100.0) {
				L = 100.0;
			}
			int inL = (int)L;
			h[inL] = h[inL] + 1;
		}
	}
	/*
	int a = h[0];
	int testa = (W2 - W1+1)*(H2 - H1+1);
	int testb = 0;
	for (int i = 0; i < 101; i++) {
		testb = testb + h[i];
	}
	int testc = testb;
	*/

	int f[101];
	for (int i = 0; i < 101; i++) {
		f[i] = 0;
	}

	f[0] = h[0];//0
	f[1] = h[1] + f[0];//2
	for (int i = 1; i < 101; i++) {
		f[i] = h[i] + f[i - 1];
	}

	int floor[101];
	for (int i = 0; i < 101; i++) {
		floor[i] = 0;
	}

	double base = (f[0] / 2.0)*(101.0 / ((W2 - W1 + 1)*(H2 - H1 + 1)));
	floor[0] = (int)base;
	//double ca = ((0 + 2) / 2.0)*(101.0 / ((W2 - W1 + 1)*(H2 - H1 + 1)));
	int tt = 0;
	for (int i = 1; i < 101; i++) {
		double cal = ((f[i - 1] + f[i]) / 2.0)*(101.0 / ((W2 - W1 + 1)*(H2 - H1 + 1)));
		floor[i] = (int)cal;
		
		if (floor[i] > 100) {
			floor[i] = 100;
		}
	}
	
	for (int i = 0; i <101; i++)
		cout << floor[i] << "	";

	//double ttt = floor[5];
	//double ddd = floor[99];
	//double bbb = floor[100];
	//int ttt = tt;
	//int test = floor[0];
	//int test1 = floor[1];
	//All floor items are 0, problem is here
	//traverse picture
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			double r = R[i][j];
			double g = G[i][j];
			double b = B[i][j];
			r = r / 255;
			g = g / 255;
			b = b / 255;
			//convert non-linear RGB to linear RGB
			double linearr;
			double linearg;
			double linearb;
			if (r < 0.03928) {
				linearr = r / 12.92;
			}
			else {
				linearr = pow(((r + 0.055) / 1.055), 2.4);
			}

			if (g < 0.03928) {
				linearg = g / 12.92;
			}
			else {
				linearg = pow(((g + 0.055) / 1.055), 2.4);
			}

			if (b < 0.03928) {
				linearb = b / 12.92;
			}
			else {
				linearb = pow(((b + 0.055) / 1.055), 2.4);
			}
			//covert RGB to XYZ
			double X = 0.412453 * linearr + 0.35758 * linearg + 0.180423 * linearb;
			double Y = 0.212671 * linearr + 0.71516 * linearg + 0.072169 * linearb;
			double Z = 0.019334 * linearr + 0.119193 * linearg + 0.950227 * linearb;


			//Covert XYZ to Luv
			double Xw = 0.95;
			double Yw = 1.0;
			double Zw = 1.09;
			double uw = (4.0 * Xw) / (Xw + 15.0 * Yw + 3.0 * Zw);
			double vw = (9.0 * Yw) / (Xw + 15.0 * Yw + 3.0 * Zw);

			double L;
			double t = Y / Yw;

			if (t > 0.008856) {
				L = 116.0 * pow(t, 1.0 / 3.0) - 16.0;
			}
			else {
				L = 903.3 * t;
			}
			if (L < 0) {
				L = 0;
			}
			if (L > 100) {
				L = 100;
			}
						
			double d = X + 15.0 * Y + 3.0 * Z;
			double uApostrophe = 4.0 * X / d;
			double vApostrophe = 9.0 * Y / d;
			double u = 13.0 * L*(uApostrophe - uw);
			double v = 13.0 * L*(vApostrophe - vw);


			int integerL = (int)L;
			int ul = floor[integerL];
			L = (double)ul;

			if (L < 0.0) {
				L = 0.0;
			}
			if (L > 100.0) {
				L = 100.0;
			}


			//

			//covert Luv to XYZ
			double X2, Y2, Z2;
			//Apostrophe
			uApostrophe = (u + 13.0 * uw * L) / (13.0 * L);
			vApostrophe = (v + 13.0 * vw * L) / (13.0 * L);

			if (L > 7.9996) {
				Y2 = pow(((L + 16.0) / 116.0), 3.0) * Yw;
			}
			else {
				Y2 = (L / 903.3) * Yw;
			}
			if (vApostrophe == 0) {
				X2 = 0.0;
				Z2 = 0.0;
			}
			else {
				X2 = Y2 * 2.25 * (uApostrophe / vApostrophe);
				Z2 = Y2 * (3 - 0.75 * uApostrophe - 5.0 * vApostrophe) / vApostrophe;
			}


			//XYZ to non-linear RGB
			double tempr1, tempg1, tempb1;
			tempr1 = ((3.240479) * X2 + (-1.53715) * Y2 + (-0.498535) * Z2);
			tempg1 = ((-0.969256) * X2 + (1.875991) * Y2 + (0.041556) * Z2);
			tempb1 = ((0.055648) * X2 + (-0.204043) * Y2 + (1.057311) * Z2);

			//convert non-linear to linear
			if (tempr1 < 0.00304) {
				tempr1 = (12.92 * tempr1);
			}
			else {
				tempr1 = (1.055 * pow(tempr1, 1.0 / 2.4) - 0.055);
			}

			R[i][j] = (int)(tempr1 * 255);

			//g1

			if (tempg1 < 0.00304) {
				tempg1 = (12.92 * tempg1);
			}
			else {
				tempg1 = (1.055 * pow(tempg1, 1.0 / 2.4) - 0.055);
			}

			G[i][j] = (int)(tempg1 * 255);

			//b1

			if (tempb1 < 0.00304) {
				tempb1 = (12.92 * tempb1);
			}
			else {
				tempb1 = (1.055 * pow(tempb1, 1.0 / 2.4) - 0.055);
			}

			B[i][j] = (int)(tempb1 * 255);


			if (R[i][j] < 0) {
				R[i][j] = 0;
			}
			if (R[i][j] > 255) {
				R[i][j] = 255;
			}
			if (G[i][j] < 0) {
				G[i][j] = 0;
			}
			if (G[i][j] > 255) {
				G[i][j] = 255;
			}
			if (B[i][j] < 0) {
				B[i][j] = 0;
			}
			if (B[i][j] > 255) {
				B[i][j] = 255;
			}
		}
	}

	Mat oR(rows, cols, CV_8UC1);
	Mat oG(rows, cols, CV_8UC1);
	Mat oB(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			oR.at < uchar >(i, j) = R[i][j];;
			oG.at < uchar >(i, j) = G[i][j];;
			oB.at < uchar >(i, j) = B[i][j];;
		}

	Mat o_planes[] = { oB , oG , oR };
	Mat outImage;
	merge(o_planes, 3, outImage);

	namedWindow(" output ", CV_WINDOW_AUTOSIZE);
	imshow(" output ", outImage);
	imwrite(outName, outImage);
}

int main(int argc, char ** argv) {
	if (argc != 7) {
		cerr << argv[0] << ": "
			<< "got " << argc - 1
			<< " arguments . Expecting six : w1 h1 w2 h2 ImageIn ImageOut ."
			<< endl;
		cerr << " Example : proj1b 0.2 0.1 0.8 0.5 fruits .jpg out .bmp " << endl;
		return (-1);
	}
	double w1 = atof(argv[1]);
	double h1 = atof(argv[2]);
	double w2 = atof(argv[3]);
	double h2 = atof(argv[4]);
	char * inputName = argv[5];
	char * outputName = argv[6];

	if (w1 <0 || h1 <0 || w2 <= w1 || h2 <= h1 || w2 >1 || h2 >1) {
		cerr << " arguments must satisfy 0 <= w1 < w2 <= 1"
			<< " , 0 <= h1 < h2 <= 1" << endl;
		return (-1);
	}

	Mat inputImage = imread(inputName, CV_LOAD_IMAGE_UNCHANGED);
	if (inputImage.empty()) {
		cout << " Could not open or find the image " << inputName << endl;
		return (-1);
	}

	string windowInput(" input : ");
	windowInput += inputName;

	namedWindow(windowInput, CV_WINDOW_AUTOSIZE);
	imshow(windowInput, inputImage);

	if (inputImage.type() != CV_8UC3) {
		cout << inputName << " is not a standard color image " << endl;
		return (-1);
	}

	int rows = inputImage.rows;
	int cols = inputImage.cols;
	int W1 = (int)(w1 *(cols - 1));
	int H1 = (int)(h1 *(rows - 1));
	int W2 = (int)(w2 *(cols - 1));
	int H2 = (int)(h2 *(rows - 1));

	runOnWindow(W1, H1, W2, H2, inputImage, outputName);

	waitKey(0); // Wait for a keystroke
	return (0);
}
