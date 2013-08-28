#include <err.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char * argv[])
{
	if (argc != 2) err(1, "please specify a file.");

	Mat image = imread(argv[1]);
	if (image.empty()) err(1, "unable to open file '%s'.", argv[1]);

	namedWindow("display image");
	imshow("display image", image);
	waitKey(0);

	return 0;
}

