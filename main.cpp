#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

// Sensor characteristic: Min and Max ranges of the beams
double Zmax = 5000, Zmin = 170;
// Defining free cells(lfree), occupied cells(locc), unknown cells(l0) log odds values
double l0 = 0, locc = 0.4, lfree = -0.4;
// Grid dimensions
double gridWidth = 100, gridHeight = 100;
// Map dimensions
double mapWidth = 30000, mapHeight = 15000;
// Robot size with respect to the map 
double robotXOffset = mapWidth / 5, robotYOffset = mapHeight / 3;
// Defining an l vector to store the log odds values of each cell
vector< vector<double> > l(mapWidth/gridWidth, vector<double>(mapHeight/gridHeight));

double inverseSensorModel(double x, double y, double theta, double xi, double yi, double sensorData[])
{
        //******************Code the Inverse Sensor Model Algorithm**********************//
    // Defining Sensor Characteristics
    double Zk, thetaK, sensorTheta;
    double minDelta = -1;
    double alpha = 200, beta = 20;

    //******************TODO: Compute r and phi**********************//
    double r = sqrt(pow((x-xi),2)+pow((y-yi),2));
    double phi=atan2((yi-y),(xi-x))-theta;
   

    //Scaling Measurement to [-90 -37.5 -22.5 -7.5 7.5 22.5 37.5 90]
    for (int i = 0; i < 8; i++) {
        if (i == 0) {
            sensorTheta = -90 * (M_PI / 180);
        }
        else if (i == 1) {
            sensorTheta = -37.5 * (M_PI / 180);
        }
        else if (i == 6) {
            sensorTheta = 37.5 * (M_PI / 180);
        }
        else if (i == 7) {
            sensorTheta = 90 * (M_PI / 180);
        }
        else {
            sensorTheta = (-37.5 + (i - 1) * 15) * (M_PI / 180);
        }

        if (fabs(phi - sensorTheta) < minDelta || minDelta == -1) {
            Zk = sensorData[i];
            thetaK = sensorTheta;
            minDelta = fabs(phi - sensorTheta);
        }
    }

    //******************TODO: Evaluate the three cases**********************//
    // You also have to consider the cells with Zk > Zmax or Zk < Zmin as unkown states
    
    if (r>min(Zmax,Zk+alpha/2) || fabs(phi-thetaK)>beta/2 ||Zk>Zmax ||Zk<Zmin){
        return l0;
        }
        else if (Zk<Zmax && fabs(r-Zk)<alpha/2){
        return locc;
        }else if(r<=Zk){
        return lfree;
        }
        
    
    
    
}

void occupancyGridMapping(double Robotx, double Roboty, double Robottheta, double sensorData[])
{
    //1 - TODO: Generate a grid (size 300x150) and then loop through all the cells
        vector<vector<double>> grid{300,vector<double>{150}};
            //2- TODO: Compute the center of mass of each cell xi and yi 
            for (int width_ind=0;width_ind <mapWidth/gridWidth;width_ind++){
                for(int height_ind=0;height_ind<mapHeight/gridHeight;height_ind++){
                    double xi = width_ind * gridWidth + gridWidth / 2 - robotXOffset;
                    double yi = -(height_ind * gridHeight + gridHeight / 2) + robotYOffset;
                    //3- TODO: Check if each cell falls under the perceptual field of the measurements
                    double dist_from_robot = sqrt(pow((xi-Robotx),2)+pow((yi-Roboty),2));
                    if (dist_from_robot<=Zmax ){ // condition to grid falls under feild of view
                         l[width_ind][height_ind]=l[width_ind][height_ind]+inverseSensorModel(Robotx,Roboty,Robottheta,xi,yi,sensorData);
                    }
                    else {
                        l[width_ind][width_ind]=l[width_ind][width_ind];

                    }
                

                }
                

            }
            
            
                 
     
    
    
    
    
    
}

int main()
{
    double timeStamp;
    double measurementData[8];
    double robotX, robotY, robotTheta;

    FILE* posesFile = fopen("poses.txt", "r");
    FILE* measurementFile = fopen("measurement.txt", "r");

    // Scanning the files and retrieving measurement and poses at each timestamp
    while (fscanf(posesFile, "%lf %lf %lf %lf", &timeStamp, &robotX, &robotY, &robotTheta) != EOF) {
        fscanf(measurementFile, "%lf", &timeStamp);
        for (int i = 0; i < 8; i++) {
            fscanf(measurementFile, "%lf", &measurementData[i]);
        }
        occupancyGridMapping(robotX, robotY, (robotTheta / 10) * (M_PI / 180), measurementData);
    }
    
    // Displaying the map
    for (int x = 0; x < mapWidth / gridWidth; x++) {
        for (int y = 0; y < mapHeight / gridHeight; y++) {
            cout << l[x][y] << " ";
        }
    }
    
    return 0;
}

