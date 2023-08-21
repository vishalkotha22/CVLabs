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

void illuminate(vector<vector<int>> &ppm, int i, int j){
    if(i >= 0 && i < ppm[0].size() && j >= 0 && j < ppm.size()){
        ppm[j][i*3] = 255;
        ppm[j][i*3+1] = 255;
        ppm[j][i*3+2] = 255;
    }
}

void illuminateRed(vector<vector<int>> &ppm, int i, int j){
    if(i >= 0 && i < ppm[0].size() && j >= 0 && j < ppm.size()){
        ppm[j][i*3] = 255;
        ppm[j][i*3+1] = 0;
        ppm[j][i*3+2] = 0;
    }
}

void voteChoose(vector<vector<int>> &ppm, int i, int j){
    if(i >= 0 && i < ppm[0].size() && j >= 0 && j < ppm.size()){
        ppm[j][i] += 1;
    }
}

void drawCircle(vector<vector<int>> &ppm, double centerX, double centerY, double radius){
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
        illuminateRed(ppm, (int)(centerX + x), (int)(centerY + y));
        illuminateRed(ppm, (int)(centerX + x), (int)(centerY + -y));
        illuminateRed(ppm, (int)(centerX + -x), (int)(centerY + y));
        illuminateRed(ppm, (int)(centerX + -x), (int)(centerY + -y));
        illuminateRed(ppm, (int)(centerX + y), (int)(centerY + x));
        illuminateRed(ppm, (int)(centerX + y), (int)(centerY + -x));
        illuminateRed(ppm, (int)(centerX + -y), (int)(centerY + x));
        illuminateRed(ppm, (int)(centerX + -y), (int)(centerY + -x));
        y2_new -= (2 * x) - 3;
    }
}

void bresenham(vector<vector<int>> &ppm, int x1, int y1, int x2, int y2){
    //cout << "startB\n";
    if(abs(x2-x1) >= abs(y2-y1) && ((x2 >= x1 && y2 >= y1) || (x1 >= x2 && y1 >= y2))){
        cout << "case 1 \n";
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
            //cout << i << " " << j << ppm[i][j] << "\n";
            voteChoose(ppm, i, j);
            //cout << "illuminated \n";
            if(deltaE >= 0){
                j++;
                deltaE -= deltaX;
            }
            deltaE += deltaY;
            if(j >= ppm.size()){
                break;
            }
        }
    }else if(abs(x2-x1) >= abs(y2-y1) && ((x1 >= x2 && y2 >= y1) || (x2 >= x1 && y1 >= y2))){
        cout << "case2 \n";
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
            voteChoose(ppm, i, j);
            if(deltaE >= 0){
                j--;
                deltaE -= deltaX;
            }
            deltaE += deltaY;
            if(j < 0){
                break;
            }
        }
    }
    else if(abs(y2 - y1) >= abs(x2 - x1) && ((x2 >= x1 && y2 >= y1) || (x1 >= x2 && y1 >= y2))){
        cout << "case3 \n";
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
            //cout << "testing " << j << " " << i << "\n";
            voteChoose(ppm, j, i);
            //cout << "illuminated\n";
            if(deltaE >= 0){
                j++;
                deltaE -= deltaY;
            }
            deltaE += deltaX;
            if(i >= ppm[0].size()){
                break;
            }
        }
    }else if(abs(y2 - y1) >= abs(x2 - x1) && ((x1 >= x2 && y2 >= y1) || (x2 >= x1 && y1 >= y2))){
        cout << "case 4 \n";
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
            //cout << i << "\n";
            voteChoose(ppm, j, i);
            if(deltaE >= 0){
                j--;
                deltaE -= deltaY;
            }
            deltaE += deltaX;
            //cout << j << " " << (j < 0) << "\n";
            if(i < 0){
                i = y2;
            }
        }
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

void part1f(){
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
    ofstream mf("image1.ppm");
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

vector<vector<int>> part2(int argc, char** argv){
    int lower = 110;
    int upper = 200;
    for(int i = 0; i < argc; i++){
        string a = argv[i];
        if(a == ("-L")){
            lower = stoi(argv[i+1]);
        }
        if(a == ("-H")){
            upper = stoi(argv[i+1]);
        }
    }
    cout << "part 1" << "\n";
    ifstream infile("image.ppm");
    string line;
    infile >> line;
    infile >> line;
    int width = stoi(line);
    infile >> line;
    int height = stoi(line);
    infile >> line;
    int scale = stoi(line);
    vector<vector<int>> img;
    cout << width << " " << height << "\n";
    for (int i = 0; i < height; i++)
    {
        vector<int> imgRow; 
        for (int j = 0; j < width * 3; j+= 3)
        {
            infile >> line;
            int r = stoi(line);
            infile >> line;
            int g = stoi(line);
            infile >> line;
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
            if(gradient[i][j] < lower){
                row.pb(0);
            }else if(gradient[i][j] < upper){
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
    ofstream mf("image1.ppm");
    mf << "P3 " << gradient[0].size() << " " << gradient.size() << " 1" << endl;
    for(int i = 0; i < gradient.size(); i++){
        for(int j = 0; j < gradient[0].size(); j++){
            if(i > 0 && i < gradient.size()-1 && j > 0 && j < gradient[0].size()-1 && table[i][j] == 2){
                mf << "1 1 1 ";
            }else{
                mf << "0 0 0 ";
            }
        }
        mf << endl;
    }
    mf.close();
    return table;
}

vector<vector<vector<double>>> part3(int argc, char** argv){
    vector<vector<vector<double>>> needed;
    vector<vector<int>> table = part2(argc, argv);
    ifstream infile("image.ppm");
    string line;
    infile >> line;
    infile >> line;
    int width = stoi(line);
    infile >> line;
    int height = stoi(line);
    infile >> line;
    int scale = stoi(line);
    vector<vector<int>> img;
    cout << width << " " << height << "\n";
    for (int i = 0; i < height; i++)
    {
        vector<int> imgRow; 
        for (int j = 0; j < width * 3; j+= 3)
        {
            infile >> line;
            int r = stoi(line);
            infile >> line;
            int g = stoi(line);
            infile >> line;
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
    vector<double> np;
    for(int i = 0; i < gradientX[0].size()+2; i++){
        np.pb(0.0);
    }
    gradient.pb(np);
    for(int i = 0; i < gradientX.size(); i++){
        vector<double> gradientRow;
        gradientRow.pb(0);
        for(int j = 0; j < gradientX[0].size(); j++){
            gradientRow.pb(sqrt(gradientX[i][j]*gradientX[i][j]+gradientY[i][j]*gradientY[i][j]));
        }
        gradientRow.pb(0);
        gradient.pb(gradientRow);
    }
    gradient.pb(np);
    cout << "calculated gradients" << "\n";
    vector<vector<double>> angleMatrix;
    for(int i = 0; i < gradientX.size(); i++){
        vector<double> angles;
        for(int j = 0; j < gradientX[0].size(); j++){
            if(gradientX[i][j] > 0 || gradientX[i][j] < 0){
                angles.pb(atan2(gradientY[i][j], gradientX[i][j])*180/M_PI);
            }else{
                if(gradientY[i][j] > 0){
                    angles.pb(90);
                }else{
                    angles.pb(-90);
                }
            }
        }
        angleMatrix.pb(angles);
    }
    needed.pb(angleMatrix);
    cout << "created angle matrix" << "\n";
    vector<int> closestAngles;
    closestAngles.pb(-180);
    closestAngles.pb(-135);
    closestAngles.pb(-90);
    closestAngles.pb(-45);
    closestAngles.pb(0);
    closestAngles.pb(45);
    closestAngles.pb(90);
    closestAngles.pb(135);
    closestAngles.pb(180);
    vector<vector<int>> rounded;
    for(int i = 0; i < gradientX.size(); i++){
        vector<int> rounding;
        for(int j = 0; j < gradientX[0].size(); j++){
            int currentClosestAngle = -180;
            for(int k = 0; k < closestAngles.size(); k++){
                if(abs(currentClosestAngle - angleMatrix[i][j]) >= abs(closestAngles[k] - angleMatrix[i][j])){
                    currentClosestAngle = closestAngles[k];
                }
            }
            rounding.pb(currentClosestAngle);
        }
        rounded.pb(rounding);
    }
    cout << "rounded angles" << "\n";
    vector<vector<int>> binary;
    for(int i = 1; i < gradientX.size()+1; i++){
        vector<int> binaryRow;
        for(int j = 1; j < gradientX[0].size()+1; j++){
            int angle = rounded[i-1][j-1];
            if(angle == 0 || angle == 180 || angle == -180){
                if(gradient[i][j-1] < gradient[i][j] && gradient[i][j+1] < gradient[i][j]){
                    binaryRow.pb(1);
                }else{
                    binaryRow.pb(0);
                }
            }
            if(angle == 45 || angle == -135){
                if(gradient[i-1][j+1] < gradient[i][j] && gradient[i+1][j-1] < gradient[i][j]){
                    binaryRow.pb(1);
                }else{
                    binaryRow.pb(0);
                }
            }
            if(angle == 90 || angle == -90){
                if(gradient[i-1][j] < gradient[i][j] && gradient[i+1][j] < gradient[i][j]){
                    binaryRow.pb(1);
                }else{
                    binaryRow.pb(0);
                }
            }
            if(angle == -45 || angle == 135){
                if(gradient[i+1][j+1] < gradient[i][j] && gradient[i-1][j-1] < gradient[i][j]){
                    binaryRow.pb(1);
                }else{
                    binaryRow.pb(0);
                }
            }
        }
        binary.pb(binaryRow);
    }
    cout << "created binary image" << "\n";
    ofstream mf("image2.ppm");
    mf << "P3 " << binary[0].size() << " " << binary.size() << " 1" << endl;
    for(int i = 0; i < binary.size(); i++){
        for(int j = 0; j < binary[0].size(); j++){
            if(binary[i][j] == 1){
                mf << "1 1 1 ";
            }else{
                mf << "0 0 0 ";
            }
        }
        mf << endl;
    }
    mf.close();
    ofstream w("imagef.ppm");
    w << "P3 " << binary[0].size() << " " << binary.size() << " 1" << endl;
    vector<vector<double>> ret;
    for(int i = 0; i < binary.size(); i++){
        vector<double> goat;
        for(int j = 0; j < binary[0].size(); j++){
            if(binary[i][j] == 1 && table[i][j] == 2){
                w << "1 1 1 ";
            }else{
                w << "0 0 0 ";
            }
            if(table[i][j] == 2){
                goat.pb(1.0);
            }else{
                goat.pb(0.0);
            }
        }
        ret.pb(goat);
        w << endl;
    }
    w.close();
    needed.pb(ret);
    return needed;
}

void part1(int argc, char** argv){
    int lower = 70;
    int upper = 170;
    int circleThres = 40;
    for(int i = 0; i < argc; i++){
        string a = argv[i];
        if(a == ("-L")){
            lower = stoi(argv[i+1]);
        }
        if(a == ("-H")){
            upper = stoi(argv[i+1]);
        }
        if(a == ("-TC")){
            circleThres = stoi(argv[i+1]);
        }
    }
    vector<vector<vector<double>>> parts = part3(argc, argv);
    vector<vector<double>> angleMatrix = parts[0];
    vector<vector<double>> finalImage = parts[1];
    vector<vector<int>> vote;
    for(int i = 0; i < finalImage.size(); i++){
        vector<int> voteRow;
        for(int j = 0; j < finalImage[0].size(); j++){
            voteRow.pb(0);
        }
        vote.pb(voteRow);
    }
    /*
    for(int i = 0; i < finalImage.size(); i++){
        for(int j = 0; j < finalImage[0].size(); j++){
            if(finalImage[i][j] == 1.0){
                cout << "running bresenham on " << i << ", " << j << "\n";
                double angle = angleMatrix[i][j]*3.14159265358979/180;
                //double slope = tan(angle);
                double slope = 999999;
                if(abs(tan(angle)) > 0){
                    slope = 1 / tan(angle);
                }
                double y1 = j - slope * i;
                double y2 = y1 + finalImage[0].size() * slope;
                cout << slope << " " << angle << " " << tan(angle) << " 0 " << y1 << " " << finalImage[0].size() << " " << y2 << "\n";
                bresenham(vote, 0, (int)y1, finalImage[0].size(), (int)y2);
                //bresenham(vote, 0, (int)y1+1, finalImage[0].size(), (int)y2+1);
                //bresenham(vote, 0, (int)y1-1, finalImage[0].size(), (int)y2-1);
                //double x1 = -y1 / slope;
                //double x2 = (finalImage.size() - y1) / slope;
                //bresenham(vote, (int)x1, 0, (int)x2, finalImage.size());
                cout << "finished bresenhams" << "\n";
            }
        }
    }*/
    int max = 0;
    for(int i = 0; i < vote.size(); i++){
        for(int j = 0; j < vote[0].size(); j++){
            if(max < vote[i][j]){
                max = vote[i][j];
            }
        }
    }
    ofstream myfile("imagev.ppm");
    myfile << "P3 " << vote[0].size() << " " << vote.size() << " " << 40 << "\n";
    for(int i = 0; i < vote.size(); i++){
        for(int j = 0; j < vote[0].size(); j++){
            myfile << vote[i][j] << " " << vote[i][j] << " " << vote[i][j] << " ";
        }
        myfile << endl;
    }
    myfile.close();
    vector<vector<int>> cc;
    for(int i = 0; i < vote.size(); i++){
        vector<int> build;
        for(int j = 0; j < vote[0].size(); j++){
            if(vote[i][j] >= circleThres){
                build.pb(1);
            }else{
                build.pb(0);
            }
        }
        cc.pb(build);
    }
    ifstream infile("image.ppm");
    string line;
    infile >> line;
    infile >> line;
    int width = stoi(line);
    infile >> line;
    int height = stoi(line);
    infile >> line;
    int scale = stoi(line);
    vector<vector<int>> img;
    cout << width << " " << height << "\n";
    for (int i = 0; i < height; i++)
    {
        vector<int> imgRow; 
        for (int j = 0; j < width * 3; j+= 3)
        {
            infile >> line;
            int r = stoi(line);
            infile >> line;
            int g = stoi(line);
            infile >> line;
            int b = stoi(line);
            imgRow.pb(0);
            imgRow.pb(0);
            imgRow.pb(0);
        }
        img.pb(imgRow);
    }
    infile.close();
    for(int i = 0; i < vote.size(); i++){
        for(int j = 0; j < vote[0].size(); j++){
            if(cc[i][j] == 1){
                illuminateRed(img, i, j);
                drawCircle(img, (double)i, (double)j, (double)1);
                drawCircle(img, (double)i, (double)j, (double)5);
                drawCircle(img, (double)i, (double)j, (double)10);
                drawCircle(img, (double)i, (double)j, (double)25);
                drawCircle(img, (double)j, (double)i, (double)1);
                drawCircle(img, (double)j, (double)i, (double)5);
                drawCircle(img, (double)j, (double)i, (double)10);
                drawCircle(img, (double)j, (double)i, (double)25);
            }
        }
    }
    cout << max << "\n";
    ofstream www("imageCC.ppm");
    www << "P3 " << vote[0].size() << " " << vote.size() << " " << scale << "\n";
    for(int i = 0; i < img.size(); i++){
        for(int j = 0; j < img[0].size(); j++){
            www << img[i][j] << " ";
        }
        www << endl;
    }
    www.close();
} 

int main(int argc, char** argv){
    part1(argc, argv);
}