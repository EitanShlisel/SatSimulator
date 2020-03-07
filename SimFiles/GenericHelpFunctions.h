#ifndef SIMPROJECT_GENERICHELPFUNCTIONS_H
#define SIMPROJECT_GENERICHELPFUNCTIONS_H

#include <stdio.h>
typedef struct{
    double x;
    double y;
    double z;
}point;
int getLine(FILE *fp, char *line);

double GnrHelper_GenerateUniformInRange(double start,double end);

double GnrHelper_GenerateGaussianNoise(double mu, double sigma);

double CalcMagnitude(point vec);

point GetUnitVector(point l1,point l2);

inline point VecMinus(point a,point b);

inline point VecPlus(point a,point b);

inline double VecMult(point a, point b);

int LineSphereIntersection(point l1, point l2, point center, double radius);

#endif //SIMPROJECT_GENERICHELPFUNCTIONS_H
