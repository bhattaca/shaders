#ifndef _READOFF_H_INCLUDED
#define _READOFF_H_INCLUDED


#include <iostream>
#include <string>
#include <vector>

#include "standard_objs.h"
using namespace std;


class POINT
{
public:
    float x,y,z;
    float n1 , n2, n3;
    int numNorms;
};


class FACE
{
public:
    int a,b,c;
};

class readOFF
{
public:
    vector<string> tokens;
    void readData(const string &inputFname);
};

//object
class OBJECT
{
public :
    int numVerts;
    int numFaces;
    POINT * points;
    FACE * faces;

    // take the tokes and make an on object out of it
    OBJECT (const vector<string> &tokens);

};
void readMeshData(const string fname,vector <Vertex> &Verts, vector <GLuint> &Indices);
void normalize(POINT & n);


#endif // _READOFF_H_INCLUDED
