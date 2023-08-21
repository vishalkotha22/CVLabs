#include <iostream>
using namespace std;
#include <math.h>
#include <vector>
#include <fstream>

const int len = 800;
vector<vector<int>> ppm;

pair<double, double> findMidpoint(double ax, double ay, double bx, double by){
    pair<double, double> coords;
    coords.first = (ax+bx)/2;
    coords.second = (ay+by)/2;
    return coords;
}

pair<double, double> findIncenter(double ax, double ay, double bx, double by, double cx, double cy, double ab, double ac, double bc){
    double incenterX = (ax * bc + bx * ac + cx * ab) / (ab + ac + bc);
    double incenterY = (ay * bc + by * ac + cy * ab) / (ab + ac + bc);
    pair<double, double> coords;
    coords.first = incenterX;
    coords.second = incenterY;
    return coords;
}

pair<double, double> pointSlopeToSlope(double slope, double x, double y){
    double intercept = -slope*x + y;
    pair<double, double> coords;
    coords.first = slope;
    coords.second = intercept;
    return coords;
}

pair<double, double> findCircumcenter(double ax, double ay, double bx, double by, double cx, double cy){
    double abPerpendicularSlope = -1 * ((bx - ax) / (by - ay));
    double acPerpendicularSlope = -1 * ((cx - ax) / (cy - ay));
    double bcPerpendicularSlope = -1 * ((cx - bx) / (cy - by));
    if(!isinf(abPerpendicularSlope) && !isinf(acPerpendicularSlope)){
        pair<double, double> abMidpoint = findMidpoint(ax, ay, bx, by);
        pair<double, double> acMidpoint = findMidpoint(ax, ay, cx, cy);
        pair<double, double> abPerpendicularBisector = pointSlopeToSlope(abPerpendicularSlope, abMidpoint.first, abMidpoint.second);
        pair<double, double> acPerpendicularBisector = pointSlopeToSlope(acPerpendicularSlope, acMidpoint.first, acMidpoint.second);
        double leftSideEquation = abPerpendicularBisector.first - acPerpendicularBisector.first;
        double rightSideEquation = acPerpendicularBisector.second - abPerpendicularBisector.second;
        double circumcenterX = rightSideEquation / leftSideEquation;
        double circumcenterY = circumcenterX * abPerpendicularBisector.first + abPerpendicularBisector.second;
        pair<double, double> coords;
        coords.first = circumcenterX;
        coords.second = circumcenterY;
        return coords;
    }
    else if(!isinf(abPerpendicularSlope) && !isinf(bcPerpendicularSlope)){
        pair<double, double> abMidpoint = findMidpoint(ax, ay, bx, by);
        pair<double, double> bcMidpoint = findMidpoint(bx, by, cx, cy);
        pair<double, double> abPerpendicularBisector = pointSlopeToSlope(abPerpendicularSlope, abMidpoint.first, abMidpoint.second);
        pair<double, double> bcPerpendicularBisector = pointSlopeToSlope(bcPerpendicularSlope, bcMidpoint.first, bcMidpoint.second);
        double leftSideEquation = abPerpendicularBisector.first - bcPerpendicularBisector.first;
        double rightSideEquation = bcPerpendicularBisector.second - abPerpendicularBisector.second;
        double circumcenterX = rightSideEquation / leftSideEquation;
        double circumcenterY = circumcenterX * abPerpendicularBisector.first + abPerpendicularBisector.second;
        pair<double, double> coords;
        coords.first = circumcenterX;
        coords.second = circumcenterY;
        return coords;
    }
    else{
        pair<double, double> bcMidpoint = findMidpoint(bx, by, cx, cy);
        pair<double, double> acMidpoint = findMidpoint(ax, ay, cx, cy);
        pair<double, double> bcPerpendicularBisector = pointSlopeToSlope(bcPerpendicularSlope, bcMidpoint.first, bcMidpoint.second);
        pair<double, double> acPerpendicularBisector = pointSlopeToSlope(acPerpendicularSlope, acMidpoint.first, acMidpoint.second);
        double leftSideEquation = bcPerpendicularBisector.first - acPerpendicularBisector.first;
        double rightSideEquation = acPerpendicularBisector.second - bcPerpendicularBisector.second;
        double circumcenterX = rightSideEquation / leftSideEquation;
        double circumcenterY = circumcenterX * acPerpendicularBisector.first + acPerpendicularBisector.second;
        pair<double, double> coords;
        coords.first = circumcenterX;
        coords.second = circumcenterY;
        return coords;
    }
}

double findOutcircleRadius(double ab, double ac, double bc, double incircleRadius, double semiperimeter){
    return ab*ac*bc/(4*incircleRadius*semiperimeter);
}

double findIncircleRadius(double semiperimeter, double ab, double ac, double bc){
    return sqrt((semiperimeter-ab)*(semiperimeter-ac)*(semiperimeter-bc)/semiperimeter);
}

double findSideLength(double ax, double ay, double bx, double by){
    return sqrt((bx-ax)*(bx-ax)+(by-ay)*(by-ay));
}

double findPerimeter(double ab, double ac, double bc){
    return ab + ac + bc;
}

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

void drawCircle(int centerX, int centerY, double radius){
    int x, y, xmax, y2, y2_new, ty;
    xmax = (int)(radius * 0.70710678);
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
        illuminate(centerX + x, centerY + y);
        illuminate(centerX + x, centerY + -y);
        illuminate(centerX + -x, centerY + y);
        illuminate(centerX + -x, centerY + -y);
        illuminate(centerX + y, centerY + x);
        illuminate(centerX + y, centerY + -x);
        illuminate(centerX + -y, centerY + x);
        illuminate(centerX + -y, centerY + -x);
        y2_new -= (2 * x) - 3;
    }
}

void convertPPMToFile(){
    ofstream myfile("outfile.ppm");
    myfile << "P3 " << len << " " << len << " 1" << endl;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            myfile << ppm[i][j*3] << " " << ppm[i][j*3+1] << " " << ppm[i][j*3+2] << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

int main(){
    ppm.resize(len, vector<int>(len*3, 1));

    srand(time(0));

    double ax = (double) rand() / RAND_MAX;
    double ay = (double) rand() / RAND_MAX;
    double bx = (double) rand() / RAND_MAX;
    double by = (double) rand() / RAND_MAX;
    double cx = (double) rand() / RAND_MAX;
    double cy = (double) rand() / RAND_MAX;
    while(ax == bx == cx || ay == by == cy){
        ax = (double) rand() / RAND_MAX;
        ay = (double) rand() / RAND_MAX;
        bx = (double) rand() / RAND_MAX;
        by = (double) rand() / RAND_MAX;
        cx = (double) rand() / RAND_MAX;
        cy = (double) rand() / RAND_MAX;
    }

    double ab = findSideLength(ax, ay, bx, by);
    double ac = findSideLength(ax, ay, cx, cy);
    double bc = findSideLength(bx, by, cx, cy);

    double semiperimeter = findPerimeter(ab, ac, bc) / 2;

    double incircleRadius = findIncircleRadius(semiperimeter, ab, ac, bc);
    pair<double, double> incircleCenter = findIncenter(ax, ay, bx, by, cx, cy, ab, ac, bc);

    double outcircleRadius = findOutcircleRadius(ab, ac, bc, incircleRadius, semiperimeter);
    pair<double, double> outcircleCenter = findCircumcenter(ax, ay, bx, by, cx, cy);  

    double ninePointCircleRadius = outcircleRadius / 2;
    pair<double, double> abMidpoint = findMidpoint(ax, ay, bx, by);
    pair<double, double> acMidpoint = findMidpoint(ax, ay, cx, cy);
    pair<double, double> bcMidpoint = findMidpoint(bx, by, cx, cy);
    pair<double, double> ninePointCenter = findCircumcenter(abMidpoint.first, abMidpoint.second, acMidpoint.first, acMidpoint.second, bcMidpoint.first, bcMidpoint.second); 

    double eulerLineSlope = (outcircleCenter.second - ninePointCenter.second) / (outcircleCenter.first - ninePointCenter.first);
    pair<double, double> eulerLine = pointSlopeToSlope(eulerLineSlope, outcircleCenter.first, outcircleCenter.second);

    double eulerLineStartX = -eulerLine.second / eulerLineSlope;    
    double eulerLineEndX = (1 - eulerLine.second) / eulerLineSlope;    

    bresenham((int)(len * ax), (int)(len * ay), (int)(len * bx), (int)(len * by));
    bresenham((int)(len * ax), (int)(len * ay), (int)(len * cx), (int)(len * cy));
    bresenham((int)(len * bx), (int)(len * by), (int)(len * cx), (int)(len * cy));
    bresenham((int)(len*eulerLineStartX), 0, (int)(len*eulerLineEndX), len);
    drawCircle((int)(len * incircleCenter.first), (int)(len * incircleCenter.second), (int)(len * incircleRadius));
    drawCircle((int)(len * outcircleCenter.first), (int)(len * outcircleCenter.second), (int)(len * outcircleRadius));
    drawCircle((int)(len * ninePointCenter.first), (int)(len * ninePointCenter.second), (int)(len * ninePointCircleRadius));

    convertPPMToFile();
}