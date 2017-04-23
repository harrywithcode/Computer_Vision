#include "opencv2/OpenCV.hpp"
#include "opencv2/highgui.hpp"
#include < iostream >
#include <math.h>
using namespace cv;
using namespace std;

int main(int argc, char ** argv) {
	if (argc != 3) {
		cout << argv[0] << ": "
			<< " got " << argc - 1 << " arguments . Expecting two : width height ."
			<< endl;
		return (-1);
	}

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int ** RED1 = new int *[height];
	int ** GREEN1 = new int *[height];
	int ** BLUE1 = new int *[height];
	int ** RED2 = new int *[height];
	int ** GREEN2 = new int *[height];
	int ** BLUE2 = new int *[height];

	for (int i = 0; i < height; i++) {
		RED1[i] = new int[width];
		GREEN1[i] = new int[width];
		BLUE1[i] = new int[width];
		RED2[i] = new int[width];
		GREEN2[i] = new int[width];
		BLUE2[i] = new int[width];
	}

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int r1, g1, b1;
			int r2, g2, b2;

			double x = (double)j / (double)width;
			double y = (double)i / (double)height;
			double Y = 1.0;
			double L = 90;
			double u = x * 512 - 255;
			double v = y * 512 - 255;
			/* Your code should be placed here
			It should translate xyY to byte sRGB
			and Luv to byte sRGB
			*/

			//First, convert xyY to XYZ
			double X1 = (x / y) * Y;
			double Y1 = Y;
			double Z1 = ((1 - x - y) / y) * Y;

			//double pic1[3] = { X1, Y1, Z1 };

			//convert Luv to sRGB
			double Xw = 0.95;
			double Yw = 1.0;
			double Zw = 1.09;
			double uw = (4 * Xw) / (Xw + 15 * Yw + 3 * Zw);
			double vw = (9 * Yw) / (Xw + 15 * Yw + 3 * Zw);
			double X2, Y2, Z2;
			Y2 = Y;
			//Apostrophe
			double uApostrophe = (u + 13 * uw * L) / (13 * L);
			double vApostrophe = (v + 13 * vw * L) / (13 * L);

			if (L > 7.9996) {
				Y2 = pow(((L + 16) / 116), 3) * Yw;
			}
			else {
				Y2 = (L / 903.3) * Yw;
			}
			if (vApostrophe == 0) {
				X2 = 0;
				Z2 = 0;
			}
			else {
				X2 = Y2 * 2.25 * (uApostrophe / vApostrophe);
				Z2 = Y2 * (3 - 0.75 * uApostrophe - 5 * vApostrophe) / vApostrophe;
			}
			//double pic2[3] = { X2, Y2, Z2 };
			double pics[6] = { X1, Y1, Z1, X2, Y2, Z2 };
			int rgb[6];
			for (int i = 0; i < 4; i = i + 3) {
				double X = pics[i];
				double Y = pics[i + 1];
				double Z = pics[i + 2];
				int r, g, b;

				double tempr1, tempg1, tempb1;
				tempr1 = ((3.240479) * X + (-1.53715) * Y + (-0.498535) * Z);
				tempg1 = ((-0.969256) * X + (1.875991) * Y + (0.041556) * Z);
				tempb1 = ((0.055648) * X + (-0.204043) * Y + (1.057311) * Z);

				//r1
				if (tempr1 < 0.00304) {
					tempr1 = (12.92 * tempr1);
				}
				else {
					tempr1 = (1.055 * pow(tempr1, 1 / 2.4) - 0.055);
				}
				r = (int)(tempr1 * 255);

				//g1
				if (tempg1 < 0.00304) {
					tempg1 = (12.92 * tempg1);
				}
				else {
					tempg1 = (1.055 * pow(tempg1, 1 / 2.4) - 0.055);
				}
				g = (int)(tempg1 * 255);

				//b1
				if (tempb1 < 0.00304) {
					tempb1 = (12.92 * tempb1);
				}
				else {
					tempb1 = (1.055 * pow(tempb1, 1 / 2.4) - 0.055);
				}
				b = (int)(tempb1 * 255);


				if (r < 0) {
					r = 0;
				}
				if (r > 255) {
					r = 255;
				}
				if (g < 0) {
					g = 0;
				}
				if (g > 255) {
					g = 255;
				}
				if (b < 0) {
					b = 0;
				}
				if (b > 255) {
					b = 255;
				}
				rgb[i] = r;
				rgb[i + 1] = g;
				rgb[i + 2] = b;
			}
			r1 = rgb[0];
			g1 = rgb[1];
			b1 = rgb[2];

			r2 = rgb[3];
			g2 = rgb[4];
			b2 = rgb[5];

			// this is the end of your code

			RED1[i][j] = r1;
			GREEN1[i][j] = g1;
			BLUE1[i][j] = b1;
			RED2[i][j] = r2;
			GREEN2[i][j] = g2;
			BLUE2[i][j] = b2;
		}


	Mat R1(height, width, CV_8UC1);
	Mat G1(height, width, CV_8UC1);
	Mat B1(height, width, CV_8UC1);
	Mat R2(height, width, CV_8UC1);
	Mat G2(height, width, CV_8UC1);
	Mat B2(height, width, CV_8UC1);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {

			R1.at < uchar >(i, j) = RED1[i][j];
			G1.at < uchar >(i, j) = GREEN1[i][j];
			B1.at < uchar >(i, j) = BLUE1[i][j];

			R2.at < uchar >(i, j) = RED2[i][j];
			G2.at < uchar >(i, j) = GREEN2[i][j];
			B2.at < uchar >(i, j) = BLUE2[i][j];
		}

	Mat xyY;
	Mat xyY_planes[] = { B1 , G1 , R1 };
	merge(xyY_planes, 3, xyY);
	namedWindow(" xyY", CV_WINDOW_AUTOSIZE);
	imshow(" xyY", xyY);

	Mat Luv;
	Mat Luv_planes[] = { B2 , G2 , R2 };
	merge(Luv_planes, 3, Luv);
	namedWindow(" Luv", CV_WINDOW_AUTOSIZE);
	imshow(" Luv", Luv);
	waitKey(0); // Wait for a keystroke
	return (0);
}