//Vishal Kotha, Period 3
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
vector<vector<int>> image;
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

void convertPPMToFile(vector<vector<int>> ppm){
    ofstream myfile("cannyedgedetect.ppm");
    myfile << "P3 " << ppm.size() << " " << ppm[0].size() << " 1" << endl;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            myfile << ppm[i][j*3] << " " << ppm[i][j*3+1] << " " << ppm[i][j*3+2] << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

void part1(){
        cout << "part 1" << "\n";
    ifstream infile("image.ppm");
    string line;
    getline(infile, line);
    line = line.substr(line.find(" ")+1);
    int width = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int height = stoi(line.substr(0, line.find(" ")));
    cout << height << " " << width << "\n";
    vector<vector<int>> img;
    for (int i = 0; i < height; i++)
    {
        vector<int> imgRow; 
        for (int j = 0; j < width * 3; j+= 3)
        {
            getline(infile, line, ' ');
            int r = stoi(line);
            getline(infile, line, ' ');
            int g = stoi(line);
            if(j == (width * 3) - 1)
            {
                getline(infile, line);
            }
            else
            {
                getline(infile, line, ' ');
            }
            int b = stoi(line);
            imgRow.pb(r);
            imgRow.pb(g);
            imgRow.pb(b);
        }
        img.pb(imgRow);
    }
    infile.close();
    cout << "read image ppm" << "\n";
    vector<vector<int>> grayscale;
    vector<int> pad;
    for(int i = 0; i < width+2; i++){
        pad.pb(0);
    }
    grayscale.pb(pad);
    for(int i = 0; i < height; i++){
        vector<int> transform;
        transform.pb(0);
        for(int j = 0; j < width; j++){
            int pixel1 = img[i][j*3];
            int pixel2 = img[i][j*3+1];
            int pixel3 = img[i][j*3+2];
            int avg = (pixel1 + pixel2 + pixel3) / 3;
            transform.pb(avg);
        }
        transform.pb(0);
        grayscale.pb(transform);
    }
    grayscale.pb(pad);
    cout << "grayscaled" << "\n";
    ofstream myfile("imageg.ppm");
    myfile << "P3 " << grayscale[0].size() << " " << grayscale.size() << " 255" << "\n";
    for(int i = 0; i < grayscale.size(); i++){
        for(int j = 0; j < grayscale[0].size(); j++){
            myfile << grayscale[i][j] << " " << grayscale[i][j] << " " << grayscale[i][j] << " ";
        }
        myfile << endl;
    }
    myfile.close();
    vector<vector<int>> sobelX;
    vector<int> temp;
    temp.pb(1);
    temp.pb(0);
    temp.pb(-1);
    sobelX.pb(temp);
    vector<int> temp2;
    temp2.pb(2);
    temp2.pb(0);
    temp2.pb(-2);
    sobelX.pb(temp2);
    sobelX.pb(temp);
    vector<vector<int>> sobelY;
    vector<int> temp3;
    temp3.pb(1);
    temp3.pb(2);
    temp3.pb(1);
    sobelY.pb(temp3);
    vector<int> temp4;
    temp4.pb(0);
    temp4.pb(0);
    temp4.pb(0);
    sobelY.pb(temp4);
    vector<int> temp5;
    temp5.pb(-1);
    temp5.pb(-2);
    temp5.pb(-1);
    sobelY.pb(temp5);
    vector<vector<int>> gradientX;
    for(int i = 0; i < grayscale.size()-2; i++){
        vector<int> accum;
        for(int j = 0; j < grayscale[0].size()-2; j++){
            int sum = 0;
            sum += grayscale[i][j] * sobelX[0][0];
            sum += grayscale[i][j+1] * sobelX[1][0];
            sum += grayscale[i][j+2] * sobelX[2][0];
            sum += grayscale[i+1][j] * sobelX[0][1];
            sum += grayscale[i+1][j+1] * sobelX[1][1];
            sum += grayscale[i+1][j+2] * sobelX[2][1];
            sum += grayscale[i+2][j] * sobelX[0][2];
            sum += grayscale[i+2][j+1] * sobelX[1][2];
            sum += grayscale[i+2][j+2] * sobelX[2][2];
            accum.pb(sum);
        }
        gradientX.pb(accum);
    }
    vector<vector<int>> gradientY;
    for(int i = 0; i < grayscale.size()-2; i++){
        vector<int> accum;
        for(int j = 0; j < grayscale[0].size()-2; j++){
            int sum = 0;
            sum += grayscale[i][j] * sobelY[0][0];
            sum += grayscale[i][j+1] * sobelY[1][0];
            sum += grayscale[i][j+2] * sobelY[2][0];
            sum += grayscale[i+1][j] * sobelY[0][1];
            sum += grayscale[i+1][j+1] * sobelY[1][1];
            sum += grayscale[i+1][j+2] * sobelY[2][1];
            sum += grayscale[i+2][j] * sobelY[0][2];
            sum += grayscale[i+2][j+1] * sobelY[1][2];
            sum += grayscale[i+2][j+2] * sobelY[2][2];
            accum.pb(sum);
        }
        gradientY.pb(accum);
    }
    vector<vector<double>> gradient;
    for(int i = 0; i < gradientX.size(); i++){
        vector<double> gradientRow;
        for(int j = 0; j < gradientX[0].size(); j++){
            gradientRow.pb(sqrt(gradientX[i][j]*gradientX[i][j]+gradientY[i][j]*gradientY[i][j]));
        }
        gradient.pb(gradientRow);
    }
    ofstream mf("imagem.ppm");
    mf << "P3 " << gradient[0].size() << " " << gradient.size() << " 1" << endl;
    for(int i = 0; i < gradient.size(); i++){
        for(int j = 0; j < gradient[0].size(); j++){
            if(gradient[i][j] > 125){
                mf << "1 1 1 ";
            }else{
                mf << "0 0 0 ";
            }
        }
        mf << endl;
    }
    mf.close();
}

void floodfill(vector<vector<int>> &table, int row, int col){
    if(row < 0 || col < 0 || row >= table.size() || col >= table[0].size()){
        return;
    }
    if(table[row][col] == 1){
        table[row][col] = 2;
        floodfill(table, row-1, col);
        floodfill(table, row-1, col-1);
        floodfill(table, row, col-1);
        floodfill(table, row+1, col-1);
        floodfill(table, row+1, col);
        floodfill(table, row+1, col+1);
        floodfill(table, row, col+1);
    }
}

void part2(){
    cout << "part 1" << "\n";
    ifstream infile("image.ppm");
    string line;
    getline(infile, line);
    line = line.substr(line.find(" ")+1);
    int width = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int height = stoi(line.substr(0, line.find(" ")));
    cout << height << " " << width << "\n";
    vector<vector<int>> img;
    for (int i = 0; i < height; i++)
    {
        vector<int> imgRow; 
        for (int j = 0; j < width * 3; j+= 3)
        {
            getline(infile, line, ' ');
            int r = stoi(line);
            getline(infile, line, ' ');
            int g = stoi(line);
            if(j == (width * 3) - 1)
            {
                getline(infile, line);
            }
            else
            {
                getline(infile, line, ' ');
            }
            int b = stoi(line);
            imgRow.pb(r);
            imgRow.pb(g);
            imgRow.pb(b);
        }
        img.pb(imgRow);
    }
    infile.close();
    cout << "read image ppm" << "\n";
    vector<vector<int>> grayscale;
    vector<int> pad;
    for(int i = 0; i < width+2; i++){
        pad.pb(0);
    }
    grayscale.pb(pad);
    for(int i = 0; i < height; i++){
        vector<int> transform;
        transform.pb(0);
        for(int j = 0; j < width; j++){
            int pixel1 = img[i][j*3];
            int pixel2 = img[i][j*3+1];
            int pixel3 = img[i][j*3+2];
            int avg = (pixel1 + pixel2 + pixel3) / 3;
            transform.pb(avg);
        }
        transform.pb(0);
        grayscale.pb(transform);
    }
    grayscale.pb(pad);
    cout << "grayscaled" << "\n";
    ofstream myfile("imageg.ppm");
    myfile << "P3 " << grayscale[0].size() << " " << grayscale.size() << " 255" << "\n";
    for(int i = 0; i < grayscale.size(); i++){
        for(int j = 0; j < grayscale[0].size(); j++){
            myfile << grayscale[i][j] << " " << grayscale[i][j] << " " << grayscale[i][j] << " ";
        }
        myfile << endl;
    }
    myfile.close();
    vector<vector<int>> sobelX;
    vector<int> temp;
    temp.pb(1);
    temp.pb(0);
    temp.pb(-1);
    sobelX.pb(temp);
    vector<int> temp2;
    temp2.pb(2);
    temp2.pb(0);
    temp2.pb(-2);
    sobelX.pb(temp2);
    sobelX.pb(temp);
    vector<vector<int>> sobelY;
    vector<int> temp3;
    temp3.pb(1);
    temp3.pb(2);
    temp3.pb(1);
    sobelY.pb(temp3);
    vector<int> temp4;
    temp4.pb(0);
    temp4.pb(0);
    temp4.pb(0);
    sobelY.pb(temp4);
    vector<int> temp5;
    temp5.pb(-1);
    temp5.pb(-2);
    temp5.pb(-1);
    sobelY.pb(temp5);
    vector<vector<int>> gradientX;
    for(int i = 0; i < grayscale.size()-2; i++){
        vector<int> accum;
        for(int j = 0; j < grayscale[0].size()-2; j++){
            int sum = 0;
            sum += grayscale[i][j] * sobelX[0][0];
            sum += grayscale[i][j+1] * sobelX[1][0];
            sum += grayscale[i][j+2] * sobelX[2][0];
            sum += grayscale[i+1][j] * sobelX[0][1];
            sum += grayscale[i+1][j+1] * sobelX[1][1];
            sum += grayscale[i+1][j+2] * sobelX[2][1];
            sum += grayscale[i+2][j] * sobelX[0][2];
            sum += grayscale[i+2][j+1] * sobelX[1][2];
            sum += grayscale[i+2][j+2] * sobelX[2][2];
            accum.pb(sum);
        }
        gradientX.pb(accum);
    }
    vector<vector<int>> gradientY;
    for(int i = 0; i < grayscale.size()-2; i++){
        vector<int> accum;
        for(int j = 0; j < grayscale[0].size()-2; j++){
            int sum = 0;
            sum += grayscale[i][j] * sobelY[0][0];
            sum += grayscale[i][j+1] * sobelY[1][0];
            sum += grayscale[i][j+2] * sobelY[2][0];
            sum += grayscale[i+1][j] * sobelY[0][1];
            sum += grayscale[i+1][j+1] * sobelY[1][1];
            sum += grayscale[i+1][j+2] * sobelY[2][1];
            sum += grayscale[i+2][j] * sobelY[0][2];
            sum += grayscale[i+2][j+1] * sobelY[1][2];
            sum += grayscale[i+2][j+2] * sobelY[2][2];
            accum.pb(sum);
        }
        gradientY.pb(accum);
    }
    vector<vector<double>> gradient;
    for(int i = 0; i < gradientX.size(); i++){
        vector<double> gradientRow;
        for(int j = 0; j < gradientX[0].size(); j++){
            gradientRow.pb(sqrt(gradientX[i][j]*gradientX[i][j]+gradientY[i][j]*gradientY[i][j]));
        }
        gradient.pb(gradientRow);
    }
    vector<vector<int>> table;
    for(int i = 0; i < gradientX.size(); i++){
        vector<int> row;
        for(int j = 0; j < gradientX[0].size(); j++){
            if(gradient[i][j] < 70){
                row.pb(0);
            }else if(gradient[i][j] < 200){
                row.pb(1);
            }else{
                row.pb(2);
            }
        }
        table.pb(row);
    }
    //vector<vector<int>> &table;
    for(int i = 0; i < gradientX.size(); i++){
        for(int j = 0; j < gradientX[0].size(); j++){
            if(table[i][j] == 2){
                floodfill(table, i, j);
            }
        }
    }
    ofstream mf("imagem.ppm");
    mf << "P3 " << gradient[0].size() << " " << gradient.size() << " 1" << endl;
    for(int i = 0; i < gradient.size(); i++){
        for(int j = 0; j < gradient[0].size(); j++){
            if(table[i][j] == 2){
                mf << "1 1 1 ";
            }else{
                mf << "0 0 0 ";
            }
        }
        mf << endl;
    }
    mf.close();
}

int main(){
    part2();
}