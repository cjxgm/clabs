#include <err.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char * argv[])
{
	String winBefore = "opencv learning - before";
	String winAfter  = "opencv learning - after" ;

	VideoCapture cap(0);
	if (!cap.isOpened()) err(1, "unable to open capture device.");

	namedWindow(winBefore);
	namedWindow(winAfter);
	moveWindow(winAfter, 720, 0);

	Mat edges;
	for (;;) {
		Mat frame;
		cap >> frame;

		imshow(winBefore, frame);

		// edge detection
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		// blend with original image
		cvtColor(edges, edges, CV_GRAY2BGR);
		addWeighted(frame, 1, edges, 1, 0.0, edges);
		imshow(winAfter , edges);

		if (waitKey(30) >= 0) break;
	}

	return 0;
}

