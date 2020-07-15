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
}point_t;

typedef point_t vector_t;

double GnrHelper_CalcMagnitude(vector_t vec);

vector_t GnrHelper_GetUnitVectorFromPoints(point_t l1, point_t l2);

vector_t GnrHelper_GetUnitVector(vector_t vec);

vector_t GnrHelper_VecMinus(vector_t a, vector_t b);

vector_t GnrHelper_VecPlus(vector_t a, vector_t b);

double GnrHelper_VecMult(vector_t a, vector_t b);

double GnrHelper_AngleBetweenVectors(vector_t a, vector_t b);

// ------------------------------   Geometry
int GnrHelper_LineSphereIntersection(point_t l1, point_t l2, point_t center, double radius);

double GnrHelper_CalcDistance(point_t a, point_t b);
// ------------------------------   Electronics

#define GnrHelper_Ohms_PowerToCurrent_mA(pwr_mW,volt_mV) ((double)(1000*(pwr_mW)/(volt_mV)))
#define GnrHelper_Ohms_PowerToVoltage_mV(pwr_mW,curr_mA) ((double)(1000*(pwr_mW)/(curr_mA)))
#define GnrHelper_Ohms_CalcPower_mW(curr_mA,volt_mV) ((double)(1000*(curr_mA)*(volt_mV)))
#define mAsec_to_mAh(mAsec) (double)(((double)(mAsec))/3600.0)



#endif //SIMPROJECT_GENERICHELPFUNCTIONS_H
