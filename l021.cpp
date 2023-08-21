#include <bits/stdc++.h>
using namespace std;
#define pb push_back

const int len = 800;
vector<vector<int>> ppm;

double distance(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

class Line{
    public:
        double x1;
        double y1;
        double x2;
        double y2;
        bool infinity;
        bool zero;
        double slope;
        double b;
    void getEquation(){
        if(x1 == x2){
            infinity = true;
            zero = false;
            slope = 0;
            b = x1;
        }else if(y1 == y2){
            infinity = false;
            zero = true;
            slope = 0;
            b = y1;
        }else{
            infinity = false;
            zero = false;
            slope = (y2 - y1) / (x2 - x1);
            b = findIntercept();
        }
    }   
    double findIntercept(){
        infinity = false;
        zero = false;
        return slope * -1 * x1 + y1;
    }
};

class Square{
    public:
        Line line1, line2, line3, line4;
        double x1, y1, x2, y2, x3, y3, x4, y4;
    void findCorners(){
        pair<double, double> corner1 = findIntersection(line1, line2);
        pair<double, double> corner2 = findIntersection(line1, line3);
        pair<double, double> corner3 = findIntersection(line2, line4);
        pair<double, double> corner4 = findIntersection(line3, line4);
        x1 = corner1.first;
        y1 = corner1.second;
        x2 = corner2.first;
        y2 = corner2.second;
        x3 = corner3.first;
        y3 = corner3.second;
        x4 = corner4.first;
        y4 = corner4.second;
    }
    pair<double, double> findIntersection(Line l1, Line l2){
        double left = l1.slope - l2.slope;
        double right = l2.b - l1.b;
        double x = right / left;
        double y = l1.slope * x + l1.b;
        pair<double, double> intersection;
        intersection.first = x;
        intersection.second = y;
        return intersection;
    }
};

bool isInTriangle(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy){
    double sign1 = (dx - bx) * (ay - by) - (ax - bx) * (dy - by);
    double sign2 = (dx - cx) * (by - cy) - (bx - cx) * (dy - cy);
    double sign3 = (dx - ax) * (cy - ay) - (cx - ax) * (dy - ay);
    bool hasNegative = (sign1 < 0) || (sign2 < 0) || (sign3 < 0);
    bool hasPositive = (sign1 > 0) || (sign2 > 0) || (sign3 > 0);
    return !(hasNegative && hasPositive);
}

void illuminate(int i, int j){
    if(i >= 0 && i < len && j >= 0 && j < len){
        ppm[j][i*3] = 0;
        ppm[j][i*3+1] = 0;
        ppm[j][i*3+2] = 0;
    }
}

void drawCircle(double centerX, double centerY, double radius){
    double x, y, xmax, y2, y2_new, ty;
    centerX = centerX * 800;
    centerY = centerY * 800;
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

void bresenham(Line line){
    int x1 = 0;
    int y1 = (int) (line.b * len);
    int x2 = len;
    int y2 = (int)(len * line.slope + y1);
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

void convertPPMToFile(){
    ofstream myfile("output.ppm");
    myfile << "P3 " << len << " " << len << " 1" << endl;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            myfile << ppm[i][j*3] << " " << ppm[i][j*3+1] << " " << ppm[i][j*3+2] << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

vector<double> part1(){
    srand(time(0));

    double ax = (double) rand() / RAND_MAX;
    double ay = (double) rand() / RAND_MAX;
    double bx = (double) rand() / RAND_MAX;
    double by = (double) rand() / RAND_MAX;
    double cx = (double) rand() / RAND_MAX;
    double cy = (double) rand() / RAND_MAX;

    double dx = (double) rand() / RAND_MAX;
    double dy = (double) rand() / RAND_MAX;
    while(isInTriangle(ax, ay, bx, by, cx, cy, dx, dy) || isInTriangle(dx, dy, bx, by, cx, cy, ax, ay) || isInTriangle(ax, ay, dx, dy, cx, cy, bx, by) || isInTriangle(ax, ay, bx, by, dx, dy, cx, cy)){
        dx = (double) rand() / RAND_MAX;
        dy = (double) rand() / RAND_MAX;
    }

    vector<double> points;
    points.pb(ax);
    points.pb(ay);
    points.pb(bx);
    points.pb(by);
    points.pb(cx);
    points.pb(cy);
    points.pb(dx);
    points.pb(dy);
    
    ofstream myfile("points.txt");

    myfile << setprecision(17);
    myfile << fixed;

    for(int i = 0; i < 8; i+=2){
        cout << points[i] << " " << points[i+1] << endl;
        if(i < 6){
            myfile << "(" << points[i] << "," << points[i+1] << ") , "; 
        }else{
            myfile << "(" << points[i] << "," << points[i+1] << ")";
        }
    }
    myfile.close();
    return points;
}

vector<Line> createSquare(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy){
    vector<Line> lines;
    if(ax == bx || ax == cx || ax == dx){
        ax += 0.000000000000000001;
    }
    if(bx == cx || bx == dx){
        bx += 0.000000000000000001;
    }
    if(cx == dx){
        cx += 0.000000000000000001;
    }
    if(ay == by || ay == cy || ay == dy){
        ay += 0.000000000000000001;
    }
    if(by == cy || by == dy){
        by += 0.000000000000000001;
    }
    if(cy == dy){
        cy += 0.000000000000000001;
    }
    //Step 1
    Line line1;
    line1.x1 = ax;
    line1.y1 = ay;
    line1.x2 = bx;
    line1.y2 = by;
    line1.getEquation();
    //Step 2
    Line line2;
    line2.x1 = cx;
    line2.y1 = cy;
    line2.slope = -1 / line1.slope;
    line2.b = line2.findIntercept();
    //Step 3
    double abDist = distance(ax, ay, bx, by);
    double lineHyp = sqrt(line2.slope * line2.slope + 1);
    Line line3;
    line3.x1 = cx;
    line3.y1 = cy;
    line3.x2 = cx + (1 / lineHyp) * abDist;
    line3.y2 = cy + (line2.slope / lineHyp) * abDist;
    line3.getEquation();
    //Step 4
    Line line4;
    line4.x1 = line3.x2;
    line4.y1 = line3.y2;
    line4.x2 = dx;
    line4.y2 = dy;
    line4.getEquation();
    //Step 5
    Line line5;
    line5.x1 = ax;
    line5.y1 = ay;
    line5.slope = -1 / line4.slope;
    line5.b = line5.findIntercept();
    Line line6;
    line6.x1 = bx;
    line6.y1 = by;
    line6.slope = line5.slope;
    line6.b = line6.findIntercept();
    //Step 6
    Line line7;
    line7.x1 = cx;
    line7.y1 = cy;
    line7.slope = line4.slope;
    line7.b = line7.findIntercept();
    lines.pb(line4);
    lines.pb(line5);
    lines.pb(line6);
    lines.pb(line7);
    return lines;
}

Square init(Square s, vector<Line> lines){
    s.line1 = lines[0];
    s.line2 = lines[1];
    s.line3 = lines[2];
    s.line4 = lines[3];
    return s;
}

void drawSquare(Square s){
    bresenham(s.line1);
    bresenham(s.line2);
    bresenham(s.line3);
    bresenham(s.line4);
    drawCircle(s.x1, s.y1, 2);
    drawCircle(s.x2, s.y2, 2);
    drawCircle(s.x3, s.y3, 2);
    drawCircle(s.x4, s.y4, 2);
}

void part2(){
    ppm.resize(len, vector<int>(len*3, 1));
    ifstream myfile("points.txt");
    string point1, point2, point3, point4, comma;
    myfile >> point1;
    myfile >> comma;
    myfile >> point2;
    myfile >> comma;
    myfile >> point3;
    myfile >> comma;
    myfile >> point4;
    point1.erase(0, 1);
    point2.erase(0, 1);
    point3.erase(0, 1);
    point4.erase(0, 1);
    point1.erase(point1.length()-1, 1);
    point2.erase(point2.length()-1, 1);
    point3.erase(point3.length()-1, 1);
    point4.erase(point4.length()-1, 1);
    double ax, ay, bx, by, cx, cy, dx, dy;
    int point1index, point2index, point3index, point4index;
    point1index = point1.find(',');
    point2index = point2.find(',');
    point3index = point3.find(',');
    point4index = point4.find(',');
    ax = stod(point1.substr(0, point1index));
    ay = stod(point1.substr(point1index+1, (point1.length() - point1index - 1)));
    bx = stod(point2.substr(0, point2index));
    by = stod(point2.substr(point2index+1, (point2.length() - point2index - 1)));
    cx = stod(point3.substr(0, point3index));
    cy = stod(point3.substr(point3index+1, (point3.length() - point3index - 1)));
    dx = stod(point4.substr(0, point4index));
    dy = stod(point4.substr(point4index+1, (point4.length() - point4index - 1)));
    vector<Line> s1 = createSquare(ax, ay, bx, by, cx, cy, dx, dy);
    vector<Line> s2 = createSquare(ax, ay, bx, by, dx, dy, cx, cy);
    vector<Line> s3 = createSquare(ax, ay, cx, cy, bx, by, dx, dy);
    vector<Line> s4 = createSquare(ax, ay, cx, cy, dx, dy, bx, by);
    vector<Line> s5 = createSquare(ax, ay, dx, dy, bx, by, cx, cy);
    vector<Line> s6 = createSquare(ax, ay, dx, dy, cx, cy, bx, by);
    Square square1, square2, square3, square4, square5, square6;
    square1 = init(square1, s1);
    square2 = init(square2, s2);
    square3 = init(square3, s3);
    square4 = init(square4, s4);
    square5 = init(square5, s5);
    square6 = init(square6, s6);
    square1.findCorners();
    square2.findCorners();
    square3.findCorners();
    square4.findCorners();
    square5.findCorners();
    square6.findCorners();
    double square1Distance = distance(square1.x1, square1.y1, square1.x2, square1.y2);
    double square2Distance = distance(square2.x1, square2.y1, square2.x2, square2.y2);
    double square3Distance = distance(square3.x1, square3.y1, square3.x2, square3.y2);
    double square4Distance = distance(square4.x1, square4.y1, square4.x2, square4.y2);
    double square5Distance = distance(square5.x1, square5.y1, square5.x2, square5.y2);
    double square6Distance = distance(square6.x1, square6.y1, square6.x2, square6.y2);
    vector<double> sizes;
    sizes.pb(square1Distance);
    sizes.pb(square2Distance);
    sizes.pb(square3Distance);
    sizes.pb(square4Distance);
    sizes.pb(square5Distance);
    sizes.pb(square6Distance);
    double minLength = square1Distance;
    double minIndex = -1;
    for(int i = 0; i < sizes.size(); i++){
        if(minLength >= sizes[i]){
            minLength = sizes[i];
            minIndex = i;
        }
    }
    if(minIndex == 0){
        drawSquare(square1);
    }else if(minIndex == 1){
        drawSquare(square2);
    }else if(minIndex == 2){
        drawSquare(square3);
    }else if(minIndex == 3){
        drawSquare(square4);
    }else if(minIndex == 4){
        drawSquare(square5);
    }else{
        drawSquare(square6);
    }
    drawCircle(ax, ay, 2);
    drawCircle(bx, by, 2);
    drawCircle(cx, cy, 2);
    drawCircle(dx, dy, 2);
    convertPPMToFile();
    ofstream file("output.txt");
    file << setprecision(17);
    file << fixed;
    file << "(" << ax << ", " << ay << ") , (" << bx << ", " << by << ") , (" << cx << ", " << cy << ") , (" << dx << ", " << dy << ")" << "\n";
    file << "(" << square1.x1 << ", " << square1.y1 << ") , (" << square1.x2 << ", " << square1.y2 << ") , (" << square1.x3 << ", " << square1.y3 << ") , (" << square1.x4 << ", " << square1.y4 << ") Area=" << (square1Distance * square1Distance) << "\n";
    file << "(" << square2.x1 << ", " << square2.y1 << ") , (" << square2.x2 << ", " << square2.y2 << ") , (" << square2.x3 << ", " << square2.y3 << ") , (" << square2.x4 << ", " << square2.y4 << ") Area=" << (square2Distance * square2Distance) << "\n";
    file << "(" << square3.x1 << ", " << square3.y1 << ") , (" << square3.x2 << ", " << square3.y2 << ") , (" << square3.x3 << ", " << square3.y3 << ") , (" << square3.x4 << ", " << square3.y4 << ") Area=" << (square3Distance * square3Distance) << "\n";
    file << "(" << square4.x1 << ", " << square4.y1 << ") , (" << square4.x2 << ", " << square4.y2 << ") , (" << square4.x3 << ", " << square4.y3 << ") , (" << square4.x4 << ", " << square4.y4 << ") Area=" << (square4Distance * square4Distance) << "\n";
    file << "(" << square5.x1 << ", " << square5.y1 << ") , (" << square5.x2 << ", " << square5.y2 << ") , (" << square5.x3 << ", " << square5.y3 << ") , (" << square5.x4 << ", " << square5.y4 << ") Area=" << (square5Distance * square5Distance) << "\n";
    file << "(" << square6.x1 << ", " << square6.y1 << ") , (" << square6.x2 << ", " << square6.y2 << ") , (" << square6.x3 << ", " << square6.y3 << ") , (" << square6.x4 << ", " << square6.y4 << ") Area=" << (square6Distance * square6Distance) << "\n";
    file.close();
}

int main(){
    //vector<double> points = part1();
    part2();
}