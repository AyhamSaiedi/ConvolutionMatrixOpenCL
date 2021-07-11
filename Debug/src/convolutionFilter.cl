#define ARRC(A,x,y,maxX,channel) (A[((x)+(y)*(maxX))*3+channel])

__Kernel
void convolution(
	__read_only unsigned char *inImg,
	__write_only unsigned char *outputImage,
	int w, 
	int h,
	int filterWidth,
	__constant float* filter)
	{
	int x = get_global_id(0);
	int y = get_global_id(1);
	
	int halfWidth = (int)(filterWidth/2)
	
	int r;
	int g;
	int b;


	int filterIdx = 0;

	int coords_x; //coords for accessing the image
	int coords_y;

	if(x<w && y<h){
	for(int i = -halfWidth; i <= halfWidth; i++){
		coords_y = y + i;
		for(int j = -halfWidth; j <= halfWidth; j++){
			coords_x = x + j;
			if(coords_x<w && coords_y<h){
			r += ARRC(inImg, coords_x, coords_y, w, 0) * filter[filterIdx++];
			g += ARRC(inImg, coords_x, coords_y, w, 1) * filter[filterIdx++];
			b += ARRC(inImg, coords_x, coords_y, w, 2) * filter[filterIdx++];
				}
				else{ 
				filterIdx++;
				}
			}
		}
		ARRC(outputImage, x, y, w, 0) = r;
		ARRC(outputImage, x, y, w, 1) = g;
		ARRC(outputImage, x, y, w, 2) = b;
		}
	}

