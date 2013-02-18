#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
using namespace std;
#include <cmath>
#include "ReadOFF.h"
#include <Eigen/Dense>
using namespace Eigen;
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
void InitSquare(         vector <Vertex> &sqverts,
                   vector <GLuint> &sqIndices)
{

    Vertex v;
  for (int i=0;i<6;i++)
  sqverts.push_back(v);


  sqverts[0].location[0] = -0.5; sqverts[0].location[1] = 0;  sqverts[0].location[2] = -0.5;  sqverts[0].location[3] = 1;
  sqverts[0].normal[0] = 0; sqverts[0].normal[1] = 1;  sqverts[0].normal[2] = 0;  sqverts[0].normal[3] = 0;
  sqverts[0].texture[0] = 0; sqverts[0].texture[1] = 0;
  sqverts[0].color[0] = 0.5; sqverts[0].color[1] = 0.5;  sqverts[0].color[2] = 1;  sqverts[0].color[3] = 1;

  sqverts[1].location[0] = 0.5; sqverts[1].location[1] = 0;  sqverts[1].location[2] = -0.5;  sqverts[1].location[3] = 1;
  sqverts[1].normal[0] = 0; sqverts[1].normal[1] = 1;  sqverts[1].normal[2] = 0;  sqverts[1].normal[3] = 0;
  sqverts[1].texture[0] = 1.0; sqverts[1].texture[1] = 0;
  sqverts[1].color[0] = 0.5; sqverts[1].color[1] = 0.5;  sqverts[1].color[2] = 1;  sqverts[1].color[3] = 1;

  sqverts[2].location[0] = 0.5; sqverts[2].location[1] = 0;  sqverts[2].location[2] = 0.5;  sqverts[2].location[3] = 1;
  sqverts[2].normal[0] = 0; sqverts[2].normal[1] = 1;  sqverts[2].normal[2] = 0;  sqverts[2].normal[3] = 0;
  sqverts[2].texture[0] = 1.0; sqverts[2].texture[1] = 1.0;
  sqverts[2].color[0] = 0.5; sqverts[2].color[1] = 0.5;  sqverts[2].color[2] = 1;  sqverts[2].color[3] = 1;

  sqverts[3].location[0] = -0.5; sqverts[3].location[1] = 0;  sqverts[3].location[2] = 0.5;  sqverts[3].location[3] = 1;
  sqverts[3].normal[0] = 0; sqverts[3].normal[1] = 1;  sqverts[3].normal[2] = 0;  sqverts[3].normal[3] = 0;
  sqverts[3].texture[0] = 0.0; sqverts[3].texture[1] = 1.0;
  sqverts[3].color[0] =1; sqverts[3].color[1] = 1;  sqverts[3].color[2] = 1;  sqverts[3].color[3] = 1;
/*

  sqverts[4].location[0] = -1.0;  sqverts[4].location[1] = 0;  sqverts[4].location[2] = -0.5;  sqverts[4].location[3] = 1;
  sqverts[4].normal[0] = 0;       sqverts[4].normal[1] = 1;    sqverts[4].normal[2] = 0;       sqverts[4].normal[3] = 0;
  sqverts[4].texture[0] = 1.0;    sqverts[4].texture[1] = 0.0;
  sqverts[4].color[0] =1;         sqverts[4].color[1] = 1;      sqverts[4].color[2] = 1;       sqverts[4].color[3] = 1;


  sqverts[5].location[0] = -1.0;  sqverts[5].location[1] = 0;  sqverts[5].location[2] = 0.5;   sqverts[5].location[3] = 1;
  sqverts[5].normal[0] = 0;       sqverts[5].normal[1] = 1;    sqverts[5].normal[2] = 0;       sqverts[5].normal[3] = 0;
  sqverts[5].texture[0] = 1.0;    sqverts[5].texture[1] = 1.0;
  sqverts[5].color[0] =1;         sqverts[5].color[1] = 1;     sqverts[5].color[2] = 1;        sqverts[5].color[3] = 1;

*/

sqIndices.push_back(0);
sqIndices.push_back(1);
sqIndices.push_back(2);

sqIndices.push_back(2);
sqIndices.push_back(3);
sqIndices.push_back(0);

/*
sqIndices.push_back(4);
sqIndices.push_back(0);
sqIndices.push_back(3);

sqIndices.push_back(3);
sqIndices.push_back(5);
sqIndices.push_back(4);
*/
}

/*
 * create a sphere
 */
void createSphere (float radius,
                   int numSlices,
                   int numStacks,
                   float color[],
                   vector <Vertex> &sphereVerts,
                   vector <GLuint> &sphereIndices)
{

  cout <<"create sphere************************"<<endl;
  // create the sphere
  vector<Vertex> temp;
  float angle = ((180.0/numStacks)* M_PI )/180.0;
  int x=0, y=1,z=2,w=3;
  for (int i=0; i<=numStacks; i++) {
    Vertex v;

    v.location[y]=-1.0*radius*cos(i*angle);
    v.location[x]=sqrt (radius*radius - v.location[y]*v.location[y]);
    v.location[z]=0.0;
    v.location[w]=1.0;
    temp.push_back(v);
  }

  //
  for (int i=0;i<=numSlices;i++){
    float angle = ((i*360.0/numSlices)* M_PI )/180.0;
    //cout <<" angle "<<angle <<endl;
    //float rotMatr[9] = {cos(angle), -1.0*sin(angle),0,sin(angle),cos(angle),0,0,0,1};
    float rotMatr[9] = { cos(angle), 0 , sin(angle),0 ,1 , 0 ,  static_cast<float>(-1.0*sin(angle)), 0, cos(angle)};
    float *p = rotMatr;
    Map<MatrixXf> rotM(p,3,3);
    for (int j=0; j<=numStacks; j++) {
      Vector3f oldPt(temp[j].location[0],temp[j].location[1], temp[j].location[2]);
      Vector3f newpt = rotM.transpose()*oldPt;
      //set up a vertex
      Vertex v;
      for (int d=0;d<3;d++)
        {
        v.location[d]=newpt[d];
        v.color[d]=color[d];
        }
      v.location[3]=1.0;
      v.color[3]=1.0;

      v.normal[0]=0.0;
      v.normal[1]=0.0;
      v.normal[2]=0.0;
      v.normal[3]=0.0;
      sphereVerts.push_back(v);
    }
  }
  int * numNorms = new int [sphereVerts.size()];
  for (int i=0;i<sphereVerts.size();i++)
    numNorms[i]=0;

  //set up the quads / triangles
  for (int i=0; i<numSlices; i++) {
    for (int j=0; j<numStacks; j++) {
      /*
       sphereIndices.push_back(i*(numStacks+1)+j);
       sphereIndices.push_back((i+1)*(numStacks+1)+j);
       sphereIndices.push_back((i+1)*(numStacks+1)+(j+1));
       sphereIndices.push_back(i*(numStacks+1)+(j+1));
       */
       //texture
       if(j%2==0 && i%2==0){
       sphereVerts[i*(numStacks+1)+j].texture[0]=0.0;sphereVerts[i*(numStacks+1)+j].texture[1]=0.0;
       sphereVerts[(i+1)*(numStacks+1)+j].texture[0]=0.0;sphereVerts[(i+1)*(numStacks+1)+j].texture[1]=1.0;

       sphereVerts[i*(numStacks+1)+(j+1)].texture[0]=1.0;sphereVerts[i*(numStacks+1)+(j+1)].texture[1]=1.0;
       sphereVerts[(i+1)*(numStacks+1)+(j+1)].texture[0]=1.0;sphereVerts[(i+1)*(numStacks+1)+(j+1)].texture[1]=0.0;
       }


      sphereIndices.push_back(i*(numStacks+1)+j);
      sphereIndices.push_back((i+1)*(numStacks+1)+j);
      sphereIndices.push_back(i*(numStacks+1)+(j+1));

        POINT a,b;  // these are the vectors
      a.x=sphereVerts[i*(numStacks+1)+j].location[0]-sphereVerts[(i+1)*(numStacks+1)+j].location[0];
      a.y=sphereVerts[i*(numStacks+1)+j].location[1]-sphereVerts[(i+1)*(numStacks+1)+j].location[1];
      a.z=sphereVerts[i*(numStacks+1)+j].location[2]-sphereVerts[(i+1)*(numStacks+1)+j].location[2];

      b.x=sphereVerts[i*(numStacks+1)+(j+1)].location[0]-sphereVerts[(i+1)*(numStacks+1)+j].location[0];
      b.y=sphereVerts[i*(numStacks+1)+(j+1)].location[1]-sphereVerts[(i+1)*(numStacks+1)+j].location[1];
      b.z=sphereVerts[i*(numStacks+1)+(j+1)].location[2]-sphereVerts[(i+1)*(numStacks+1)+j].location[2];

      POINT n;
      n.x = a.y*b.z-a.z*b.y;
      n.y = a.z*b.x-a.x*b.z;
      n.z = a.x*b.y-a.y*b.x;
      normalize(n);


      sphereVerts[i*(numStacks+1)+j].normal[0]+=n.x;
      sphereVerts[i*(numStacks+1)+j].normal[1]+=n.y;
      sphereVerts[i*(numStacks+1)+j].normal[2]+=n.z;
      numNorms[i*(numStacks+1)+j]+=1;

      sphereVerts[(i+1)*(numStacks+1)+j].normal[0]+=n.x;
      sphereVerts[(i+1)*(numStacks+1)+j].normal[1]+=n.y;
      sphereVerts[(i+1)*(numStacks+1)+j].normal[2]+=n.z;
      numNorms[(i+1)*(numStacks+1)+j]+=1;

      sphereVerts[i*(numStacks+1)+(j+1)].normal[0]+=n.x;
      sphereVerts[i*(numStacks+1)+(j+1)].normal[1]+=n.y;
      sphereVerts[i*(numStacks+1)+(j+1)].normal[2]+=n.z;
      numNorms[i*(numStacks+1)+(j+1)]+=1;


      //second triangle
      sphereIndices.push_back(i*(numStacks+1)+(j+1));
      sphereIndices.push_back((i+1)*(numStacks+1)+j);
      sphereIndices.push_back((i+1)*(numStacks+1)+(j+1));

      int pt1 = i*(numStacks+1)+(j+1);
      int pt2 = (i+1)*(numStacks+1)+j;
      int pt3 = (i+1)*(numStacks+1)+(j+1);

      a.x=sphereVerts[pt1].location[0]-sphereVerts[pt2].location[0];
      a.y=sphereVerts[pt1].location[1]-sphereVerts[pt2].location[1];
      a.z=sphereVerts[pt1].location[2]-sphereVerts[pt2].location[2];

      b.x=sphereVerts[pt3].location[0]-sphereVerts[pt2].location[0];
      b.y=sphereVerts[pt3].location[1]-sphereVerts[pt2].location[1];
      b.z=sphereVerts[pt3].location[2]-sphereVerts[pt2].location[2];


      n.x = a.y*b.z-a.z*b.y;
      n.y = a.z*b.x-a.x*b.z;
      n.z = a.x*b.y-a.y*b.x;
      normalize(n);


      sphereVerts[pt3].normal[0]+=n.x;
      sphereVerts[pt3].normal[1]+=n.y;
      sphereVerts[pt3].normal[2]+=n.z;
      numNorms[pt3]+=1;

      sphereVerts[pt2].normal[0]+=n.x;
      sphereVerts[pt2].normal[1]+=n.y;
      sphereVerts[pt2].normal[2]+=n.z;
      numNorms[pt2]+=1;

      sphereVerts[pt1].normal[0]+=n.x;
      sphereVerts[pt1].normal[1]+=n.y;
      sphereVerts[pt1].normal[2]+=n.z;
      numNorms[pt1]+=1;


    }
  }

  for (int i=0;i<sphereVerts.size();i++)
    {
    for (int d=0;d<3;d++)
    sphereVerts[i].normal[d]/= (1.0*numNorms[i]);
    sphereVerts[i].normal[3]=1.0;
    }

}
/*******
 Conics helper functions
 *******/
void normalize (float * vec , const int size)
{
  float mag = 0.0;
  float sum=0.0;
  for (int j=0;j<size;j++)
    {
    sum = sum + (vec[j]*vec[j]);
    }
  mag = sqrt (sum);
  if (abs(mag-0.0) > 0.001)
    for (int j=0;j<size;j++)
      {
      vec[j]=vec[j]/mag;
      }
  else
    {
    for (int j=0;j<size;j++)
      {
      vec[j]=0.0;
      }
    }
}

void computeUDist (float * a, float * b, int numStacks, float & udist)
{
  float sum=0.0;
  for (int j=0; j<3 ;j++)
    {
    sum = sum + (b[j]-a[j])*(b[j]-a[j]);
    }
  udist = sqrt(sum)/float(numStacks);
}
/*
void createConics (float BaseRadius,
		float TopRadius,
		float height,
		int numSlices,
		int numStacks,
		float color[],
		vector <Vertex> &conicsVerts,
		vector <GLuint> &conicsIndices)
{

  //
  float a[3] = {TopRadius,height,0};
  float b[3] = {BaseRadius,0,0};

  float baDir[3] = {0.0,0.0,0.0};
  for (int i=0;i<3;i++)
    { baDir[i]=a[i]-b[i]; }
  normalize(baDir, 3);



  float udist(0.0);
  computeUDist (a,b,numStacks,udist);
  //

  vector<Vertex> temp;
  int x=0, y=1,z=2,w=3;
  for (int i=0; i<=numStacks; i++) {

    Vertex v;
    v.location[y]=b[y]+i*baDir[y]*udist*1.0;
    v.location[x]=b[x]+i*baDir[x]*udist*1.0;
    v.location[z]=b[z]+i*baDir[z]*udist*1.0;
    v.location[w]=1.0;
    temp.push_back(v);
  }
  //conics
  //
  for (int i=0;i<=numSlices;i++){
    float angle = ((i*360.0/numSlices)* M_PI )/180.0;
    //cout <<" angle "<<angle <<endl;
    //float rotMatr[9] = {cos(angle), -1.0*sin(angle),0,sin(angle),cos(angle),0,0,0,1};
    float rotMatr[9] = { cos(angle), 0 , sin(angle),0 ,1 , 0 ,  static_cast<float>(-1.0*sin(angle)), 0, cos(angle)};
    float *p = rotMatr;
    Map<MatrixXf> rotM(p,3,3);
    for (int j=0; j<=numStacks; j++) {
      Vector3f oldPt(temp[j].location[0],temp[j].location[1], temp[j].location[2]);
      Vector3f newpt = rotM.transpose()*oldPt;
      //set up a vertex
      Vertex v;
      for (int d=0;d<3;d++)
        {
        v.location[d]=newpt[d];
        v.color[d]=color[d];
        }
      v.location[3]=1.0;
      v.color[3]=1.0;
      conicsVerts.push_back(v);
    }
  }
    int * numNorms = new int [conicsVerts.size()];
  //set up the quads / triangles
  for (int i=0; i<numSlices; i++) {
    for (int j=0; j<numStacks; j++) {
      //
      // sphereIndices.push_back(i*(numStacks+1)+j);
       //sphereIndices.push_back((i+1)*(numStacks+1)+j);
      // sphereIndices.push_back((i+1)*(numStacks+1)+(j+1));
      // sphereIndices.push_back(i*(numStacks+1)+(j+1));
      //
     if(j%2==0 && i%2==0 ){
       conicsVerts[i*(numStacks+1)+j].texture[0]=0.0;conicsVerts[i*(numStacks+1)+j].texture[1]=0.0;
       conicsVerts[(i+1)*(numStacks+1)+j].texture[0]=0.0;conicsVerts[(i+1)*(numStacks+1)+j].texture[1]=1.0;

       conicsVerts[i*(numStacks+1)+(j+1)].texture[0]=1.0;conicsVerts[i*(numStacks+1)+(j+1)].texture[1]=0.0;
       conicsVerts[(i+1)*(numStacks+1)+(j+1)].texture[0]=1.0;conicsVerts[(i+1)*(numStacks+1)+(j+1)].texture[1]=1.0;
       }



      conicsIndices.push_back(i*(numStacks+1)+j);
      conicsIndices.push_back((i+1)*(numStacks+1)+j);
      conicsIndices.push_back(i*(numStacks+1)+(j+1));
      POINT a,b,n;
      int pt1 = (i*(numStacks+1)+j);
      int pt2 = (i+1)*(numStacks+1)+j;
      int pt3 = i*(numStacks+1)+(j+1);

      a.x=conicsVerts[pt2].location[0]-conicsVerts[pt1].location[0];
      a.y=conicsVerts[pt2].location[1]-conicsVerts[pt1].location[1];
      a.z=conicsVerts[pt2].location[2]-conicsVerts[pt1].location[2];

      b.x=conicsVerts[pt3].location[0]-conicsVerts[pt1].location[0];
      b.y=conicsVerts[pt3].location[1]-conicsVerts[pt1].location[1];
      b.z=conicsVerts[pt3].location[2]-conicsVerts[pt1].location[2];


      n.x = a.y*b.z-a.z*b.y;
      n.y = a.z*b.x-a.x*b.z;
      n.z = a.x*b.y-a.y*b.x;
      normalize(n);


      conicsVerts[pt3].normal[0]+=n.x;
      conicsVerts[pt3].normal[1]+=n.y;
      conicsVerts[pt3].normal[2]+=n.z;
      numNorms[pt3]+=1;

      conicsVerts[pt2].normal[0]+=n.x;
      conicsVerts[pt2].normal[1]+=n.y;
      conicsVerts[pt2].normal[2]+=n.z;
      numNorms[pt2]+=1;

      conicsVerts[pt1].normal[0]+=n.x;
      conicsVerts[pt1].normal[1]+=n.y;
      conicsVerts[pt1].normal[2]+=n.z;
      numNorms[pt1]+=1;

      conicsIndices.push_back(i*(numStacks+1)+(j+1));
      conicsIndices.push_back((i+1)*(numStacks+1)+j);
      conicsIndices.push_back((i+1)*(numStacks+1)+(j+1));
      //

      pt2=(i*(numStacks+1)+(j+1));
      pt3=(i+1)*(numStacks+1)+j;
      pt1=(i+1)*(numStacks+1)+(j+1);

      a.x=conicsVerts[pt2].location[0]-conicsVerts[pt1].location[0];
      a.y=conicsVerts[pt2].location[1]-conicsVerts[pt1].location[1];
      a.z=conicsVerts[pt2].location[2]-conicsVerts[pt1].location[2];

      b.x=conicsVerts[pt3].location[0]-conicsVerts[pt1].location[0];
      b.y=conicsVerts[pt3].location[1]-conicsVerts[pt1].location[1];
      b.z=conicsVerts[pt3].location[2]-conicsVerts[pt1].location[2];


      n.x = a.y*b.z-a.z*b.y;
      n.y = a.z*b.x-a.x*b.z;
      n.z = a.x*b.y-a.y*b.x;
      normalize(n);


      conicsVerts[pt3].normal[0]+=n.x;
      conicsVerts[pt3].normal[1]+=n.y;
      conicsVerts[pt3].normal[2]+=n.z;
      numNorms[pt3]+=1;

      conicsVerts[pt2].normal[0]+=n.x;
      conicsVerts[pt2].normal[1]+=n.y;
      conicsVerts[pt2].normal[2]+=n.z;
      numNorms[pt2]+=1;

      conicsVerts[pt1].normal[0]+=n.x;
      conicsVerts[pt1].normal[1]+=n.y;
      conicsVerts[pt1].normal[2]+=n.z;
      numNorms[pt1]+=1;


    }
  }
//
  for (int i=0;i<conicsVerts.size();i++)
    {
    for (int d=0;d<3;d++)
    conicsVerts[i].normal[d]/= (1.0*numNorms[i]);
    conicsVerts[i].normal[3]=1.0;
    }

}
*/

void createConics (float BaseRadius, float TopRadius, float height, int numSlices, int numStacks, float color[],
                   vector <Vertex> &conicsVerts, vector <GLuint> &conicsIndices)
{

  //
  float a[3] = {TopRadius,height,0};
  float b[3] = {BaseRadius,0,0};

  float baDir[3] = {0.0,0.0,0.0};
  for (int i=0;i<3;i++)
    { baDir[i]=a[i]-b[i]; }
  normalize(baDir, 3);



  float udist(0.0);
  computeUDist (a,b,numStacks,udist);
  //

  vector<Vertex> temp;
  int x=0, y=1,z=2,w=3;
  for (int i=0; i<=numStacks; i++) {

    Vertex v;
    v.location[y]=b[y]+i*baDir[y]*udist*1.0;
    v.location[x]=b[x]+i*baDir[x]*udist*1.0;
    v.location[z]=b[z]+i*baDir[z]*udist*1.0;
    v.location[w]=1.0;
    temp.push_back(v);
  }
  //conics
  //
  for (int i=0;i<=numSlices;i++){
    float angle = ((i*360.0/numSlices)* M_PI )/180.0;
    //cout <<" angle "<<angle <<endl;
    //float rotMatr[9] = {cos(angle), -1.0*sin(angle),0,sin(angle),cos(angle),0,0,0,1};
    float rotMatr[9] = { cos(angle), 0 , sin(angle),0 ,1 , 0 ,  static_cast<float>(-1.0*sin(angle)), 0, cos(angle)};
    float *p = rotMatr;
    Map<MatrixXf> rotM(p,3,3);
    for (int j=0; j<=numStacks; j++) {
      Vector3f oldPt(temp[j].location[0],temp[j].location[1], temp[j].location[2]);
      Vector3f newpt = rotM.transpose()*oldPt;
      //set up a vertex
      Vertex v;
      for (int d=0;d<3;d++)
        {
        v.location[d]=newpt[d];
        v.color[d]=color[d];
        }
      v.location[3]=1.0;
      v.color[3]=1.0;
      conicsVerts.push_back(v);
    }
  }
    int * numNorms = new int [conicsVerts.size()];
  //set up the quads / triangles
  for (int i=0; i<numSlices; i++) {
    for (int j=0; j<numStacks; j++) {
      //
      // sphereIndices.push_back(i*(numStacks+1)+j);
       //sphereIndices.push_back((i+1)*(numStacks+1)+j);
      // sphereIndices.push_back((i+1)*(numStacks+1)+(j+1));
      // sphereIndices.push_back(i*(numStacks+1)+(j+1));
      //
     if(j%2==0 && i%2==0 ){
       conicsVerts[i*(numStacks+1)+j].texture[0]=0.0;conicsVerts[i*(numStacks+1)+j].texture[1]=0.0;
       conicsVerts[(i+1)*(numStacks+1)+j].texture[0]=0.0;conicsVerts[(i+1)*(numStacks+1)+j].texture[1]=1.0;

       conicsVerts[i*(numStacks+1)+(j+1)].texture[0]=1.0;conicsVerts[i*(numStacks+1)+(j+1)].texture[1]=0.0;
       conicsVerts[(i+1)*(numStacks+1)+(j+1)].texture[0]=1.0;conicsVerts[(i+1)*(numStacks+1)+(j+1)].texture[1]=1.0;
       }



      conicsIndices.push_back(i*(numStacks+1)+j);
      conicsIndices.push_back((i+1)*(numStacks+1)+j);
      conicsIndices.push_back(i*(numStacks+1)+(j+1));
      POINT a,b,n;
      int pt1 = (i*(numStacks+1)+j);
      int pt2 = (i+1)*(numStacks+1)+j;
      int pt3 = i*(numStacks+1)+(j+1);

      a.x=conicsVerts[pt2].location[0]-conicsVerts[pt1].location[0];
      a.y=conicsVerts[pt2].location[1]-conicsVerts[pt1].location[1];
      a.z=conicsVerts[pt2].location[2]-conicsVerts[pt1].location[2];

      b.x=conicsVerts[pt3].location[0]-conicsVerts[pt1].location[0];
      b.y=conicsVerts[pt3].location[1]-conicsVerts[pt1].location[1];
      b.z=conicsVerts[pt3].location[2]-conicsVerts[pt1].location[2];


      n.x = a.y*b.z-a.z*b.y;
      n.y = a.z*b.x-a.x*b.z;
      n.z = a.x*b.y-a.y*b.x;
      normalize(n);


      conicsVerts[pt3].normal[0]+=n.x;
      conicsVerts[pt3].normal[1]+=n.y;
      conicsVerts[pt3].normal[2]+=n.z;
      numNorms[pt3]+=1;

      conicsVerts[pt2].normal[0]+=n.x;
      conicsVerts[pt2].normal[1]+=n.y;
      conicsVerts[pt2].normal[2]+=n.z;
      numNorms[pt2]+=1;

      conicsVerts[pt1].normal[0]+=n.x;
      conicsVerts[pt1].normal[1]+=n.y;
      conicsVerts[pt1].normal[2]+=n.z;
      numNorms[pt1]+=1;

      conicsIndices.push_back(i*(numStacks+1)+(j+1));
      conicsIndices.push_back((i+1)*(numStacks+1)+j);
      conicsIndices.push_back((i+1)*(numStacks+1)+(j+1));
      //

      pt2=(i*(numStacks+1)+(j+1));
      pt3=(i+1)*(numStacks+1)+j;
      pt1=(i+1)*(numStacks+1)+(j+1);

      a.x=conicsVerts[pt2].location[0]-conicsVerts[pt1].location[0];
      a.y=conicsVerts[pt2].location[1]-conicsVerts[pt1].location[1];
      a.z=conicsVerts[pt2].location[2]-conicsVerts[pt1].location[2];

      b.x=conicsVerts[pt3].location[0]-conicsVerts[pt1].location[0];
      b.y=conicsVerts[pt3].location[1]-conicsVerts[pt1].location[1];
      b.z=conicsVerts[pt3].location[2]-conicsVerts[pt1].location[2];


      n.x = a.y*b.z-a.z*b.y;
      n.y = a.z*b.x-a.x*b.z;
      n.z = a.x*b.y-a.y*b.x;
      normalize(n);


      conicsVerts[pt3].normal[0]+=n.x;
      conicsVerts[pt3].normal[1]+=n.y;
      conicsVerts[pt3].normal[2]+=n.z;
      numNorms[pt3]+=1;

      conicsVerts[pt2].normal[0]+=n.x;
      conicsVerts[pt2].normal[1]+=n.y;
      conicsVerts[pt2].normal[2]+=n.z;
      numNorms[pt2]+=1;

      conicsVerts[pt1].normal[0]+=n.x;
      conicsVerts[pt1].normal[1]+=n.y;
      conicsVerts[pt1].normal[2]+=n.z;
      numNorms[pt1]+=1;


    }
  }
//
  for (int i=0;i<conicsVerts.size();i++)
    {
    for (int d=0;d<3;d++)
    conicsVerts[i].normal[d]/= (1.0*numNorms[i]);
    conicsVerts[i].normal[3]=1.0;
    }

}






void createCube(vector <Vertex> &sqverts,
        vector <GLuint> &sqIndices)
{
	   Vertex v;
	  for (int i=0;i<24;i++)
	  sqverts.push_back(v);
	  //front
	  sqverts[0].location[0] = -1.0; sqverts[0].location[1] = -1.0;  sqverts[0].location[2] = 1.0;  sqverts[0].location[3] = 1.0;
	  sqverts[1].location[0] = 1.0; sqverts[1].location[1] = -1.0;  sqverts[1].location[2] = 1.0;  sqverts[1].location[3] = 1.0;
	  sqverts[2].location[0] = 1.0; sqverts[2].location[1] = 1.0;  sqverts[2].location[2] = 1.0;  sqverts[2].location[3] = 1.0;
	  sqverts[3].location[0] = -1.0; sqverts[3].location[1] = 1.0;  sqverts[3].location[2] = 1.0;  sqverts[3].location[3] = 1.0;

	  // top
	  sqverts[4].location[0] = -1.0; sqverts[4].location[1] = 1.0;  sqverts[4].location[2] = 1.0;  sqverts[4].location[3] = 1.0;
	  sqverts[5].location[0] = 1.0; sqverts[5].location[1] = 1.0;  sqverts[5].location[2] = 1.0;  sqverts[5].location[3] = 1.0;
	  sqverts[6].location[0] = 1.0; sqverts[6].location[1] = 1.0;  sqverts[6].location[2] = -1.0;  sqverts[6].location[3] = 1.0;
	  sqverts[7].location[0] = -1.0; sqverts[7].location[1] = 1.0;  sqverts[7].location[2] = -1.0;  sqverts[7].location[3] = 1.0;

	  // back
	  sqverts[8].location[0] = 1.0; sqverts[8].location[1] = -1.0;  sqverts[8].location[2] = -1.0;  sqverts[8].location[3] = 1.0;
	  sqverts[9].location[0] = -1.0; sqverts[9].location[1] = -1.0;  sqverts[9].location[2] = -1.0;  sqverts[9].location[3] = 1.0;
	  sqverts[10].location[0] = -1.0; sqverts[10].location[1] = 1.0;  sqverts[10].location[2] = -1.0;  sqverts[10].location[3] = 1.0;
	  sqverts[11].location[0] = 1.0; sqverts[11].location[1] = 1.0;  sqverts[11].location[2] = -1.0;  sqverts[11].location[3] = 1.0;

	  //bottom
	  sqverts[12].location[0] = -1.0; sqverts[12].location[1] = -1.0;  sqverts[12].location[2] = -1.0;  sqverts[12].location[3] = 1.0;
	  sqverts[13].location[0] = 1.0; sqverts[13].location[1] = -1.0;  sqverts[13].location[2] = -1.0;  sqverts[13].location[3] = 1.0;
	  sqverts[14].location[0] = 1.0; sqverts[14].location[1] = -1.0;  sqverts[14].location[2] = 1.0;  sqverts[14].location[3] = 1.0;
	  sqverts[15].location[0] = -1.0; sqverts[15].location[1] = -1.0;  sqverts[15].location[2] = 1.0;  sqverts[15].location[3] = 1.0;
	  //left
	  sqverts[16].location[0] = -1.0; sqverts[16].location[1] = -1.0;  sqverts[16].location[2] = -1.0;  sqverts[16].location[3] = 1.0;
	  sqverts[17].location[0] = -1.0; sqverts[17].location[1] = -1.0;  sqverts[17].location[2] = 1.0;  sqverts[17].location[3] = 1.0;
	  sqverts[18].location[0] = -1.0; sqverts[18].location[1] = 1.0;  sqverts[18].location[2] = 1.0;  sqverts[18].location[3] = 1.0;
	  sqverts[19].location[0] = -1.0; sqverts[19].location[1] = 1.0;  sqverts[19].location[2] = -1.0;  sqverts[19].location[3] = 1.0;
	  //right
	  sqverts[20].location[0] = 1.0; sqverts[20].location[1] = -1.0;  sqverts[20].location[2] = 1.0;  sqverts[20].location[3] = 1.0;
	  sqverts[21].location[0] = 1.0; sqverts[21].location[1] = -1.0;  sqverts[21].location[2] = -1.0;  sqverts[21].location[3] = 1.0;
	  sqverts[22].location[0] = 1.0; sqverts[22].location[1] = 1.0;  sqverts[22].location[2] = -1.0;  sqverts[22].location[3] = 1.0;
	  sqverts[23].location[0] = 1.0; sqverts[23].location[1] = 1.0;  sqverts[23].location[2] = 1.0;  sqverts[23].location[3] = 1.0;

	  //normal
	  for (int i = 0; i < 4; i++)
		  {sqverts[i].normal[0] = 0.0; sqverts[i].normal[1] = 0.0;sqverts[i].normal[2] = 1.0; sqverts[i].normal[3] = 0.0;}
	  for (int i = 0; i < 4; i++)
	  		  {sqverts[i+4].normal[0] = 0.0; sqverts[i+4].normal[1] = 1.0;sqverts[i+4].normal[2] = 0.0; sqverts[i+4].normal[3] = 0.0;}
	  for (int i = 0; i < 4; i++)
	  		  {sqverts[i+8].normal[0] = 0.0; sqverts[i+8].normal[1] = 0.0;sqverts[i+8].normal[2] = -1.0; sqverts[i+8].normal[3] = 0.0;}
	  for (int i = 0; i < 4; i++)
	  		  {sqverts[i+12].normal[0] = 0.0; sqverts[i+12].normal[1] = -1.0;sqverts[i+12].normal[2] = 0.0; sqverts[i+12].normal[3] = 0.0;}
	  for (int i = 0; i < 4; i++)
	  		  {sqverts[i+16].normal[0] = -1.0; sqverts[i+16].normal[1] = 0.0;sqverts[i+16].normal[2] = 0.0; sqverts[i+16].normal[3] = 0.0;}
	  for (int i = 0; i < 4; i++)
	  		  {sqverts[i+20].normal[0] = 1.0; sqverts[i+20].normal[1] = 0.0;sqverts[i+20].normal[2] = 0.0; sqverts[i+20].normal[3] = 0.0;}

	  //texcoordinates
	  for (int i = 0; i < 6; i++){
		  sqverts[i*4].texture[0] = 0.0; sqverts[i*4].texture[1] = 0.0;
		  sqverts[i*4+1].texture[0] = 1.0; sqverts[i*4+1].texture[1] = 0.0;
		  sqverts[i*4+2].texture[0] = 1.0; sqverts[i*4+2].texture[1] = 1.0;
		  sqverts[i*4+3].texture[0] = 0.0; sqverts[i*4+3].texture[1] = 1.0;
	  }

	  // Triangulate
	  // front
	  sqIndices.push_back(0);
	  sqIndices.push_back(1);
	  sqIndices.push_back(2);
	  sqIndices.push_back(2);
	  sqIndices.push_back(3);
	  sqIndices.push_back(0);
	  // top
	  sqIndices.push_back(4);
	  sqIndices.push_back(5);
	  sqIndices.push_back(6);
	  sqIndices.push_back(6);
	  sqIndices.push_back(7);
	  sqIndices.push_back(4);
	  // back
	  sqIndices.push_back(8);
	  sqIndices.push_back(9);
	  sqIndices.push_back(10);
	  sqIndices.push_back(10);
	  sqIndices.push_back(11);
	  sqIndices.push_back(8);
	  // bottom
	  sqIndices.push_back(12);
	  sqIndices.push_back(13);
	  sqIndices.push_back(14);
	  sqIndices.push_back(14);
	  sqIndices.push_back(15);
	  sqIndices.push_back(12);
	  //left
	  sqIndices.push_back(16);
	  sqIndices.push_back(17);
	  sqIndices.push_back(18);
	  sqIndices.push_back(18);
	  sqIndices.push_back(19);
	  sqIndices.push_back(16);
	  //right
	  sqIndices.push_back(20);
	  sqIndices.push_back(21);
	  sqIndices.push_back(22);
	  sqIndices.push_back(22);
	  sqIndices.push_back(23);
	  sqIndices.push_back(20);


	  for (int i = 0; i<24; i++)
	  {
		  for (int d =0; d <4; d++)
		  {
			  sqverts[i].color[d] = 1;
		  }
	  }
		  for (int i = 0; i < 36; i+=3){

			  int i1 = sqIndices[i];
			  int i2 = sqIndices[i+1];
			  int i3 = sqIndices[i+2];
			  glm::vec3 p1(sqverts[i1].location[0],sqverts[i1].location[1],sqverts[i1].location[2] );
			  glm::vec3 p2(sqverts[i2].location[0],sqverts[i2].location[1],sqverts[i2].location[2] );
			  glm::vec3 p3(sqverts[i3].location[0],sqverts[i3].location[1],sqverts[i3].location[2] );
			  glm::vec2 uv1(sqverts[i1].texture[0], sqverts[i1].texture[1]);
			  glm::vec2 uv2(sqverts[i2].texture[0], sqverts[i2].texture[1]);
			  glm::vec2 uv3(sqverts[i3].texture[0], sqverts[i3].texture[1]);

			  glm::vec3 p1p2 = p2 - p1;
			  glm::vec3 p1p3 = p3 - p1;
			  glm::vec2 uv1uv2 = uv2 - uv1;
			  glm::vec2 uv1uv3 = uv3 - uv1;

			  float c = uv1uv2.s * uv1uv3.t - uv1uv3.s * uv1uv2.t;
			  if (c != 0) {
			          float mul = 1.0 / c;
			          glm::vec3 tangent = (p1p2 * uv1uv3.t - p1p3 * uv1uv2.t) * mul;
			          tangent = glm::normalize(tangent);
			          sqverts[i1].tangents[0] = sqverts[i2].tangents[0] = sqverts[i3].tangents[0] = tangent[0];
			          sqverts[i1].tangents[1] = sqverts[i2].tangents[1] = sqverts[i3].tangents[1] = tangent[1];
			          sqverts[i1].tangents[2] = sqverts[i2].tangents[2] = sqverts[i3].tangents[2] = tangent[2];
			          sqverts[i1].tangents[3] = sqverts[i2].tangents[3] = sqverts[i3].tangents[3] = 0.0;

			  }

		  }
}




















