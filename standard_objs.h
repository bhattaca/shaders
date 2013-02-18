#ifndef STANDARD_OBJS_H_INCLUDED
#define STANDARD_OBJS_H_INCLUDED

#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
using namespace std;

typedef struct
{
    float location[4];
    float color[4];
    float normal[4];
    float texture[2];
    float tangents[4];
} Vertex;


void InitSquare(         vector <Vertex> &sqVerts,
                   vector <GLuint> &sqIndices);
void createSphere (float radius, int numSlices, int numStacks, float color[],
                     vector <Vertex> &sphereVerts, vector <GLuint> &sphereIndices);
void createConics (float BaseRadius, float TopRadius, float height, int numSlices,
                    int numStacks, float color[],
                   vector <Vertex> &conicsVerts, vector <GLuint> &conicsIndices);
void createCube(vector <Vertex> &sqverts,
        vector <GLuint> &sqIndices);
#endif // STANDARD_OBJS_H_INCLUDED
