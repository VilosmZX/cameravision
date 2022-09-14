#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define MAX_BLUR 100
#define KERNEL_SIZE 17
#define MAX_GAMMA 100

typedef struct RecordStruct {
  bool isRecord;
  cv::String recordStatus;

  RecordStruct(bool isRecord, cv::String recordStatus)
  {
    this->isRecord = isRecord;
    this->recordStatus = recordStatus;
  }

} record_struct_t ;

void filterGray(int state, void* userData)
{
  bool* isGray= (bool*)userData;
  if (*isGray)
    *isGray = false;
  else 
    *isGray = true;
  std::cout << *isGray << std::endl;
}

void mirrorCam(int state, void* userData)
{
  bool* isMirror = (bool*)userData;
  if (*isMirror)
    *isMirror = false;
  else 
    *isMirror = true;
  std::cout << *isMirror << std::endl;
}

void record(int state, void* userData)
{
  record_struct_t* recordData = (record_struct_t*)userData;
  if (recordData->isRecord)
  {
    recordData->isRecord = false;
    recordData->recordStatus = "Record";
  }
  else 
  {
    recordData->isRecord = true;
    recordData->recordStatus = "Stop";
  }

}


int main() 
{
  cv::Mat frame;
  bool isGray = false, isMirror = false;
  int blur = 1, gamma = 1;
  record_struct_t rs(false, "Record");

  cv::namedWindow("Cam", cv::WINDOW_AUTOSIZE);
  cv::createButton("Gray", filterGray, &isGray, cv::QtButtonTypes::QT_PUSH_BUTTON);
  cv::createButton("Mirror Cam", mirrorCam, &isMirror, cv::QtButtonTypes::QT_CHECKBOX, isMirror);
  cv::createTrackbar("Blur", "Cam", &blur, MAX_BLUR);
  cv::createTrackbar("Contrast", "Cam", &gamma, MAX_GAMMA);
  cv::createButton(rs.recordStatus, record, &rs.isRecord, cv::QtButtonTypes::QT_PUSH_BUTTON, &rs);
  cv::VideoCapture cap(0);
  cv::VideoWriter writer;
  

  while(true)
  {
    cap >> frame;

    cv::GaussianBlur(frame, frame, cv::Size(KERNEL_SIZE, KERNEL_SIZE), blur);
    frame.convertTo(frame, -1, gamma, gamma);

    if (isGray)
      cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    if (isMirror)
    {
      cv::flip(frame, frame, 1);
    } 

    if (rs.isRecord)
    {
      writer.open(
        "tedt.mp4", 
        cv::VideoWriter::fourcc('M', 'P', '4', 'V'), 
        30, 
        cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT))
      );
      cv::putText(
        frame,
        "Recording...",
        cv::Point(frame.cols - 200, 50),
        cv::QT_FONT_NORMAL,
        1.0,
        cv::Scalar(255, 255, 255)
      );
      writer.write(frame);
    }

    cv::imshow("Cam", frame); 
    cv::waitKey(1);
  }

  cap.release();
  writer.release();
  cv::destroyAllWindows();
  return 0;
}