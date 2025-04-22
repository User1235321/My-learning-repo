#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
  cv::VideoCapture cap(0);
  if (!cap.isOpened())
  {
    std::cerr << "main.cpp, line 10: cannot open the video camera\n";
    return -1;
  }

  cv::VideoWriter oVideoWriter("/home/pavel/Desktop/PolikekWorks/My-learning-repo/Sem4/Computer acrhitector/Labs/Lab5/video.avi",
                                cv::VideoWriter::fourcc('M','J','P','G'),
                                24,
                                cv::Size(static_cast< int >(cap.get(cv::CAP_PROP_FRAME_WIDTH)), static_cast< int >(cap.get(cv::CAP_PROP_FRAME_HEIGHT))),
                                true);

  if (!oVideoWriter.isOpened())
  {
    std::cerr << "main.cpp, line 21: cannot save the video to a file\n";
    return -1;
  }

  cv::Mat watermark = cv::imread("/home/pavel/Desktop/PolikekWorks/My-learning-repo/Sem4/Computer acrhitector/Labs/Lab5/watermark.png");

  while (cv::waitKey(10) != 27)
  {
    cv::Mat frame;
    if (!cap.read(frame))
    {
      std::cerr << "main.cpp, line 31: video camera is disconnected\n";
      break;
    }
    cv::Mat res = (0.9 * frame) + (0.1 * watermark);
    oVideoWriter.write(res);
    cv::imshow("Camera", res);
  }

  oVideoWriter.release();
  return 0;
}
