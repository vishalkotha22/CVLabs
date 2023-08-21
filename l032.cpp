#include<chrono>
#include<bits/stdc++.h>
using namespace std;
#define pb push_back

const int len = 800;
const int numPoints = 1400;
vector<vector<int>> ppm;
ofstream myfile("results.txt");

double distance(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

class Point{
    public:
        double x, y;
};

class Segment{
    public:
        Point a, b;
        double dist;
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
    if(a.x == b.x){
        return a.y < b.y;
    }
    return a.x < b.x;
}

Segment recur(vector<Point> points){
    if(points.size() == 2){
        Segment temp;
        temp.a = points[0];
        temp.b = points[1];
        temp.dist = distance(points[0].x, points[0].y, points[1].x, points[1].y);
        return temp;
    }else if(points.size() == 3){
        Segment temp1;
        temp1.a = points[0];
        temp1.b = points[1];
        temp1.dist = distance(points[0].x, points[0].y, points[1].x, points[1].y);
        Segment temp2;
        temp2.a = points[0];
        temp2.b = points[2];
        temp2.dist = distance(points[0].x, points[0].y, points[2].x, points[2].y);
        Segment temp3;
        temp3.a = points[1];
        temp3.b = points[2];
        temp3.dist = distance(points[1].x, points[1].y, points[2].x, points[2].y);
        if(temp1.dist <= temp2.dist && temp1.dist <= temp3.dist){
            return temp1;
        }else if(temp2.dist <= temp1.dist && temp2.dist <= temp3.dist){
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
        double d = min(left.dist, right.dist);
        double divide = (first[first.size()-1].x + second[0].x) / 2;
        vector<Point> closeToBound;
        for(int i = first.size()-1; i >= 0; i--){
            if(divide - first[i].x < d){
                closeToBound.pb(first[i]);
            }else{
                break;
            }
        }
        for(int j = 0; j < second.size(); j++){
            if(second[j].x - divide < d){
                closeToBound.pb(second[j]);
            }else{
                break;
            }
        }
        Segment temp;
        temp.a.x = 0;
        temp.a.y = 0;
        temp.b.x = 1;
        temp.b.y = 1;
        temp.dist = 1.5;
        for(int a = 0; a < closeToBound.size(); a++){
            for(int b = a+1; b < closeToBound.size(); b++){
                if(distance(closeToBound[a].x, closeToBound[a].y, closeToBound[b].x, closeToBound[b].y) < temp.dist){
                    temp.a = closeToBound[a];
                    temp.b = closeToBound[b];
                    temp.dist = distance(closeToBound[a].x, closeToBound[a].y, closeToBound[b].x, closeToBound[b].y);
                }
            }
        }
        if(temp.dist <= left.dist && temp.dist <= right.dist){
            return temp;
        }else if(left.dist <= temp.dist && left.dist <= right.dist){
            return left;
        }else{
            return right;
        }
    }
}

void part2(){
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<Point> points;
    ifstream file("points.txt");
    for(int i = 0; i < numPoints; i++){
        Point temp;
        string s1;
        file >> s1;
        temp.x = stod(s1);
        string s2;
        file >> s2;
        temp.y = stod(s2);
        points.pb(temp);
        drawCircle(temp.x, temp.y, 3, true);
    }
    sort(points.begin(), points.end(), cmp);
    Segment segment = recur(points);
    segment.dist = distance(segment.a.x, segment.a.y, segment.b.x, segment.b.y);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << segment.a.x << " " << segment.a.y << " " << segment.b.x << " " << segment.b.y << " " << segment.dist << "\n";
    myfile << segment.a.x << " " << segment.a.y << " " << segment.b.x << " " << segment.b.y << " " << segment.dist << "\n";
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    myfile << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    drawCircle(segment.a.x, segment.a.y, 2, false);
    drawCircle(segment.a.x, segment.a.y, 3, false);
    drawCircle(segment.b.x, segment.b.y, 2, false);
    drawCircle(segment.b.x, segment.b.y, 3, false);
    convertPPMToFile();
    myfile.close();
}

void part1(){
    srand(time(0));
    ppm.resize(len, vector<int>(len*3, 1));
    list<Point> points;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ofstream pointsfile("points.txt");
    for(int i = 0; i < numPoints; i++){
        Point temp;
        temp.x = (double) rand() / RAND_MAX;
        temp.y = (double) rand() / RAND_MAX;
        pointsfile << temp.x << "  " << temp.y << "\n";
        //drawCircle(temp.x, temp.y, 3, true);
        points.pb(temp);
    }
    Segment segment;
    Point origin;
    origin.x = 0;
    origin.y = 0;
    segment.a = origin;
    Point farthest;
    farthest.x = 1;
    farthest.y = 1;
    segment.b = farthest;
    segment.dist = 1;
    list<Point>::iterator i;
    list<Point>::iterator j;
    for(i = points.begin(); i != points.end(); i++){
        for(j = next(i); j != points.end(); j++){
            double dist = distance((*i).x, (*i).y, (*j).x, (*j).y);
            if(dist < segment.dist){
                segment.dist = dist;
                segment.a = *i;
                segment.b = *j;
            }
        }
    }
    //drawCircle(segment.a.x, segment.a.y, 3, false);
    //drawCircle(segment.a.x, segment.a.y, 2, false);
    //drawCircle(segment.b.x, segment.b.y, 3, false);
    //drawCircle(segment.b.x, segment.b.y, 2, false);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << segment.a.x << " " << segment.a.y << " " << segment.b.x << " " << segment.b.y << " " << segment.dist << "\n";
    myfile << segment.a.x << " " << segment.a.y << " " << segment.b.x << " " << segment.b.y << " " << segment.dist << "\n";
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    myfile << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << "\n";
    //convertPPMToFile();
}

int main(){
    part1();
    part2();
}