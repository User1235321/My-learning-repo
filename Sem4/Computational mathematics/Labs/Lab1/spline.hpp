#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

using vec = vector<double>;

struct SplineSet{
    double a;
    double b;
    double c;
    double d;
    double x;
};

vector<SplineSet> spline(vec &x, vec &y);
