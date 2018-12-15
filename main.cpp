#include <cstdio>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>

using namespace cv;
using namespace std;

int xSobel(Mat image, int x, int y)
{
	return image.at<uchar>(y-1, x-1) +
	2*image.at<uchar>(y, x-1) +
	image.at<uchar>(y+1, x-1) -
	image.at<uchar>(y-1, x+1) -
	2*image.at<uchar>(y, x+1) -
	image.at<uchar>(y+1, x+1);
}


int ySobel(Mat image, int x, int y)
{
	return image.at<uchar>(y-1, x-1) +
	2*image.at<uchar>(y-1, x) +
	image.at<uchar>(y-1, x+1) -
	image.at<uchar>(y+1, x-1) -
	2*image.at<uchar>(y+1, x) -
	image.at<uchar>(y+1, x+1);
}

int main()
{
	Mat image, image2;
	Mat dst; // result matrix
	Mat dst2; // result matrix
	int sum;
	//int element1[3][3]={0 ,1 ,0,1 ,1 ,1,0 ,1 ,0};
	/*int element[5][5]={0 ,1 ,1 ,1 ,0, 
		1 ,1 ,1 ,1 ,1 ,
		1 ,1 ,1 ,1, 1 ,
		1 ,1 ,1 ,1 ,0 ,
		0 ,1 ,1 ,1 ,0};*/
	Mat src = imread("HW5.jpg", 0);

	int t = 20;
	dst = src.clone();
	dst2 = src.clone();
	image2 = src.clone();
	while(t>0)
		{	t--;
			for (int x=0; x<src.rows;x++)
			{
				for(int y=0; y<src.cols;y++)
				{
					int s=0; 
					for (int i = 0; i < 3; i++)
					{
						for (int j = 0; j < 3; j++) 
						{
							if((x+i)>src.rows-1 && (y+j)>src.cols-1)
							{
								if (s < src.at<uchar>(src.rows-1,src.cols-1)  )
									s = image2.at<uchar>(src.rows-1,src.cols-1);
							}
							else if((x+i)>src.rows-1)
							{
								if (s < src.at<uchar>(src.rows-1,y+j)  )
									s = image2.at<uchar>(src.rows-1,y+j);
							}
							else if((y+j)>src.cols-1)
							{
								if (s < src.at<uchar>(x+i,src.cols-1)  )
									s = image2.at<uchar>(x+i,src.cols-1);
							}
							else if (s < src.at<uchar>(x+i,y+j)  )
								s = image2.at<uchar>(x+i,y+j);
						}
					}
					image2.at<uchar>(x,y) = s;
				}
			}
		}
		
		t=30;
		while(t>0)
			{	t--;
				
				for (int x=0; x<src.rows;x++)
				{
					for(int y=0; y<src.cols;y++)
					{
						int s=300; 
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 3; j++) 
							{
								if((x+i)>src.rows-1 && (y+j)>src.cols-1)
								{
									if (s > src.at<uchar>(src.rows-1,src.cols-1)  )
										s = image2.at<uchar>(src.rows-1,src.cols-1);
								}
								else if((x+i)>src.rows-1)
								{
									if (s > src.at<uchar>(src.rows-1,y+j)  )
										s = image2.at<uchar>(src.rows-1,y+j);
								}
								else if((y+j)>src.cols-1)
								{
									if (s > src.at<uchar>(x+i,src.cols-1)  )
										s = image2.at<uchar>(x+i,src.cols-1);
								}
								else if (s >src.at<uchar>(x+i,y+j))
									s = image2.at<uchar>(x+i,y+j);  
							}
						}      
						image2.at<uchar>(x,y) = s;
					}
				}
			}
			
			int morph_size = 2;
			Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
			cout<<element;
			morphologyEx( src, dst, MORPH_CLOSE, element, Point(-1,-1), 14 ); 
			
			int morph_size2 = 2;
			Mat element2 = getStructuringElement( MORPH_ELLIPSE, Size( 2*morph_size2 + 1, 2*morph_size2+1 ), Point( morph_size2, morph_size2 ));
			cout<<element2;
			morphologyEx( dst, dst2, MORPH_OPEN , element2, Point(-1,-1), 35 );

			image = dst2.clone();
			for(int y = 1; y < dst2.rows - 1; y++){
				for(int x = 1; x < dst2.cols - 1; x++){
					sum = abs(xSobel(dst2, x, y)) + abs(ySobel(dst2, x, y));
					if (sum > 255)
						sum = 255;
					if (sum < 0)
						sum = 0;
					image.at<uchar>(y,x) = sum;
				}
			}
			for(int y = 1; y < dst2.rows - 1; y++){
				for(int x = 1; x < dst2.cols - 1; x++)
				{
					if(image.at<uchar>(y,x) != 255)
						image.at<uchar>(y,x) = 0;
				}
			}
			for(int y = 1; y < dst2.rows - 1; y++){
				for(int x = 1; x < dst2.cols - 1; x++)
				{
					if(image.at<uchar>(y,x) == 255)
					{
						src.at<uchar>(y,x) = 255 ;
					}
				}
			}

			imshow("source", src);
			imshow("result", dst);
			imshow("result2", dst2);
			imshow("result3", image);
			imwrite("big banana.jpg", src);
			imwrite("big banana2.jpg", dst);
			imwrite("big banana3.jpg", dst2);
			imwrite("big banana4.jpg", image);
			
			waitKey(0);
			
			return 0;
		}