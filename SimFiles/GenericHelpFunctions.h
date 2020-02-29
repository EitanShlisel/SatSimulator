#ifndef SIMPROJECT_GENERICHELPFUNCTIONS_H
#define SIMPROJECT_GENERICHELPFUNCTIONS_H

#include <stdio.h>

int getLine(FILE *fp, char *line);

double GnrHelper_GenerateUniformInRange(double start,double end);

double GnrHelper_GenerateGaussianNoise(double mu, double sigma);

#endif //SIMPROJECT_GENERICHELPFUNCTIONS_H
