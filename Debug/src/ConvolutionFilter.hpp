#pragma once

#include "ImageFilter.hpp"

/// Implements a sobel operator
class ConvolutionFilter:public ImageFilter {
public:
	ConvolutionFilter(const DeviceInterface&dev, std::string programText,
			std::string kernelName,unsigned int dIn,unsigned int dOut,bool buildKernelFromFile=true) :
			ImageFilter(dev, programText, kernelName, dIn, dOut,buildKernelFromFile) {
	};
	virtual ~ConvolutionFilter() {
	};
	void operator()(unsigned char*input, unsigned char*output, unsigned int currWidth,
			unsigned int currHeight, const float *convWeight, int filterSize) {
		unsigned int bytesIn = currWidth * currHeight * depthIn;
		unsigned int bytesOut = currWidth * currHeight * depthOut;
		resizeBuffers(currWidth,currHeight);
		// the frame is to be passed as an argument to the kernel function
		// the actual call to the kernel function is represented by an
		// object of type cl::Kernel which stores the kernel arguments
		setArg(0, inputBuffer);
		setArg(1, outputBuffer);

		// image width/height has to be passed anyway for each new input image
		setArg(2, currWidth);
		setArg(3, currHeight);
		setArg(4, filterSize);
		setArg(5, convWeight);

		// copy frame from CPU to GPU memory
				SAFE_CALL(
						deviceInterface.getQueue().enqueueWriteBuffer(inputBuffer,
								CL_TRUE, 0, bytesIn, input));


				// call the kernel
				SAFE_CALL(
						deviceInterface.getQueue().enqueueNDRangeKernel(kernel,
								cl::NullRange /* offset */,
								cl::NDRange(width,height) /* global size (number of work items) */,
								cl::NullRange /* work group size (here, again, selected by OpenCL) */));

				// finish all enqueued commands -- i.e., wait for kernel to finish
				SAFE_CALL(deviceInterface.getQueue().finish());

				// copy frame from GPU to CPU memory
				SAFE_CALL(
						deviceInterface.getQueue().enqueueReadBuffer(outputBuffer,
								CL_TRUE, 0, bytesOut, output));
			}

};
