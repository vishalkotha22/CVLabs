using namespace std;

#include<vector>
#include<math.h>
#include<chrono>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<stack>
#include<algorithm>

const int len = 400;
const int numPoints = 60;
vector<vector<int>> ppm;
#define pb push_back

class Point{
    private:
        double x, y, angle;
    public:
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
        double getAngle(){
            return angle;
        }
        void setX(double coord){
            x = coord;
        }
        void setY(double coord){
            y = coord;
        }
        void setAngle(double ang){
            angle = ang;
        }
};

class Line{
    private:
        Point first;
        Point second;
        bool infinity;
        bool zero;
        double slope;
        double b;
    public:
        Point getFirst(){
            return first;
        }
        Point getSecond(){
            return second;
        }
        void setFirst(Point f){
            first = f;
        }
        void setSecond(Point s){
            second = s;
        }
        void setSlope(double s){
            slope = s;
        }
        bool getInfinity(){
            return infinity;
        }
        bool getZero(){
            return zero;
        }
        double getSlope(){
            return slope;
        }
        double getIntercept(){
            return b;
        }
        void getEquation(){
            if(first.getX() == second.getX()){
                infinity = true;
                zero = false;
                slope = 0;
                b = first.getX();
            }else if(first.getY() == second.getY()){
                infinity = false;
                zero = true;
                slope = 0;
                b = first.getY();
            }else{
                infinity = false;
                zero = false;
                slope = (second.getY() - first.getY()) / (second.getX() - first.getX());
                b = findIntercept();
            }
        }   
        double findIntercept(){
            infinity = false;
            zero = false;
            return slope * -1 * first.getX() + first.getY();
        }
};

void illuminate(int i, int j){
    if(i >= 0 && i < len && j >= 0 && j < len){
        ppm[j][i*3] = 0;
        ppm[j][i*3+1] = 0;
        ppm[j][i*3+2] = 0;
    }
}

void bresenham(int x1, int y1, int x2, int y2){
    if(abs(x2-x1) >= abs(y2-y1) && ((x2 >= x1 && y2 >= y1) || (x1 >= x2 && y1 >= y2))){
        if(x1 >= x2 && y1 >= y2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int deltaX = x2 - x1;
        int deltaY = y2 - y1;
        int j = y1;
        int deltaE = deltaY - deltaX;
        for(int i = x1; i <= x2; i++){
            illuminate(i, j);
            if(deltaE >= 0){
                j++;
                deltaE -= deltaX;
            }
            deltaE += deltaY;
        }
    }else if(abs(x2-x1) >= abs(y2-y1) && ((x1 >= x2 && y2 >= y1) || (x2 >= x1 && y1 >= y2))){
        if(x2 >= x1 && y1 >= y2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int deltaX = x1 - x2;
        int deltaY = y2 - y1;
        int j = y2;
        int deltaE = deltaY - deltaX;
        for(int i = x2; i <= x1; i++){
            illuminate(i, j);
            if(deltaE >= 0){
                j--;
                deltaE -= deltaX;
            }
            deltaE += deltaY;
        }
    }
    else if(abs(y2 - y1) >= abs(x2 - x1) && ((x2 >= x1 && y2 >= y1) || (x1 >= x2 && y1 >= y2))){
        if(x1 >= x2 && y1 >= y2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int deltaX = x2-x1;
        int deltaY = y2-y1;
        int deltaE = deltaX - deltaY;
        int j = x1;
        for(int i = y1; i <= y2; i++){
            illuminate(j, i);
            if(deltaE >= 0){
                j++;
                deltaE -= deltaY;
            }
            deltaE += deltaX;
        }
    }else if(abs(y2 - y1) >= abs(x2 - x1) && ((x1 >= x2 && y2 >= y1) || (x2 >= x1 && y1 >= y2))){
        if(x2 >= x1 && y1 >= y2){
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int deltaX = x1-x2;
        int deltaY = y2-y1;
        int deltaE = deltaX - deltaY;
        int j = x1;
        for(int i = y1; i <= y2; i++){
            illuminate(j, i);
            if(deltaE >= 0){
                j--;
                deltaE -= deltaY;
            }
            deltaE += deltaX;
        }
    }
}

void drawCircle(double centerX, double centerY, double radius){
    double x, y, xmax, y2, y2_new, ty;
    centerX = centerX * len;
    centerY = centerY * len;
    xmax = (radius * 0.70710678);
    y = radius;
    y2 = y * y;
    ty = (2 * y) - 1; 
    y2_new = y2;
    for (x = 0; x <= xmax+1; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        illuminate((int)(centerX + x), (int)(centerY + y));
        illuminate((int)(centerX + x), (int)(centerY + -y));
        illuminate((int)(centerX + -x), (int)(centerY + y));
        illuminate((int)(centerX + -x), (int)(centerY + -y));
        illuminate((int)(centerX + y), (int)(centerY + x));
        illuminate((int)(centerX + y), (int)(centerY + -x));
        illuminate((int)(centerX + -y), (int)(centerY + x));
        illuminate((int)(centerX + -y), (int)(centerY + -x));
        y2_new -= (2 * x) - 3;
    }
}

void convertPPMToFile(){
    ofstream myfile("grahamscan.ppm");
    myfile << "P3 " << len << " " << len << " 1" << endl;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            myfile << ppm[i][j*3] << " " << ppm[i][j*3+1] << " " << ppm[i][j*3+2] << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

double distance(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

bool isInTriangle(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy){
    double sign1 = (dx - bx) * (ay - by) - (ax - bx) * (dy - by);
    double sign2 = (dx - cx) * (by - cy) - (bx - cx) * (dy - cy);
    double sign3 = (dx - ax) * (cy - ay) - (cx - ax) * (dy - ay);
    bool hasNegative = (sign1 < 0) || (sign2 < 0) || (sign3 < 0);
    bool hasPositive = (sign1 > 0) || (sign2 > 0) || (sign3 > 0);
    return !(hasNegative && hasPositive);
}

void findHull(vector<Point> points, Point P, Point Q){
    if(points.size() == 0){
        Line side;
        int startX = (int)(P.getX() * len);
        int startY = (int)(P.getY() * len);
        int endX = (int)(Q.getX() * len);
        int endY = (int)(Q.getY() * len);
        bresenham(startX, startY, endX, endY);
    }else{
        Line divide;
        divide.setFirst(P);
        divide.setSecond(Q);
        divide.getEquation();
        double divideB = divide.getIntercept();
        Point farthest;
        double maxDist = 0;
        for(int i = 0; i < points.size(); i++){
            Line perp;
            perp.setSlope(-1 / divide.getSlope());
            perp.setFirst(points[i]);
            double poiX = (perp.findIntercept() - divideB) / (divide.getSlope() - perp.getSlope());
            double poiY = perp.getSlope() * poiX + perp.findIntercept();
            double dist = distance(poiX, poiY, points[i].getX(), points[i].getY());
            if(dist > maxDist){
                maxDist = dist;
                farthest.setX(points[i].getX());
                farthest.setY(points[i].getY());
            }
        }
        //bresenham(divide);
        //drawCircle(farthest.getX(), farthest.getY(), 10);
        Line side1;
        side1.setFirst(P);
        side1.setSecond(farthest);
        side1.getEquation();
        Line side2;
        side2.setFirst(farthest);
        side2.setSecond(Q);
        side2.getEquation();
        double poiX = (side1.findIntercept() - side2.findIntercept()) / (side2.getSlope() - side1.getSlope());
        vector<Point> s1;
        vector<Point> s2;
        vector<Point> s;
        for(int i = 0; i < points.size(); i++){
            if(!isInTriangle(P.getX(), P.getY(), Q.getX(), Q.getY(), farthest.getX(), farthest.getY(), points[i].getX(), points[i].getY())){
                s.pb(points[i]);
            }
        }
        for(int i = 0; i < s.size(); i++){
            if(s[i].getX() < poiX){
                s1.pb(s[i]);
                //drawCircle(s[i].getX(), s[i].getY(), 8);
            }else{
                s2.pb(s[i]);
                //drawCircle(s[i].getX(), s[i].getY(), 13);
            }
        }
        //bresenham(side1);
        //bresenham(side2);
        findHull(s1, P, farthest);
        findHull(s2, farthest, Q);
    }
}

void part1(vector<Point> points){
    vector<Point> convexHull;
    Point leftmost;
    leftmost.setX(1.1);
    leftmost.setY(1.1);
    Point rightmost;
    rightmost.setX(-.1);
    rightmost.setY(-.1);
    for(int i = 0; i < numPoints; i++){
        Point temp = points[i];
        if(temp.getX() > rightmost.getX()){
            rightmost = temp;
        }
        if(temp.getX() < leftmost.getX()){
            leftmost = temp;
        }
    }
    Line divide;
    divide.setFirst(leftmost);
    divide.setSecond(rightmost);
    divide.getEquation();
    double divideB = divide.getIntercept();
    double divideSlope = divide.getSlope();
    vector<Point> s1;
    vector<Point> s2;
    drawCircle(leftmost.getX(), leftmost.getY(), 3);
    drawCircle(rightmost.getX(), rightmost.getY(), 3);
    //bresenham(divide);
    for(int i = 0; i < numPoints; i++){
        if(points[i].getX() != leftmost.getX() && points[i].getX() != rightmost.getX()){
            double yhat = divideSlope * points[i].getX() + divideB;
            if(yhat < points[i].getY()){
                s1.pb(points[i]);
                drawCircle(points[i].getX(), points[i].getY(), 3);
            }else{
                s2.pb(points[i]);
                drawCircle(points[i].getX(), points[i].getY(), 3);
            }
        }
    }
    //cout << leftmost.getX() << " " << leftmost.getY() << " " << rightmost.getX() << " " << rightmost.getY() << "\n";
    findHull(s1, leftmost, rightmost);
    findHull(s2, leftmost, rightmost);
}

bool cmpAngle(Point a, Point b){
    return a.getAngle() < b.getAngle();
}

double crossProduct(Point a, Point b, Point c){
    return (b.getX() - a.getX())*(c.getY() - b.getY()) - (b.getY() - a.getY())*(c.getX() - b.getX());
}

void part2(vector<Point> points){
    Point min;
    min.setX(1.5);
    min.setY(1.5);
    stack<Point> stackP;
    for(int i = 0; i < numPoints; i++){
        if(points[i].getY() < min.getY()){
            min.setX(points[i].getX());
            min.setY(points[i].getY());
        }else if(points[i].getY() == min.getY() && points[i].getX() < min.getX()){
            min.setX(points[i].getX());
            min.setY(points[i].getY());
        }
    }
    for(int i = 0; i < numPoints; i++){
        if(points[i].getX() != min.getX() || points[i].getY() != min.getY()){
            double x = points[i].getX() - min.getX();
            double y = points[i].getY() - min.getY();
            double angle = atan(y / x);
            if(angle < 0){
                points[i].setAngle(angle + M_PI);
            }else{
                points[i].setAngle(angle);
            }
        }else{
            points[i].setAngle(0);
        }
    }
    sort(points.begin(), points.end(), cmpAngle);
    stackP.push(points[0]);
    stackP.push(points[1]);
    for(int i = 2; i < points.size(); i++){
        Point current = points[i];
        while(stackP.size() >= 2){
            Point topP = stackP.top();
            stackP.pop();
            Point prev = stackP.top();
            double cross = crossProduct(prev, topP, current);
            if(cross > 0){
                stackP.push(topP);
                break;
            }
        }
        stackP.push(current);
    }
    const int siz = stackP.size();
    Point first = stackP.top();
    for(int i = 0; i < siz-1; i++){
        Point curr = stackP.top();
        stackP.pop();
        Point prev = stackP.top();
        //cout << curr.getX() << " " << curr.getY() << "\n";
        int x1 = (int)(curr.getX() * len);
        int y1 = (int)(curr.getY() * len);
        int x2 = (int)(prev.getX() * len);
        int y2 = (int)(prev.getY() * len);
        bresenham(x1, y1, x2, y2);
    }  
    Point last = stackP.top();
    int x1 = (int)(first.getX() * len);
    int y1 = (int)(first.getY() * len);
    int x2 = (int)(last.getX() * len);
    int y2 = (int)(last.getY() * len);
    bresenham(x1, y1, x2, y2);
}

int main(){
    srand(time(0));
    ppm.resize(len, vector<int>(len*3, 1));
    ofstream myfile("points.txt");
    myfile << setprecision(20);
    myfile << fixed;
    vector<Point> points;
    for(int i = 0; i < numPoints; i++){
        Point temp;
        temp.setX((double) rand() / RAND_MAX);
        temp.setY((double) rand() / RAND_MAX);
        points.pb(temp);
        myfile << temp.getX() << " " << temp.getY() << "\n";
        drawCircle(temp.getX(), temp.getY(), 3);
    }
    myfile.close();
    //part1(points);
    part2(points);
    convertPPMToFile();
}