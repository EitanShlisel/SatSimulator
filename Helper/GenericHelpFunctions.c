#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "GenericHelpFunctions.h"
#include <stdlib.h>
int getLine(FILE *fp, char *line){
    if(NULL == fp){
        return -1;
    }
    if(NULL == line){
        return -2;
    }
    unsigned int i = 0;
    while('\n' != (line[i] = (char)getc(fp))){
        i++;
    }
    line[i] = '\0';
    return 0;
}

double GnrHelper_GenerateUniformInRange(double start,double end){
    if(end < start){
        return 0;
    }
    if(end == start) {
        return start;
    }
    int rnd = rand();
    double range = end - start;
    int pw = pow(10,((int)log10(RAND_MAX)));
    double ret = start
                 + (double)(rnd % (int)(range))
                 + ( rnd % pw)/(double)pw;
    return ret;
}

double GnrHelper_GenerateGaussianNoise(double mu, double sigma)
{
    static const double epsilon = DBL_MIN;
    static const double two_pi = 2.0 * 3.14159265358979323846;
    static double z1;
    static bool generate;
    generate = !generate;

    if (!generate) {
        return z1 * sigma + mu;
    }
    double u1, u2;
    do{
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    }
    while (u1 <= epsilon);
    double z0;
    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);

    return z0 * sigma + mu;
}

double GnrHelper_CalcMagnitude(vector_t vec){
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

vector_t GnrHelper_GetUnitVectorFromPoints(point_t l1, point_t l2){
    vector_t unit;
    unit.x = l1.x-l2.x;
    unit.y = l1.y-l2.y;
    unit.z = l1.z-l2.z;

    return GnrHelper_GetUnitVector(unit);
}

vector_t GnrHelper_GetUnitVector(vector_t vec){
    if(vec.x == 0 && vec.y == 0 && vec.z == 0 ){
        return (vector_t){0,0,0};
    }
    vector_t unit = {.x=vec.x,.y=vec.y,.z=vec.z};
    double mag = GnrHelper_CalcMagnitude(unit);
    unit.x /= mag;
    unit.y /= mag;
    unit.z /= mag;
    return unit;
}

vector_t GnrHelper_VecMinus(vector_t a, vector_t b){
    return (point_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vector_t GnrHelper_VecPlus(vector_t a, vector_t b){
    return (point_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

double GnrHelper_VecMult(vector_t a, vector_t b){
    return (a.x*b.x + a.y*b.y + a.z*b.z);
}

int GnrHelper_LineSphereIntersection(point_t l1, point_t l2, point_t center, double radius){
    double res = 0;
    point_t unit = GnrHelper_GetUnitVectorFromPoints(l1, l2);
    double temp = 0;
    res = pow(GnrHelper_VecMult(unit, GnrHelper_VecMinus(l2, center)), 2) -
          (pow(GnrHelper_CalcMagnitude(GnrHelper_VecMinus(l2, center)), 2) - pow(radius, 2));
    if(res > 0)
        return 2;
    if(res == 0)
        return 1;
    if(res < 0)
        return 0;
}

double GnrHelper_CalcDistance(point_t a, point_t b){
    vector_t vec = GnrHelper_VecMinus(a,b);
    return GnrHelper_CalcMagnitude(vec);
}