debug:
	g++ -I/usr/include/opencv4 -L/usr/lib/ -lopencv_imgproc -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_highgui  -g -O3 -o main main.cpp 