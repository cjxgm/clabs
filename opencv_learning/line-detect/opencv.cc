#include <err.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void text(Mat & image, const string & str, int r, int g, int b)
{
	cv::rectangle(image, Point(0, 0), Point(800, 40), Scalar(0, 0, 0), -1);
	putText(image, str, Point(10, 30), 2, 1, Scalar(b, g, r), 2);
}

int main(int argc, char * argv[])
{
	string winBefore = "opencv learning - before";
	string winAfter  = "opencv learning - after" ;

	VideoCapture cap(0);
	if (!cap.isOpened()) err(1, "unable to open capture device.");

	namedWindow(winBefore);
	namedWindow(winAfter);
	moveWindow(winAfter, 720, 0);

	Mat gray;
	vector<Vec2f> lines;
	for (;;) {
		Mat frame;
		cap >> frame;

		imshow(winBefore, frame);

		// gray, blur, find edges, find lines.
		cvtColor(frame, gray, CV_BGR2GRAY);
		GaussianBlur(gray, gray, Size(7, 7), 1.5, 1.5);
		Canny(gray, gray, 0, 60, 3);
		HoughLines(gray, lines, 1, CV_PI/180, 100);

		// draw the result
		cvtColor(gray, gray, CV_GRAY2BGR);
		if (!lines.size())
			text(frame, "No line detected.", 255, 100, 0);
		else if (lines.size() < 100) {
			stringstream ss;
			ss << lines.size() << " line(s) detected.";
			text(frame, ss.str(), 100, 255, 0);

			for (auto line : lines) {
				float rho   = line[0];
				float theta = line[1];
				double c = cos(theta), s = sin(theta);
				double x = c*rho, y = s*rho;
				Point p1(x - 1000*s, y + 1000*c);
				Point p2(x + 1000*s, y - 1000*c);
				cv::line(gray, p1, p2, Scalar(0, 0, 255), 1, CV_AA);
			}
		}
		else {
			stringstream ss;
			ss << "Too many lines (" << lines.size() << ").";
			text(frame, ss.str(), 255, 0, 100);
		}

		text(gray, "", 0, 0, 0);	// clear out heads.
		addWeighted(frame, 1, gray, 1, 0.0, frame);
		imshow(winAfter, frame);

		if (waitKey(30) >= 0) break;
	}

	return 0;
}

