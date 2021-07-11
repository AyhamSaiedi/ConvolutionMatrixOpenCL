#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "OpenCLInterface.hpp"
#include "ConvolutionFilter.hpp"

static float guassianBlurFactor = 273.0f;
static float guassianBlur[25] = {

		1.0f, 4.0f, 7.0f, 4.0f, 1.0f,
		4.0f, 16.0f, 26.0f, 16.0f, 4.0f,
		7.0f, 26.0f, 41.0f, 26.0f, 7.0f,
		4.0f, 16.0f, 26.0f, 16.0f, 4.0f,
		1.0f, 4.0f, 7.0f, 4.0f, 1.0f
};

static int filterWidth = 5;

DeviceInterface defaultDevice(CL_DEVICE_TYPE_GPU);
ConvolutionFilter convolutionFilter(defaultDevice,"src/convolutionFilter.cl", "convolution", 3, 3, true);

int main(int argc, const char** argv) {
	cv::VideoCapture capture(0); //0=default, -1=any camera, 1..99=your camera
	cv::Mat frame;



	bool cameraOn = capture.isOpened();
	if (cameraOn) {
		if (!capture.read(frame))
			exit(3);
	} else {
		std::cerr << "No camera detected" << std::endl;
		frame = cv::imread("preview.png");
		if(frame.data == NULL)
			exit(3);
	}

	for (int i = 0; i < filterWidth*filterWidth; i++)
	{
		guassianBlur[i] = guassianBlur[i]/guassianBlurFactor;
	}

	const unsigned int w = frame.cols;
	const unsigned int h = frame.rows;

	// image after applying sobel-based effects is color, again
	cv::Mat convolutionFrame(h, w, CV_8UC3);

	cv::namedWindow("preview", 0);
	cv::namedWindow("convolution", 0);

	//while (((char)cv::waitKey(10)) <= -1) {
		if (cameraOn && !capture.read(frame))
			exit(3);
		convolutionFilter(frame.data, convolutionFrame.data, w, h, guassianBlur, filterWidth);
		// show the result
		cv::imshow("preview", frame);
		cv::imshow("convolution", convolutionFrame);
	//}
		while (((char)cv::waitKey(10)) <= -1);
	//cv::destroyWindow("preview");
	//cv::destroyWindow("converted");
	//cv::destroyWindow("edge");
	//cv::destroyWindow("effect");
	//cv::destroyWindow("con");

	return 0;
}
