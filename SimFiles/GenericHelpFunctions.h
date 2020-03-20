#ifndef SIMPROJECT_GENERICHELPFUNCTIONS_H
#define SIMPROJECT_GENERICHELPFUNCTIONS_H

#include <stdio.h>

// ------------------------------   IO
int getLine(FILE *fp, char *line);

// ------------------------------   Statistics
double GnrHelper_GenerateUniformInRange(double start,double end);

double GnrHelper_GenerateGaussianNoise(double mu, double sigma);

// ------------------------------   Vector Calculus
typedef struct{
    double x;
    double y;
    double z;
}point;

double GnrHelper_CalcMagnitude(point vec);

point GnrHelper_GetUnitVector(point l1, point l2);

point GnrHelper_VecMinus(point a, point b);

point GnrHelper_VecPlus(point a, point b);

double GnrHelper_VecMult(point a, point b);

// ------------------------------   Geometry
int GnrHelper_LineSphereIntersection(point l1, point l2, point center, double radius);

// ------------------------------   Electronics

#define GnrHelper_Ohms_PowerToCurrent_mA(pwr_mW,volt_mV) ((double)(1000*(pwr_mW)/(volt_mV)))
#define GnrHelper_Ohms_PowerToVoltage_mV(pwr_mW,curr_mA) ((double)(1000*(pwr_mW)/(curr_mA)))
#define GnrHelper_Ohms_CalcPower_mW(curr_mA,volt_mV) ((double)(1000*(curr_mA)*(volt_mV)))
#define mAsec_to_mAh(mAsec) (double)(((double)(mAsec))/3600.0)



#endif //SIMPROJECT_GENERICHELPFUNCTIONS_H
