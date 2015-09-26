#pragma once

#define DEG_TO_RAD 0.0174532925f
#define RAD_TO_DEG 57.2957795f
#define DEGREE_360 360.0f

// Pie is not round, pie r squared
#ifndef PI
#define PI 3.14159f
#endif

// Roundoff error to be ignored in floating point number comparisons
#ifndef EPSILON
#define EPSILON 0.0001f
#endif

#define EPSI(test, goal) (test < goal + EPSILON && test > goal - EPSILON)