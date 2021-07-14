#define ARRC(A,x,y,maxX,channel) (A[((x)+(y)*(maxX))*3+channel])

void __kernel convolution(
	unsigned char *inImg,
	unsigned char *outputImage,
	unsigned int w, 
	unsigned int h,
	int filterWidth)
	{
	int x = get_global_id(0);
	int y = get_global_id(1);
	
	float filter[25] = {

		1.0f, 4.0f, 7.0f, 4.0f, 1.0f,
		4.0f, 16.0f, 26.0f, 16.0f, 4.0f,
		7.0f, 26.0f, 41.0f, 26.0f, 7.0f,
		4.0f, 16.0f, 26.0f, 16.0f, 4.0f,
		1.0f, 4.0f, 7.0f, 4.0f, 1.0f
	};
	
	
	int halfWidth = (int)(filterWidth/2);
	
	int r;
	int g;
	int b;


	int filterIdx = 0;

	int coords_x; //coords for accessing the image
	int coords_y;

	if(x<w && y<h && x>0 && y>0){
	for(int i = -halfWidth; i <= halfWidth; i++){
		coords_y = y + i;
		 for(int j = -halfWidth; j <= halfWidth; j++){
			coords_x = x + j;
			if(coords_x<w && coords_y<h && coords_x>0 && coords_y>0){
			r += ARRC(inImg, coords_x, coords_y, w, 0) * filter[filterIdx];
			g += ARRC(inImg, coords_x, coords_y, w, 1) * filter[filterIdx];
			b += ARRC(inImg, coords_x, coords_y, w, 2) * filter[filterIdx];
				filterIdx++;
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

