#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

using namespace cv;
using namespace std;

ofstream myfile("log.txt");

Mat mm(Mat u, Mat v){
    Mat w = Mat_<double>(u.rows, v.cols);
    for(int i = 0; i < u.rows; i++){
        for(int j = 0; j < v.cols; j++){
            int sum = 0;
            for(int k = 0; k < u.cols; k++){
                sum += v.at<double>(k, j) * u.at<double>(i, k);
            }
            w.at<double>(i, j) = sum;
        }
    }
    return w;
}

vector<Mat> rot(vector<Mat> vec, int deg){
    vector<Mat> output;
    for(int i = 0; i < vec.size(); i++){
        output.push_back(mm((Mat_<double>(4, 4) << cos(deg * M_PI/180), -1*sin(deg * M_PI/180), 0, 0, sin(deg * M_PI/180), cos(deg * M_PI/180), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) * (Mat_<double>(4, 4) << 1, 0, 0, 0, 0, cos(deg * M_PI/180), -1*sin(deg * M_PI/180), 0, 0, sin(deg * M_PI/180), cos(deg * M_PI/180), 1), vec[i]));
    }
    return output;

}

Mat ppc(vector<Mat> vec){
    Mat image(600, 800, CV_8UC3, Scalar(0, 0, 0));
    Mat transformtocenter = (Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1);
    vector<Point> points;
    for (int i = 0; i < vec.size(); i++)
    {
        double t = (400 - 1300) / (vec[i].at<double>(2, 0) - 1300);
        int x = vec[i].at<double>(0, 0) * t + 400;
        int y = vec[i].at<double>(1, 0) * t + 300;
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
        }
    }
    line(image, points[2], points[3], Scalar(255, 255, 255));
    line(image, points[4], points[5], Scalar(255, 0, 0));
    line(image, points[4], points[6], Scalar(0, 255, 0));
    line(image, points[7], points[5], Scalar(0, 0, 255));
    line(image, points[7], points[6], Scalar(255, 0, 255));
    line(image, points[7], points[3], Scalar(255, 255, 255));
    line(image, points[0], points[2], Scalar(255, 0, 0));
    line(image, points[0], points[4], Scalar(0, 255, 0));
    line(image, points[0], points[1], Scalar(0, 0, 255));
    line(image, points[1], points[3], Scalar(255, 255, 0));
    line(image, points[1], points[5], Scalar(0, 255, 255));
    line(image, points[2], points[6], Scalar(255, 0, 255));
    return image;
}

Mat ppt(vector<Mat> vec)
{
    vector<Point> points;
    Mat image(600, 800, CV_8UC3, Scalar(0, 0, 0));
    for (int i = 0; i < vec.size(); i++)
    {
        double t = (400 - 1300) / (vec[i].at<double>(2, 0) - 1300);
        int x = vec[i].at<double>(0, 0) * t + 400;
        int y = vec[i].at<double>(1, 0) * t + 300;
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(255, 0, 0), 3);
        }
    }
    line(image, points[1], points[2], Scalar(255, 255, 0));
    line(image, points[1], points[3], Scalar(0, 255, 255));
    line(image, points[2], points[3], Scalar(255, 0, 255));
    line(image, points[0], points[1], Scalar(255, 0, 0));
    line(image, points[0], points[2], Scalar(0, 255, 0));
    line(image, points[0], points[3], Scalar(0, 0, 255));
    return image;
}

Mat twod(vector<Mat> vec){
    Mat image(600, 800, CV_8UC3, Scalar(0, 0, 0));  
    vector<Point> points;
    for(int i = 0; i < vec.size(); i++){
        Mat temp = mm((Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1), vec[i]);
        int x = temp.at<double>(0, 0);
        int y = temp.at<double>(1, 0);
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
        }
    }
    line(image, points[2], points[3], Scalar(255, 0, 255));
    line(image, points[4], points[5], Scalar(0, 0, 255));
    line(image, points[4], points[6], Scalar(255, 0, 0));
    line(image, points[7], points[5], Scalar(0, 255, 0));
    line(image, points[7], points[6], Scalar(0, 0, 255));
    line(image, points[7], points[3], Scalar(0, 0, 0));
    line(image, points[0], points[2], Scalar(0, 255, 0));
    line(image, points[0], points[4], Scalar(255, 0, 0));
    line(image, points[0], points[1], Scalar(0, 255, 0));
    line(image, points[1], points[3], Scalar(0, 0, 255));
    line(image, points[1], points[5], Scalar(255, 255, 0));
    line(image, points[2], points[6], Scalar(0, 255, 255));
    return image;
}

void coordsToText(vector<Mat> vec){
    myfile << "[" << vec[2].at<double>(0, 0) << ", " << vec[2].at<double>(1, 0) << ", " << vec[2].at<double>(2, 0) << "] " << "[ " << vec[3].at<double>(0, 0) << ", " << vec[3].at<double>(1, 0) << ", " << vec[3].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[4].at<double>(0, 0) << ", " << vec[4].at<double>(1, 0) << ", " << vec[4].at<double>(2, 0) << "] " << "[ " << vec[5].at<double>(0, 0) << ", " << vec[5].at<double>(1, 0) << ", " << vec[5].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[4].at<double>(0, 0) << ", " << vec[4].at<double>(1, 0) << ", " << vec[4].at<double>(2, 0) << "] " << "[ " << vec[6].at<double>(0, 0) << ", " << vec[6].at<double>(1, 0) << ", " << vec[6].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[7].at<double>(0, 0) << ", " << vec[7].at<double>(1, 0) << ", " << vec[7].at<double>(2, 0) << "] " << "[ " << vec[5].at<double>(0, 0) << ", " << vec[5].at<double>(1, 0) << ", " << vec[5].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[7].at<double>(0, 0) << ", " << vec[7].at<double>(1, 0) << ", " << vec[7].at<double>(2, 0) << "] " << "[ " << vec[6].at<double>(0, 0) << ", " << vec[6].at<double>(1, 0) << ", " << vec[6].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[7].at<double>(0, 0) << ", " << vec[7].at<double>(1, 0) << ", " << vec[7].at<double>(2, 0) << "] " << "[ " << vec[3].at<double>(0, 0) << ", " << vec[3].at<double>(1, 0) << ", " << vec[3].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[0].at<double>(0, 0) << ", " << vec[0].at<double>(1, 0) << ", " << vec[0].at<double>(2, 0) << "] " << "[ " << vec[2].at<double>(0, 0) << ", " << vec[2].at<double>(1, 0) << ", " << vec[2].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[0].at<double>(0, 0) << ", " << vec[0].at<double>(1, 0) << ", " << vec[0].at<double>(2, 0) << "] " << "[ " << vec[4].at<double>(0, 0) << ", " << vec[4].at<double>(1, 0) << ", " << vec[4].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[0].at<double>(0, 0) << ", " << vec[0].at<double>(1, 0) << ", " << vec[0].at<double>(2, 0) << "] " << "[ " << vec[1].at<double>(0, 0) << ", " << vec[1].at<double>(1, 0) << ", " << vec[1].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[1].at<double>(0, 0) << ", " << vec[1].at<double>(1, 0) << ", " << vec[1].at<double>(2, 0) << "] " << "[ " << vec[3].at<double>(0, 0) << ", " << vec[3].at<double>(1, 0) << ", " << vec[3].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[1].at<double>(0, 0) << ", " << vec[1].at<double>(1, 0) << ", " << vec[1].at<double>(2, 0) << "] " << "[ " << vec[5].at<double>(0, 0) << ", " << vec[5].at<double>(1, 0) << ", " << vec[5].at<double>(2, 0) << "]\n";
    myfile << "[" << vec[2].at<double>(0, 0) << ", " << vec[2].at<double>(1, 0) << ", " << vec[2].at<double>(2, 0) << "] " << "[ " << vec[6].at<double>(0, 0) << ", " << vec[6].at<double>(1, 0) << ", " << vec[6].at<double>(2, 0) << "]\n";
    myfile << "\n";
}

void coordsToText2(vector<Point> vec){
    myfile << vec[2] << " " << vec[3] << "\n";
    myfile << vec[4] << " " << vec[5] << "\n";
    myfile << vec[4] << " " << vec[6] << "\n";
    myfile << vec[7] << " " << vec[5] << "\n";
    myfile << vec[7] << " " << vec[6] << "\n";
    myfile << vec[7] << " " << vec[3] << "\n";
    myfile << vec[0] << " " << vec[2] << "\n";
    myfile << vec[0] << " " << vec[4] << "\n";
    myfile << vec[0] << " " << vec[1] << "\n";
    myfile << vec[1] << " " << vec[3] << "\n";
    myfile << vec[1] << " " << vec[5] << "\n";
    myfile << vec[2] << " " << vec[6] << "\n";
    myfile << "\n";
}

Mat twodt(vector<Mat> vec)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    vector<Point> points;
    for (int i = 0; i < vec.size(); i++)
    {
        Mat temp = mm((Mat_<double>(4, 4) << 1, 0, 0, 400, 0, 1, 0, 300, 0, 0, 1, 0, 0, 0, 0, 1), vec[i]);

        int x = temp.at<double>(0, 0);
        int y = temp.at<double>(1, 0);
        points.push_back(Point(x, y));
        if (x > 0 && x < 800 && y > 0 && y < 600)
        {
            circle(image, Point(x, y), 2, Scalar(0, 0, 0), 3);
        }
    }
    line(image, points[0], points[1], Scalar(255, 0, 0));
    line(image, points[0], points[2], Scalar(0, 255, 0));
    line(image, points[0], points[3], Scalar(0, 0, 255));
    line(image, points[1], points[2], Scalar(255, 0, 255));
    line(image, points[1], points[3], Scalar(255, 255, 0));
    line(image, points[2], points[3], Scalar(0, 255, 255));
    return image;
}

VideoWriter ct(VideoWriter video, bool perspective){
    ofstream myfile;
    myfile.open("coordinates.txt");
    myfile.close();
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    vector<Mat> points;
    for (int i = 0; i < 8; i++)
    {
        double x = (i % 4 < 2) ? 1 : -1;
        double y = (i % 2 == 0) ? 1 : -1;
        double z = (i < 4) ? 1 : -1;
        points.push_back(mm((Mat_<double>(4, 4) << 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 1), (Mat_<double>(4, 1) << x, y, z, 1)));
    }
    vector<Mat> rr = points;
    for (int i = 0; i <= 180; i++)
    {
        if(i > 0 && i < 5){
            coordsToText(rr);
        }
        rr = rot(points, i);
        Mat image;
        if(perspective){
            image = ppc(rr);
        }
        else{
            image = twod(rr);
        }     
        video.write(image);
    }
    return video;
}

VideoWriter tt(VideoWriter video, bool perspective)
{
    Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
    Mat transformation = (Mat_<double>(4, 4) << 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 1);
    double radian = 30 * M_PI / 180;
    vector<Mat> points; // store a vector of the points in an platonic tetrahedron
    points.push_back((Mat_<double>(4, 1) << 1/ sqrt(3), 0, 0, 1));
    points.push_back((Mat_<double>(4, 1) << 0, 0, 2 / sqrt(6), 1));
    points.push_back((Mat_<double>(4, 1) << -1 * sqrt(3) / 6, 0.5, 0, 1));
    points.push_back((Mat_<double>(4, 1) << -1 * sqrt(3) / 6, -0.5, 0, 1));
    for (int i = 0; i < points.size(); i++)
    {
        Mat newpoint = mm(transformation, points[i]);
        points[i] = newpoint;
    }
    vector<Mat> res = points;
    for (int i = 0; i <= 180; i++)
    {
        res = rot(points, i);
        if (perspective)
        {
            image = ppt(res);
        }
        else
        {
            image = twodt(res);
        }
        video.write(image);
    }
    return video;
}

void part1(){
    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
    video = ct(video, false);
    video = tt(video, false);
}

void part2(){
    VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
    video = ct(video, true);
    video = tt(video, true);
}

void part3(){
   VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(800, 600), true);
   Mat eye = (Mat_<double>(3, 1) << 500, 600, 500);
   Mat n = (Mat_<double>(3, 1) << 1, 1, 2);
   Mat a = (Mat_<double>(3, 1) << 400, 300, 400);
   myfile << "The plane defined by (x-a)*n=0 is: \n";
   myfile << "a = " << a << "\n";
   myfile << "n = " << n << "\n";
   myfile << "The eye e is: \n";
   myfile << "e = " << eye << "\n";
   myfile << "The coordinates in the plane x = p0 + u*w1 + v*w2 is: \n"; 
   vector<Mat> points;
   for(int i = 0; i < 8; i++){
      int x = (i % 4 < 2) ? 1 : -1;
      int y = (i % 2 == 0) ? 1 : -1;
      int z = (i < 4) ? 1 : -1;
      points.push_back(mm((Mat_<double>(4, 4) << 250, 0, 0, 0, 0, 250, 0, 0, 0, 0, 250, 0, 0, 0, 0, 1), (Mat_<double>(4, 1) << x, y, z, 1)));
   }
   Mat rotatedPoint = points[0];
   Mat otherPoint = points[1];
   Mat otherPoint2 = points[2]; 
   double num = (a - eye).dot(n);
       	 Mat denom = (Mat_<double>(3, 1) << rotatedPoint.at<double>(0, 0) - eye.at<double>(0, 0), rotatedPoint.at<double>(1, 0) - eye.at<double>(1, 0), rotatedPoint.at<double>(2, 0) - eye.at<double>(2, 0));
         double down = denom.dot(n);
         double t = num / down;
         Mat vec = (Mat_<double>(3, 1) << t * (rotatedPoint.at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (rotatedPoint.at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (rotatedPoint.at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
       	 Mat denom1 = (Mat_<double>(3, 1) << otherPoint.at<double>(0, 0) - eye.at<double>(0, 0), otherPoint.at<double>(1, 0) - eye.at<double>(1, 0), otherPoint.at<double>(2, 0) - eye.at<double>(2, 0));
         double down1 = denom1.dot(n);
         t = num / down1;
         Mat p1 = (Mat_<double>(3, 1) << t * (otherPoint.at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (otherPoint.at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (otherPoint.at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
       	 Mat denom2 = (Mat_<double>(3, 1) << otherPoint2.at<double>(0, 0) - eye.at<double>(0, 0), otherPoint2.at<double>(1, 0) - eye.at<double>(1, 0), otherPoint2.at<double>(2, 0) - eye.at<double>(2, 0));
         double down2 = denom2.dot(n);
         t = num / down2;
         Mat p2 = (Mat_<double>(3, 1) << t * (otherPoint2.at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (otherPoint2.at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (otherPoint2.at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
         Mat h = p1 - p2;
	 Mat i = vec - p2;
	 Mat w1 = (1 / sqrt(h.dot(h))) * h;
	 Mat k = i - (i.dot(h) / h.dot(h)) * h;
	 Mat w2 = (1 / sqrt(k.dot(k))) * k;   
   double t0 = ((a - eye).dot(n)) / ((-1*eye).dot(n));
   Mat p0 = t0 * -1 * eye + eye;
   cout << p0 << "\n";   
   myfile << "p0 = " << p0 << "\n";
   myfile << "w1 = " << w1 << "\n";
   myfile << "w2 = " << w2 << "\n";
   for(int deg = 0; deg < 180; deg++){
      Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
      vector<Mat> rotated = rot(points, deg);
      if(deg < 4){
         coordsToText(rotated);
      }
      vector<Point> newpoints;
      for(int j = 0; j < rotated.size(); j++){
         Mat denom = (Mat_<double>(3, 1) << rotated[j].at<double>(0, 0) - eye.at<double>(0, 0), rotated[j].at<double>(1, 0) - eye.at<double>(1, 0), rotated[j].at<double>(2, 0) - eye.at<double>(2, 0));
         double down = denom.dot(n);
         t = num / down;
         Mat vec = (Mat_<double>(3, 1) << t * (rotated[j].at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (rotated[j].at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (rotated[j].at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
       	 double u = (vec - p0).dot(w1) / (w1.dot(w1)) + 400;
         double v = (vec - p0).dot(w2) / (w2.dot(w2)) + 300;
         newpoints.push_back(Point(u, v));
         if(u > 0 && u < 800 && v > 0 && v < 600){
	    circle(image, Point(u, v), 2, Scalar(255, 0, 0), 3);
         }	    
      }
      if(deg < 4){
         coordsToText2(newpoints);
      }
          line(image, newpoints[2], newpoints[3], Scalar(0, 0, 0));
    line(image, newpoints[4], newpoints[5], Scalar(255, 0, 0));
    line(image, newpoints[4], newpoints[6], Scalar(0, 255, 0));
    line(image, newpoints[7], newpoints[5], Scalar(0, 0, 255));
    line(image, newpoints[7], newpoints[6], Scalar(255, 0, 255));
    line(image, newpoints[7], newpoints[3], Scalar(0, 0, 0));
    line(image, newpoints[0], newpoints[2], Scalar(255, 0, 0));
    line(image, newpoints[0], newpoints[4], Scalar(0, 255, 0));
    line(image, newpoints[0], newpoints[1], Scalar(0, 0, 255));
    line(image, newpoints[1], newpoints[3], Scalar(255, 255, 0));
    line(image, newpoints[1], newpoints[5], Scalar(0, 255, 255));
    line(image, newpoints[2], newpoints[6], Scalar(255, 0, 255));
    video.write(image);
   }
    Mat transformation = (Mat_<double>(4, 4) << 500, 0, 0, 0, 0, 500, 0, 0, 0, 0, 500, 0, 0, 0, 0, 1);
    vector<Mat> tetra;
    tetra.push_back((Mat_<double>(4, 1) << 1/ sqrt(3), 0, 0, 1));
    tetra.push_back((Mat_<double>(4, 1) << 0, 0, 2 / sqrt(6), 1));
    tetra.push_back((Mat_<double>(4, 1) << -1 * sqrt(3) / 6, 0.5, 0, 1));
    tetra.push_back((Mat_<double>(4, 1) << -1 * sqrt(3) / 6, -0.5, 0, 1));
    for (int i = 0; i < tetra.size(); i++)
    {
        Mat newpoint = mm(transformation, tetra[i]);
        tetra[i] = newpoint;
    }
    rotatedPoint = tetra[0];
    otherPoint = tetra[1];
    otherPoint2 = tetra[2]; 
    num = (a - eye).dot(n);
       	 Mat td1 = (Mat_<double>(3, 1) << rotatedPoint.at<double>(0, 0) - eye.at<double>(0, 0), rotatedPoint.at<double>(1, 0) - eye.at<double>(1, 0), rotatedPoint.at<double>(2, 0) - eye.at<double>(2, 0));
         double tdown1 = td1.dot(n);
         t = num / tdown1;
         vec = (Mat_<double>(3, 1) << t * (rotatedPoint.at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (rotatedPoint.at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (rotatedPoint.at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
       	 Mat td2 = (Mat_<double>(3, 1) << otherPoint.at<double>(0, 0) - eye.at<double>(0, 0), otherPoint.at<double>(1, 0) - eye.at<double>(1, 0), otherPoint.at<double>(2, 0) - eye.at<double>(2, 0));
         double tdown2 = td2.dot(n);
         t = num / tdown2;
         p1 = (Mat_<double>(3, 1) << t * (otherPoint.at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (otherPoint.at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (otherPoint.at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
       	 Mat td3 = (Mat_<double>(3, 1) << otherPoint2.at<double>(0, 0) - eye.at<double>(0, 0), otherPoint2.at<double>(1, 0) - eye.at<double>(1, 0), otherPoint2.at<double>(2, 0) - eye.at<double>(2, 0));
         double tdown3 = td3.dot(n);
         t = num / tdown3;
         p2 = (Mat_<double>(3, 1) << t * (otherPoint2.at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (otherPoint2.at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (otherPoint2.at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
         h = p1 - p2;
	 i = vec - p2;
	 w1 = (1 / sqrt(h.dot(h))) * h;
	 k = i - (i.dot(h) / h.dot(h)) * h;
	 w2 = (1 / sqrt(k.dot(k))) * k;
    
   t0 = ((a - eye).dot(n)) / ((-1*eye).dot(n));
   p0 = t0 * -1 * eye + eye;
   cout << p0 << "\n";
    for(int deg = 0; deg < 180; deg++){
      Mat image(600, 800, CV_8UC3, Scalar(255, 255, 255));
      vector<Mat> rotated = rot(tetra, deg);
      vector<Point> newpoints;
      for(int j = 0; j < rotated.size(); j++){
         Mat denom = (Mat_<double>(3, 1) << rotated[j].at<double>(0, 0) - eye.at<double>(0, 0), rotated[j].at<double>(1, 0) - eye.at<double>(1, 0), rotated[j].at<double>(2, 0) - eye.at<double>(2, 0));
         double down = denom.dot(n);
         t = num / down;
         Mat vec = (Mat_<double>(3, 1) << t * (rotated[j].at<double>(0, 0) - eye.at<double>(0, 0)) + eye.at<double>(0, 0), t * (rotated[j].at<double>(1, 0) - eye.at<double>(1, 0)) + eye.at<double>(1, 0), t * (rotated[j].at<double>(2, 0) - eye.at<double>(2, 0)) + eye.at<double>(2, 0));
       	 double u = (vec - p0).dot(w1) / (w1.dot(w1)) + 400;
         double v = (vec - p0).dot(w2) / (w2.dot(w2)) + 300;
         newpoints.push_back(Point(u, v));
         if(u > 0 && u < 800 && v > 0 && v < 600){
	    circle(image, Point(u, v), 2, Scalar(255, 0, 0), 3);
         }	    
      }
    line(image, newpoints[0], newpoints[1], Scalar(255, 0, 0));
    line(image, newpoints[0], newpoints[2], Scalar(0, 255, 0));
    line(image, newpoints[0], newpoints[3], Scalar(0, 0, 255));
    line(image, newpoints[1], newpoints[2], Scalar(255, 0, 255));
    line(image, newpoints[1], newpoints[3], Scalar(255, 255, 0));
    line(image, newpoints[2], newpoints[3], Scalar(0, 255, 255));
    video.write(image);
   }
   cout << "finished creating video\n";
  }

int main(){
    //part1();
    //part2();
    part3();
}
