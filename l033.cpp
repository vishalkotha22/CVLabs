#include<chrono>
#include<bits/stdc++.h>
using namespace std;
#define pb push_back

const int len = 800;
const int numPoints = 1000000;
vector<vector<int>> ppm;
ofstream myfile("results.txt");

double distance(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

class Point{
    private:
        double x, y;
    public:
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
        void setX(double coord){
            x = coord;
        }
        void setY(double coord){
            y = coord;
        }
};

class Segment{
    private:
        Point a, b;
        double dist;
    public:
        Point getA(){
            return a;
        }
        Point getB(){
            return b;
        }
        double getDist(){
            return dist;
        }
        void setA(Point first){
            a = first;
        }
        void setB(Point second){
            b = second;
        }
        void setDist(double distance){
            dist = distance;
        }
};

void illuminate(int i, int j){
    if(i >= 0 && i < len && j >= 0 && j < len){
        ppm[j][i*3] = 0;
        ppm[j][i*3+1] = 0;
        ppm[j][i*3+2] = 0;
    }
}

void illuminateRed(int i, int j){
    if(i >= 0 && i < len && j >= 0 && j < len){
        ppm[j][i*3] = 1;
        ppm[j][i*3+1] = 0;
        ppm[j][i*3+2] = 0;
    }
}

void drawCircle(double centerX, double centerY, double radius, bool plain){
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
        if(plain){
            illuminate((int)(centerX + x), (int)(centerY + y));
            illuminate((int)(centerX + x), (int)(centerY + -y));
            illuminate((int)(centerX + -x), (int)(centerY + y));
            illuminate((int)(centerX + -x), (int)(centerY + -y));
            illuminate((int)(centerX + y), (int)(centerY + x));
            illuminate((int)(centerX + y), (int)(centerY + -x));
            illuminate((int)(centerX + -y), (int)(centerY + x));
            illuminate((int)(centerX + -y), (int)(centerY + -x));
        }else{
            illuminateRed((int)(centerX + x), (int)(centerY + y));
            illuminateRed((int)(centerX + x), (int)(centerY + -y));
            illuminateRed((int)(centerX + -x), (int)(centerY + y));
            illuminateRed((int)(centerX + -x), (int)(centerY + -y));
            illuminateRed((int)(centerX + y), (int)(centerY + x));
            illuminateRed((int)(centerX + y), (int)(centerY + -x));
            illuminateRed((int)(centerX + -y), (int)(centerY + x));
            illuminateRed((int)(centerX + -y), (int)(centerY + -x));          
        }
        y2_new -= (2 * x) - 3;
    }
}

void convertPPMToFile(){
    ofstream myfile("points.ppm");
    myfile << "P3 " << len << " " << len << " 1" << endl;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            myfile << ppm[i][j*3] << " " << ppm[i][j*3+1] << " " << ppm[i][j*3+2] << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

bool cmp(Point a, Point b){
    if(a.getX() == b.getX()){
        return a.getY() < b.getY();
    }
    return a.getX() < b.getX();
}

bool cmpY(Point a, Point b){
    return a.getY() < b.getY();
}

Segment recur2(vector<Point> points){
    if(points.size() == 2){
        Segment temp;
        temp.setA(points[0]);
        temp.setB(points[1]);
        temp.setDist(distance(points[0].getX(), points[0].getY(), points[1].getX(), points[1].getY()));
        return temp;
    }else if(points.size() == 3){
        Segment temp1;
        temp1.setA(points[0]);
        temp1.setB(points[1]);
        temp1.setDist(distance(points[0].getX(), points[0].getY(), points[1].getX(), points[1].getY()));
        Segment temp2;
        temp2.setA(points[0]);
        temp2.setB(points[2]);
        temp2.setDist(distance(points[0].getX(), points[0].getY(), points[2].getX(), points[2].getY()));
        Segment temp3;
        temp3.setA(points[1]);
        temp3.setB(points[2]);
        temp3.setDist(distance(points[1].getX(), points[1].getY(), points[2].getX(), points[2].getY()));
        if(temp1.getDist() <= temp2.getDist() && temp1.getDist() <= temp3.getDist()){
            return temp1;
        }else if(temp2.getDist() <= temp1.getDist() && temp2.getDist() <= temp3.getDist()){
            return temp2;
        }else{
            return temp3;
        }
    }else{
        int midpoint = points.size() / 2;
        vector<Point> first, second;
        for(int i = 0; i < midpoint; i++){
            first.pb(points[i]);
        }
        for(int j = midpoint; j < points.size(); j++){
            second.pb(points[j]);
        }
        Segment left = recur2(first);
        Segment right = recur2(second);
        double d = 3 * min(left.getDist(), right.getDist()) / 2;
        //double divide = (first[first.size()-1].getX() + second[0].getX()) / 2;
        double divide = points[midpoint].getX();
        vector<Point> vectorStrip;
        for(int i = 0; i <= first.size(); i++){
            if(divide - first[i].getX() <= d){
                vectorStrip.pb(first[i]);
            }else{
                break;
            }
        }
        for(int j = 0; j < second.size(); j++){
            if(second[j].getX() - divide <= d){
                vectorStrip.pb(second[j]);
            }else{
                break;
            }
        }
        sort(vectorStrip.begin(), vectorStrip.end(), cmpY);
        Segment temp;
        Point a;
        a.setX(0);
        a.setY(0);
        temp.setA(a);
        Point b;
        b.setX(1);
        b.setY(1);
        temp.setB(b);
        temp.setDist(1.5);
        for(int i = 0; i < vectorStrip.size(); i++){
            for(int j = i; j < vectorStrip.size() && j < i+16; j++){
                if(distance(vectorStrip[i].getX(), vectorStrip[i].getY(), vectorStrip[j].getX(), vectorStrip[j].getY()) < temp.getDist()){
                    if(distance(vectorStrip[i].getX(), vectorStrip[i].getY(), vectorStrip[j].getX(), vectorStrip[j].getY()) != 0){
                        temp.setA(vectorStrip[i]);
                        temp.setB(vectorStrip[j]);
                        temp.setDist(distance(vectorStrip[i].getX(), vectorStrip[i].getY(), vectorStrip[j].getX(), vectorStrip[j].getY()));
                    }
                }
            }
        }
        if(temp.getDist() <= left.getDist() && temp.getDist() <= right.getDist()){
            return temp;
        }else if(left.getDist() <= temp.getDist() && left.getDist() <= right.getDist()){
            return left;
        }else{
            return right;
        }
    }
}

void part3(){
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<Point> points;
    ifstream file("points1m.txt");
    string line;
    if(file.is_open()){
        while(getline(file, line, '\n')){
            int pos = line.find(" ");
            double x = stod(line.substr(0, pos));
            double y = stod(line.substr(pos));
            Point temp;
            temp.setX(x);
            temp.setY(y);
            points.pb(temp);
        }
    }
    sort(points.begin(), points.end(), cmp);
    Segment segment = recur2(points);
    segment.setDist(distance(segment.getA().getX(), segment.getA().getY(), segment.getB().getX(), segment.getB().getY()));
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << segment.getA().getX() << " " << segment.getA().getY() << " " << segment.getB().getX() << " " << segment.getB().getY() << " " << segment.getDist() << "\n";
    myfile << segment.getA().getX() << " " << segment.getA().getY() << " " << segment.getB().getX() << " " << segment.getB().getY() << " " << segment.getDist() << "\n";
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    myfile << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    //drawCircle(segment.getA().getX(), segment.getA().getY(), 2, false);
    //drawCircle(segment.getA().getX(), segment.getA().getY(), 3, false);
    //drawCircle(segment.getB().getX(), segment.getB().getY(), 2, false);
    //drawCircle(segment.getB().getX(), segment.getB().getY(), 3, false);
    convertPPMToFile();
    myfile.close();
}

Segment recur(vector<Point> points){
    if(points.size() == 2){
        Segment temp;
        temp.setA(points[0]);
        temp.setB(points[1]);
        temp.setDist(distance(points[0].getX(), points[0].getY(), points[1].getX(), points[1].getY()));
        return temp;
    }else if(points.size() == 3){
        Segment temp1;
        temp1.setA(points[0]);
        temp1.setB(points[1]);
        temp1.setDist(distance(points[0].getX(), points[0].getY(), points[1].getX(), points[1].getY()));
        Segment temp2;
        temp2.setA(points[0]);
        temp2.setB(points[2]);
        temp2.setDist(distance(points[0].getX(), points[0].getY(), points[2].getX(), points[2].getY()));
        Segment temp3;
        temp3.setA(points[1]);
        temp3.setB(points[2]);
        temp3.setDist(distance(points[1].getX(), points[1].getY(), points[2].getX(), points[2].getY()));
        if(temp1.getDist() <= temp2.getDist() && temp1.getDist() <= temp3.getDist()){
            return temp1;
        }else if(temp2.getDist() <= temp1.getDist() && temp2.getDist() <= temp3.getDist()){
            return temp2;
        }else{
            return temp3;
        }
    }else{
        int midpoint = points.size() / 2;
        vector<Point> first, second;
        for(int i = 0; i < midpoint; i++){
            first.pb(points[i]);
        }
        for(int j = midpoint; j < points.size(); j++){
            second.pb(points[j]);
        }
        Segment left = recur(first);
        Segment right = recur(second);
        double d = 3 * min(left.getDist(), right.getDist()) / 2;
        double divide = (first[first.size()-1].getX() + second[0].getX()) / 2;
        //double divide = points[midpoint].getX();
        vector<Point> closeToBound;
        for(int i = first.size()-1; i >= 0; i--){
            if(divide - first[i].getX() < d){
                closeToBound.pb(first[i]);
            }else{
                break;
            }
        }
        for(int j = 0; j < second.size(); j++){
            if(second[j].getX() - divide < d){
                closeToBound.pb(second[j]);
            }else{
                break;
            }
        }
        Segment temp;
        temp.setDist(1.5);
        for(int a = 0; a < closeToBound.size(); a++){
            for(int b = a+1; b < closeToBound.size(); b++){
                if(distance(closeToBound[a].getX(), closeToBound[a].getY(), closeToBound[b].getX(), closeToBound[b].getY()) < temp.getDist()){
                    temp.setA(closeToBound[a]);
                    temp.setB(closeToBound[b]);
                    temp.setDist(distance(closeToBound[a].getX(), closeToBound[a].getY(), closeToBound[b].getX(), closeToBound[b].getY()));
                }
            }
        }
        if(temp.getDist() <= left.getDist() && temp.getDist() <= right.getDist()){
            return temp;
        }else if(left.getDist() <= temp.getDist() && left.getDist() <= right.getDist()){
            return left;
        }else{
            return right;
        }
    }
}

void part2(){
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<Point> points;
    ifstream file("points1m.txt");
    string line;
    if(file.is_open()){
        while(getline(file, line, '\n')){
            int pos = line.find(" ");
            double x = stod(line.substr(0, pos));
            double y = stod(line.substr(pos));
            Point temp;
            temp.setX(x);
            temp.setY(y);
            drawCircle(x, y, 3, true);
            points.pb(temp);
        }
    }
    sort(points.begin(), points.end(), cmp);
    Segment segment = recur(points);
    segment.setDist(distance(segment.getA().getX(), segment.getA().getY(), segment.getB().getX(), segment.getB().getY()));
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << setprecision(23);
    cout << fixed;
    myfile << setprecision(23);
    myfile << fixed;
    cout << segment.getA().getX() << " " << segment.getA().getY() << " " << segment.getB().getX() << " " << segment.getB().getY() << " " << segment.getDist() << "\n";
    myfile << segment.getA().getX() << " " << segment.getA().getY() << " " << segment.getB().getX() << " " << segment.getB().getY() << " " << segment.getDist() << "\n";
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    myfile << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    drawCircle(segment.getA().getX(), segment.getA().getY(), 2, false);
    drawCircle(segment.getA().getX(), segment.getA().getY(), 3, false);
    drawCircle(segment.getB().getX(), segment.getB().getY(), 2, false);
    drawCircle(segment.getB().getX(), segment.getB().getY(), 3, false);
    convertPPMToFile();
    myfile.close();
}

void part1(){
    srand(time(0));
    list<Point> points;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    //cout << setprecision(23);
    //cout << fixed;
    ofstream pointsfile("points.txt");
    for(int i = 0; i < numPoints; i++){
        Point temp;
        temp.setX((double) rand() / RAND_MAX);
        temp.setY((double) rand() / RAND_MAX);
        pointsfile << temp.getX() << "  " << temp.getY() << "\n";
        //drawCircle(temp.x, temp.y, 3, true);
        points.pb(temp);
    }
    /*ifstream myfile("points100k.txt"); 
    for(int i = 0; i < numPoints; i++){
        Point temp;
        string s1;
        myfile >> s1;
        temp.setX(stod(s1));
        string s2;
        myfile >> s2;
        temp.setY(stod(s2));
        points.pb(temp);
    }
    Segment segment;
    Point origin;
    origin.setX(0);
    origin.setY(0);
    segment.setA(origin);
    Point farthest;
    farthest.setX(1);
    farthest.setY(1);
    segment.setB(farthest);
    segment.setDist(1.5);
    list<Point>::iterator i;
    list<Point>::iterator j;
    for(i = points.begin(); i != points.end(); i++){
        for(j = next(i); j != points.end(); j++){
            double dist = distance((*i).getX(), (*i).getY(), (*j).getX(), (*j).getY());
            if(dist < segment.getDist()){
                segment.setDist(dist);
                segment.setA(*i);
                segment.setB(*j);
            }
        }
    }
    //drawCircle(segment.a.x, segment.a.y, 3, false);
    //drawCircle(segment.a.x, segment.a.y, 2, false);
    //drawCircle(segment.b.x, segment.b.y, 3, false);
    //drawCircle(segment.b.x, segment.b.y, 2, false);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << segment.getA().getX() << " " << segment.getA().getY() << " " << segment.getB().getX() << " " << segment.getB().getY() << " " << segment.getDist() << "\n";
    //myfile << segment.getA().getX() << " " << segment.getA().getY() << " " << segment.getB().getX() << " " << segment.getB().getY() << " " << segment.getDist() << "\n";
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    //myfile << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    convertPPMToFile();*/
}

int main(){
    ppm.resize(len, vector<int>(len*3, 1));
    part2();
    part3();
}