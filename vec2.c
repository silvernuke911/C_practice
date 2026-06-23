#include <stdio.h>
#include <math.h>

// Define the vec2 struct
typedef struct 
{
    float x;
    float y;
} vec2;

// Function to add two vec2s
vec2 vec2add(vec2 a, vec2 b) 
{
    vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec2 vec2sub(vec2 a, vec2 b) 
{
	vec2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

double vec2dot(vec2 a, vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

double vec2crs(vec2 a, vec2 b) 
{
	return a.x * b.y - a.y * b.x;
}

double vec2norm(vec2 v) 
{
	return sqrt(v.x*v.x + v.y*v.y);
} 

vec2 vec2unit(vec2 v) 
{
	vec2 result;
	double vmag = vec2norm(v);
	result.x = v.x / vmag;
	result.y = v.y / vmag;
	return result;
}

double vec2vang(vec2 a, vec2 b) 
{
	double dot = vec2dot(a, b);
	double arg = dot / ( vec2norm(a) * vec2norm(b));
	return acos(arg);
}

vec2 vec2vprj(vec2 a, vec2 b) 
{
	vec2 result;
	double normb = vec2norm(b);
	double arg = (vec2dot(a, b) / (normb * normb));
	result.x = b.x * arg;
	result.y = b.y * arg;
	return result;
}

vec2 vec2vrej(vec2 a, vec2 b) 
{
	vec2 result;
	vec2 projab = vec2vprj(a, b);
	result.x = a.x - projab.x;
	result.y = a.y - projab.y;
	return result;
}

#define vec2mag(v)       vec2norm(v)
#define vec2normalize(v) vec2unit(v)
#define vec2magnitude(v) vec2norm(v)

// Example usage
int main() 
{
    vec2 v1 = {3.0, 4.0};
    vec2 v2 = {1.5, 2.5};
    
    vec2   v3   = vec2add(v1, v2);
    vec2   v4   = vec2sub(v1, v2);
    double dot  = vec2dot(v1, v2);
    double crs  = vec2crs(v1, v2);
    double mag  = vec2norm(v1);
    vec2   norm = vec2unit(v1);
    double ang  = vec2vang(v1, v2);
    vec2   proj = vec2vprj(v1, v2);
    vec2   rej  = vec2vrej(v1, v2);
    
    printf("v1      : [%.2f, %.2f]\n", v1.x, v1.y);
    printf("v2      : [%.2f, %.2f]\n", v2.x, v2.y);
    printf("v1 + v2 : [%.2f, %.2f]\n", v3.x, v3.y);
    printf("v1 - v2 : [%.2f, %.2f]\n", v4.x, v4.y);
    printf("v1 * v2 :  %.2f\n", dot);
    printf("v1 x v2 :  %.2f\n", crs);
    printf("|v1|    :  %.2f\n", mag);
    printf("v1 normalized            : [%.2f, %.2f]\n", norm.x, norm.y);
    printf("Angle between v1 and  v2 :  %.2f rad (%.2f deg)\n", ang, ang * 180 / M_PI);
    printf("Projection of v1 onto v2 : [%.2f, %.2f]\n", proj.x, proj.y);
    printf("Rejection  of v1 from v2 : [%.2f, %.2f]\n", rej.x, rej.y);
    
    return 0;
}


