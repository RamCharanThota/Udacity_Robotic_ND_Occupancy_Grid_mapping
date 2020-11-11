#include <iostream>
#include <math.h>
using namespace std;

const int mapWidth =  2;
const int mapHeight = 2;
double fused_map[mapHeight][mapWidth]={{0,0},{0,0}};

void sensorFusion(double m1[][mapWidth], double m2[][mapWidth])
{
    //*#############TODO: Code the Sensor Fusion Function############*//
    // Fuse the measurments of the two maps and print the resulting 
    //map in a matrix form:
    //a  b
    //c  d
    
    for (int r=0;r<mapHeight;r++){ //loop through each row
        for (int c=0;c<mapWidth;c++ ){// loop through each column of row
          
            fused_map[r][c]=1-((1-m1[r][c])*(1-m2[r][c])); // deMorgans law 
            cout<<fused_map[r][c]<<" ";
        }
        cout<<endl;
    }



} 

int main()
{

    double m1[mapHeight][mapWidth] = { { 0.9, 0.6 }, { 0.1, 0.5 } };
    double m2[mapHeight][mapWidth] = { { 0.3, 0.4 }, { 0.4, 0.3 } };
    sensorFusion(m1, m2);
    cout <<"fuesed map is"<<fused_map[0][0];

    return 0;
}
