/*
 * read an off file

 */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <iterator>
#include <algorithm>
#include "ReadOFF.h"

#include "standard_objs.h"

using namespace std;


void normalize(POINT & n)
{
	double mag = sqrt(n.x*n.x  +  n.y*n.y + n.z*n.z);
	if (mag > 0.0)
	{
		n.x = n.x/mag;
		n.y = n.y/mag;
		n.z = n.z/mag;
	}
}

void readMeshData(const string fname,vector <Vertex> &Verts,
		vector <GLuint> &Indices)
{
	// read a mesh data
	readOFF offFile;
	offFile.readData(fname);
	OBJECT obj (offFile.tokens);

	//0 th object

	for (int m=0; m < obj.numVerts; m++) {
		Vertex v;
		v.location[0]=obj.points[m].x;
		v.location[1]=obj.points[m].y;
		v.location[2]=obj.points[m].z;
		v.location[3]=1.0;


		v.color[0]=1.0;
		v.color[1]=0.0;
		v.color[2]=0.0;
		v.color[3]=1.0;

		v.normal[0]=obj.points[m].n1;
		v.normal[1]=obj.points[m].n2;
		v.normal[2]=obj.points[m].n3;
		v.normal[3]=1.0;


		Verts.push_back(v);
	}

	for (int n=0; n<obj.numFaces; n++) {
		Indices.push_back(obj.faces[n].a);
		Indices.push_back(obj.faces[n].b);
		Indices.push_back(obj.faces[n].c);
	}
}
void readOFF::readData(const string &inputFname)
{
	string line;
	//ifstream myfile ("/home/arindam/Course/lab3/cse5542Lab3/dragon.off");
	ifstream myfile(inputFname.c_str());
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,line);
			//cout << line << endl;
			istringstream iss(line);
			copy(istream_iterator<string>(iss),
					istream_iterator<string>(),
					back_inserter<vector<string> >(tokens));
		}
		myfile.close();
	}

	else cout << "Unable to open file";

}


// constructor for OBJECT

OBJECT::OBJECT(const vector<string> &tokens)
{
	//set the number of obj.faces and vertices
	numVerts = atoi(tokens[1].c_str());
	numFaces = atoi(tokens[2].c_str());

	points = new POINT [numVerts];
	faces = new FACE [numFaces];

	cout <<" size of tokes " << tokens.size()<<endl;
	cout <<" num obj.faces "<< numFaces<<endl;
	cout <<" num verts "<< numVerts<<endl;
	int ind =4;
	for (int n=0; n<numVerts; n++)
	{

		POINT p;
		p.numNorms=0;
		p.n1=0.0;
		p.n2=0.0;
		p.n3=0.0;
		//cout <<" n "<<n<<" "<<tokens[ind].c_str();
		p.x = atof(tokens[ind].c_str());
		ind++;
		//cout <<" "<<tokens[ind].c_str();
		p.y = atof(tokens[ind].c_str());
		ind++;
		//cout <<" "<<tokens[ind].c_str()<<endl;
		p.z = atof(tokens[ind].c_str());
		ind++;

		points[n]=p;
	}



	for (int n=0; n<numFaces; n++)
	{
		ind++;
		FACE f;

		f.a  = atoi(tokens[ind].c_str());
		ind++;
		f.b  = atoi(tokens[ind].c_str());
		ind++;
		f.c  = atoi(tokens[ind].c_str());
		ind++;

		faces[n]=f;
		//cout <<" faces "<<faces[n].a <<" "<<faces[n].b<<" "<<faces[n].c<<endl;
	}

	// compute normals
	for (int m=0; m<numFaces; m++)
	{
		FACE f =  faces[m];
		POINT a,b;  // these are the vectors

		a.x = points[f.b].x - points[f.a].x;
		a.y = points[f.b].y - points[f.a].y;
		a.z = points[f.b].z - points[f.a].z;

		b.x = points[f.c].x - points[f.a].x;
		b.y = points[f.c].y - points[f.a].y;
		b.z = points[f.c].z - points[f.a].z;

		POINT n;
		n.x = a.y*b.z-a.z*b.y;
		n.y = a.z*b.x-a.x*b.z;
		n.z = a.x*b.y-a.y*b.x;
		normalize(n);

		points[faces[m].a].n1 += n.x;
		points[faces[m].a].n2 += n.y;
		points[faces[m].a].n3 += n.z;

		points[faces[m].a].numNorms++;

		points[faces[m].b].n1 += n.x;
		points[faces[m].b].n2 += n.y;
		points[faces[m].b].n3 += n.z;

		points[faces[m].b].numNorms++;

		points[faces[m].c].n1 += n.x;
		points[faces[m].c].n2 += n.y;
		points[faces[m].c].n3 += n.z;

		points[faces[m].c].numNorms++;
	}

	for (int m=0; m<numVerts; m++)
	{
		if (points[m].numNorms>0)
		{
			points[m].n1 /=  (-1.0*points[m].numNorms);
			points[m].n2 /=(-1.0*points[m].numNorms);
			points[m].n3 /=(-1.0*points[m].numNorms);
		}
	}

}
