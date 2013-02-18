////////////////////////////////////////////////////////////////////////////////////////////////
//
// The purpose of this program is to use vertex buffer objects (VBOs) to define a simple 3D cube
// Note that we will not use glBegin/End any more since they have been deprecated in OpenGL 3.x
//
// A very simple shader (cube_simple.{vert,frag}) is uesd in this example
//
// In this example, the triangles vertices and colors are listed in separate arrays and VBOs. We also
// use a triangle index array (GL_ELEMENT_ARRAY_BUFFER) to assemble triangles
// vertices. As a result, we use glDrawArrays() to draw.
//
// Simple 3D viewing is set up, using the OpenGL mathematics (glm) library. This is to replace the
// OpenGL 2.x methods (GL_MODELVIEW, GL_PROJECTIon, etc) that have been deprecated.
//
// Han-Wei Shen, July 2012 @ Ohio State University
// hwshen@cse.ohio-state.edu
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <glm/glm.hpp>

#include "ReadOFF.h"
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "tga.c"

#define RENDER_WIDTH 640.0
#define RENDER_HEIGHT 480.0
extern bool use_geom_shader;

void init2DTexture(GLint texName, GLint texWidth,
		GLint texHeight, GLubyte *texPtr);
GLubyte *cubeMapImages[6]; // cube mapping texture

GLubyte readImage[990][990][4];
const int textureL = 100;
GLubyte texImage[textureL][textureL][4];
typedef enum {SQUARE,  SPHERE, CYL,CUBE, BUNNY} shapes;
int height, width, ccv;
const int num_of_objects=5;
vector<vector<Vertex> > vertxLists;
vector<vector<GLuint> > tindicesLists;
GLuint vboHandle[num_of_objects];
GLuint indexVBO[num_of_objects];

///// shadow
GLuint depthTextureID;
GLuint fboId;

GLuint simple_texture;
GLuint grass_texture;
GLuint normal_texture;
GLuint normal_texture2;
#define SHADOW_MAP_RATIO 2


GLuint cubemap_texture;

GLuint programObject;
GLuint SetupGLSL(char*);

GLuint vao;

void display_object(const shapes s, GLuint &c0, GLuint &c1, GLuint &c2, GLuint &c3);

/////////////////////////////////
// glut mouse control
//
int xform_mode = 0;
#define XFORM_NONE    0
#define XFORM_ROTATE  1
#define XFORM_SCALE 2

int press_x, press_y;
int release_x, release_y;
float x_angle = 0.0;
float y_angle = 0.0;
float scale_size = 1;
float turn = 0.0;
float LAangle =0.0;


//http://www.opengl.org/discussion_boards/showthread.php/176961-Modelview-translation-with-GLM
//////////////////////////////////////////////////////////////////////////////////
//
//  Just hard code the geometry
//
void InitGeometry()
{
	vector <Vertex> verts;
	vector <GLuint> indices;

	GLfloat b[3]= {0.0,0.0,1.0};
	GLfloat r[3]= {1.0,0.0,0.0};

	InitSquare(verts,indices);
	cout <<" verts size "<<verts.size()<<" indice "<<indices.size()<<endl;
	vertxLists.push_back(verts);
	tindicesLists.push_back(indices);
	verts.clear();
	indices.clear();

	createSphere(3,20,20,r,verts,indices);
	vertxLists.push_back(verts);
	tindicesLists.push_back(indices);
	verts.clear();
	indices.clear();

	createConics(0.5,0.5,2.0,12,12,r,verts,indices);
	vertxLists.push_back(verts);
	tindicesLists.push_back(indices);
	verts.clear();
	indices.clear();

	createCube(verts,indices);
	vertxLists.push_back(verts);
	tindicesLists.push_back(indices);
	verts.clear();
	indices.clear();

	readMeshData("/home/arindam/Course/lab3/cse5542Lab3/dragon.off",verts,indices);
	vertxLists.push_back(verts);
	tindicesLists.push_back(indices);
	verts.clear();
	indices.clear();
	/*
        // square
        InitSquare(verts,indices);
        cout <<" verts size "<<verts.size()<<" indice "<<indices.size()<<endl;
        vertxLists.push_back(verts);
        tindicesLists.push_back(indices);
        verts.clear();
        indices.clear();
	 */
	/*
    readMeshData("/home/arindam/Course/lab3/cse5542Lab3/dragon.off",verts,indices);
    vertxLists.push_back(verts);
    tindicesLists.push_back(indices);
    verts.clear();
    indices.clear();

    createSphere(2,20,20,r,verts,indices);
    vertxLists.push_back(verts);
    tindicesLists.push_back(indices);
    verts.clear();
    indices.clear();

    createConics(0.5,0.5,2.0,10,10,r,verts,indices);
    vertxLists.push_back(verts);
    tindicesLists.push_back(indices);
    verts.clear();
    indices.clear();

    createConics(1.0,1.0,2.0,10,10,r,verts,indices);
    vertxLists.push_back(verts);
    tindicesLists.push_back(indices);
    verts.clear();
    indices.clear();

    readMeshData("/home/arindam/Course/lab3/cse5542Lab3/bunny.off",verts,indices);
    vertxLists.push_back(verts);
    tindicesLists.push_back(indices);
    verts.clear();
    indices.clear();
	 */
}
GLuint FramebufferName=0;
GLuint renderedTexture=0;
GLuint depthTexture=0;


void display_object(const shapes s, GLuint &c0, GLuint &c1, GLuint &c2, GLuint &c3)
{
	cout <<" s "<<int(s)<<endl;
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[s]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[s]);

	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0);
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16);
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);

	glDrawElements(GL_TRIANGLES, tindicesLists[s].size(), GL_UNSIGNED_INT, (char*) NULL+0);

}
glm::mat4 modelCurr = glm::mat4(1.0f);




GLhandleARB shadowShaderId;
GLuint shadowMapUniform;


void generateShadowFBO()
{

	int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_RATIO;
	int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_RATIO;

	//GLfloat borderColor[4] = {0,0,0,0};

	GLenum FBOstatus;

	// Try to use a texture depth component
	glGenTextures(1, &depthTextureID);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	//glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );



	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
			GL_TEXTURE_2D, depthTextureID, 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void draw_sphere(
		const glm::mat4 &modelTemp,
		const glm::mat4 &view,
		const glm::mat4 &projection,
		GLuint &c0,
		GLuint &c1,
		GLuint &c2,
		GLuint &c3
){
	glm::mat4 modelview;
	glm::mat4 modelview_projection;
	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	GLuint m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	GLuint m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	GLuint m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	display_object(SPHERE,c0,c1,c2,c3);
}
//Camera position
float p_camera[3] = {32,20,0};

//Camera lookAt
float l_camera[3] = {2,0,-10};

//Light position
float p_light[3] = {3,20,0};

//Light lookAt
float l_light[3] = {0,0,-5};
void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,RENDER_WIDTH/RENDER_HEIGHT,10,40000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
}
void startTranslate(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);

	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(x,y,z);
}

void endTranslate()
{
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawObjects(void)
{
	// Ground
	glColor4f(0.3f,0.3f,0.3f,1);
	glBegin(GL_QUADS);
	glVertex3f(-35,2,-35);
	glVertex3f(-35,2, 15);
	glVertex3f( 15,2, 15);
	glVertex3f( 15,2,-35);
	glEnd();

	glColor4f(0.9f,0.9f,0.9f,1);

	// Instead of calling glTranslatef, we need a custom function that also maintain the light matrix
	startTranslate(0,4,-16);
	glutSolidCube(4);
	endTranslate();

	startTranslate(0,4,-5);
	glutSolidCube(4);
	endTranslate();


}

void setTextureMatrix(void)
{

	static double modelView[16];
	static double projection[16];

	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5
	// y = y* 0.5 + 0.5
	// z = z* 0.5 + 0.5
	// Moving from unit cube [-1,1] to [0,1]
	const GLdouble bias[16] = {
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0};

	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);


	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);

	glLoadIdentity();
	glLoadMatrixd(bias);

	// concatating all matrice into one.
	glMultMatrixd (projection);
	glMultMatrixd (modelView);

	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void displayBumpMapping ()
{
	const shapes s = SQUARE;
	glUseProgram(programObject);
	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// texture for texture mapping
	glActiveTexture(GL_TEXTURE0);
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, simple_texture);
	glUniform1i(glGetUniformLocation(programObject, "tex"), 0);

	// texture for NORMAL mapping
	glActiveTexture(GL_TEXTURE1);
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, normal_texture);
	glUniform1i(glGetUniformLocation(programObject, "normal_tex"), 1);



	/// display function for bump mapping

	glm::mat4 projection = glm::perspective(60.0f,1.0f,.1f,100.0f);
	glm::mat4 view = glm::lookAt
			(		glm::vec3(15.0, 15.0, 15.0), 		//position of the camera in world space
					glm::vec3(0.0, 0.0, 0.0),    // camera target
					glm::vec3(0.0, 1.0, 0.0));  	// up vector
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

	GLuint c0 = glGetAttribLocation(programObject, "position");
	GLuint c1 = glGetAttribLocation(programObject, "color");
	GLuint c2 = glGetAttribLocation(programObject, "normal");
	GLuint c3 = glGetAttribLocation(programObject, "texCoord");
	GLuint c4 = glGetAttribLocation(programObject, "tangentCoord");
	glEnableVertexAttribArray(c0);
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glEnableVertexAttribArray(c4);

	/// light computation
	GLuint LightID = glGetUniformLocation(programObject, "LightPosition_worldspace");
	/// define the light
	glm::vec3 lightPos = glm::vec3(0.0,20.0,0.0);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	glm::mat4 modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, -5.0, 0.0));
	modelTemp = glm::scale(modelTemp,glm::vec3(20.0,20.0,20.0));
	glm::mat4 modelview = view * modelTemp;
	glm::mat4 modelview_projection = projection * modelview;
	GLuint m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	GLuint m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	GLuint m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[CUBE]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[CUBE]);

	glVertexAttribPointer(c0,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0);
	glVertexAttribPointer(c1,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16);
	glVertexAttribPointer(c2,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	glVertexAttribPointer(c4,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+56);
	glDrawElements(GL_TRIANGLES, tindicesLists[CUBE].size(), GL_UNSIGNED_INT, (char*) NULL+0);



	glutSwapBuffers();

	glUseProgram(0);
}
void display(void)
{
	//shadow
	//First step: Render from the light POV to a FBO, story depth values only
	glBindFramebuffer(GL_FRAMEBUFFER,fboId);	//Rendering offscreen

	//Using the fixed pipeline to render to the depthbuffer
	glUseProgram(programObject);

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);

	// Clear previous frame values
	glClear( GL_DEPTH_BUFFER_BIT);

	//Disable color rendering, we only want to write to the Z-Buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	//setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);


	////////////////////////////
	glUseProgram(programObject);
	// DONOT use_texture
	glUniform1i(glGetUniformLocation(programObject, "use_texture"), 0);

	GLuint LightID = glGetUniformLocation(programObject, "LightPosition_worldspace");
	//define the light
	glm::vec3 lightPos = glm::vec3(0.0,20.0,0.0);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	glm::mat4 projection = glm::perspective(60.0f,1.0f,10.0f,400.0f);
	glm::mat4 view = glm::lookAt(
			glm::vec3(lightPos), //position of the camera in world space
			glm::vec3(0.0, 0.0, -5.0),     // camera target
			glm::vec3(0.0, 1.0, 0.0));   // up vector

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

	///// darw object 1
	glm::mat4 modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 1.0, 0.0));
	modelTemp = glm::scale(modelTemp,glm::vec3(25.0,25.0,25.0));
	glm::mat4 modelview = view * modelTemp;
	glm::mat4 modelview_projection = projection * modelview;
	GLuint m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	GLuint m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	GLuint m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	GLuint c0 = glGetAttribLocation(programObject, "position");
	GLuint c1 = glGetAttribLocation(programObject, "color");
	GLuint c2 = glGetAttribLocation(programObject, "normal");
	GLuint c3 = glGetAttribLocation(programObject, "texCoord");
	glEnableVertexAttribArray(c0);
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);

	glCullFace(GL_FRONT);
	//display_object(CUBE,c0,c1,c2,c3);
	display_object(BUNNY,c0,c1,c2,c3);
	//// object 2 which is also a  square
	//glActiveTextureARB(GL_TEXTURE7);
/*
	modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, -5.0, 0.0));
	modelTemp = glm::scale(modelTemp,glm::vec3(15.0,15.0,15.0));
	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	display_object(SQUARE,c0,c1,c2,c3);
*/
	///


	// Now rendering from the camera POV, using the FBO to generate shadows
	////////////////////////////
	glUseProgram(programObject);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);

	//glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects

	glDisable(GL_CULL_FACE);
	//glClearColor(0,0,0,1);
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//Enabling color write (previously disabled for light POV z-buffer rendering)
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//Using the shadow shader
	glUseProgram(programObject);
	glUniform1i(glGetUniformLocation(programObject, "use_texture"), 1);
	glActiveTexture(GL_TEXTURE0);
	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,depthTextureID);
	glUniform1i(glGetUniformLocationARB(programObject,"ShadowMap"),0);

	view = glm::lookAt(
			glm::vec3(12,22,12), //position of the camera in world space
			glm::vec3(2.0, 0.0, -10.0),     // camera target
			glm::vec3(0.0, 1.0, 0.0));   // up vector
	projection = glm::perspective(60.0f,1.0f,10.0f,40.0f);
	model = glm::mat4(1.0f);

	model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

	glActiveTexture(GL_TEXTURE1);
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, simple_texture);
	glUniform1i(glGetUniformLocation(programObject, "tex"), 1);

	///// darw object 1
	modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 1.0, 0.0));
	modelTemp = glm::scale(modelTemp,glm::vec3(25.0,25.0,25.0));
	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);

	m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);

	glm::mat4 biasMat = glm::mat4(0.0f);
	biasMat[0][0]=0.5;
	biasMat[1][1]=0.5;
	biasMat[2][2]=0.5;
	biasMat[3][3]=1.0;
	biasMat[3][2]=0.5;
	biasMat[2][1]=0.5;
	biasMat[3][0]=0.5;
	/// debug
	cout <<"biaMat "<<biasMat[2][1]<<endl;
	GLuint bias  = glGetUniformLocation(programObject, "bias"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(bias, 1, GL_FALSE, &biasMat[0][0]);

	c0 = glGetAttribLocation(programObject, "position");
	c1 = glGetAttribLocation(programObject, "color");
	c2 = glGetAttribLocation(programObject, "normal");
	c3 = glGetAttribLocation(programObject, "texCoord");
	glEnableVertexAttribArray(c0);
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);
	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	//glCullFace(GL_FRONT);
	glUniform1i(glGetUniformLocation(programObject, "use_texture"), 0);
	display_object(BUNNY,c0,c1,c2,c3);
	//display_object(CUBE,c0,c1,c2,c3);
	glUniform1i(glGetUniformLocation(programObject, "use_texture"), 1);

	/*
	modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(-4.0, 0.0, 0.0));
	//modelTemp = glm::scale(modelTemp,glm::vec3(5.0,5.0,5.0));

	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	display_object(CUBE,c0,c1,c2,c3);
	/////
*/
	///// kept static for shadow mapping

	glm::mat4 modelTemp2;
	modelTemp2 = glm::translate(modelTemp2,glm::vec3(5.0, -5.0, -5.0));
	modelTemp2 = glm::scale(modelTemp2,glm::vec3(35.0,35.0,35.0));

	modelview = view * modelTemp2;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	display_object(SQUARE,c0,c1,c2,c3);

	/*
	modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(5.0, -5.0, -5.0));
	modelTemp = glm::scale(modelTemp,glm::vec3(15.0,15.0,15.0));

	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	display_object(SQUARE,c0,c1,c2,c3);
	*/
	///////////////////////////////////////


	// DEBUG only. this piece of code draw the depth buffer onscreen
/*
	glUseProgramObjectARB(0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor4f(1,1,1,1);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,depthTextureID);
	glEnable(GL_TEXTURE_2D);
	glTranslated(0,0,-1);
	glBegin(GL_QUADS);
	glTexCoord2d(0,0);glVertex3f(0,0,0);
	glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
	glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
	glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);


	glEnd();
	glDisable(GL_TEXTURE_2D);
*/
	glutSwapBuffers();
	glUseProgram(0);

}
void display_everything ()
{
	//shadow
		//First step: Render from the light POV to a FBO, story depth values only
		glBindFramebuffer(GL_FRAMEBUFFER,fboId);	//Rendering offscreen

		//Using the fixed pipeline to render to the depthbuffer
		glUseProgram(programObject);

		// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
		glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_RATIO,RENDER_HEIGHT* SHADOW_MAP_RATIO);

		// Clear previous frame values
		glClear( GL_DEPTH_BUFFER_BIT);

		//Disable color rendering, we only want to write to the Z-Buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		////////////////////////////
		glUseProgram(programObject);
		// DONOT use_texture
		glUniform1i(glGetUniformLocation(programObject, "use_texture"), 0);

		GLuint LightID = glGetUniformLocation(programObject, "LightPosition_worldspace");
		//define the light
		glm::vec3 lightPos = glm::vec3(0.0,20.0,0.0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glm::mat4 projection = glm::perspective(60.0f,1.0f,10.0f,400.0f);
		glm::mat4 view = glm::lookAt(
				glm::vec3(lightPos), //position of the camera in world space
				glm::vec3(0.0, 0.0, -5.0),     // camera target
				glm::vec3(0.0, 1.0, 0.0));   // up vector

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

		///// darw object 1
		glm::mat4 modelTemp = model;
		modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 1.0, 0.0));
		//modelTemp = glm::scale(modelTemp,glm::vec3(5.0,5.0,5.0));
		glm::mat4 modelview = view * modelTemp;
		glm::mat4 modelview_projection = projection * modelview;
		GLuint m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
		GLuint m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
		GLuint m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
		GLuint c0 = glGetAttribLocation(programObject, "position");
		GLuint c1 = glGetAttribLocation(programObject, "color");
		GLuint c2 = glGetAttribLocation(programObject, "normal");
		GLuint c3 = glGetAttribLocation(programObject, "texCoord");
		glEnableVertexAttribArray(c0);
		glEnableVertexAttribArray(c1);
		glEnableVertexAttribArray(c2);
		glEnableVertexAttribArray(c3);
		glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);

		glCullFace(GL_FRONT);
		display_object(CUBE,c0,c1,c2,c3);
		//second object
		modelTemp = model;
			modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 1.0, 0.0));
			modelTemp = glm::scale(modelTemp,glm::vec3(15.0,15.0,15.0));
			modelview = view * modelTemp;
			modelview_projection = projection * modelview;
			m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
			glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
			m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
			glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);

			m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
			glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
			//display_object(CUBE,c0,c1,c2,c3);

		// Now rendering from the camera POV, using the FBO to generate shadows
		////////////////////////////
		glUseProgram(programObject);
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);

		//glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects

		glDisable(GL_CULL_FACE);
		//glClearColor(0,0,0,1);
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//Enabling color write (previously disabled for light POV z-buffer rendering)
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		// Clear previous frame values
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
///
		const shapes s = SQUARE;
		glUseProgram(programObject);
		glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// texture for NORMAL mapping
		glActiveTexture(GL_TEXTURE2);
		glEnable (GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, normal_texture);
		glUniform1i(glGetUniformLocation(programObject, "normal_tex"), 2);
		////////////////

		//Using the shadow shader
		glUseProgram(programObject);
		glUniform1i(glGetUniformLocation(programObject, "use_texture"), 1);
		glActiveTexture(GL_TEXTURE0);
		glEnable (GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,depthTextureID);
		glUniform1i(glGetUniformLocationARB(programObject,"ShadowMap"),0);

		view = glm::lookAt(
				glm::vec3(22,22,22), //position of the camera in world space
				glm::vec3(2.0, 0.0, -5.0),     // camera target
				glm::vec3(0.0, 1.0, 0.0));   // up vector
		projection = glm::perspective(60.0f,1.0f,10.0f,400.0f);
		model = glm::mat4(1.0f);

		model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

		glActiveTexture(GL_TEXTURE1);
		glEnable (GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, simple_texture);
		glUniform1i(glGetUniformLocation(programObject, "tex"), 1);

		///// darw object 1
		modelTemp = model;
		modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 1.0, 0.0));
		//modelTemp = glm::scale(modelTemp,glm::vec3(5.0,5.0,5.0));
		modelview = view * modelTemp;
		modelview_projection = projection * modelview;
		m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
		m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);

		m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);

		glm::mat4 biasMat = glm::mat4(0.0f);
		biasMat[0][0]=0.5;
		biasMat[1][1]=0.5;
		biasMat[2][2]=0.5;
		biasMat[3][3]=1.0;
		biasMat[3][2]=0.5;
		biasMat[2][1]=0.5;
		biasMat[3][0]=0.5;
		/// debug
		cout <<"biaMat "<<biasMat[2][1]<<endl;
		GLuint bias  = glGetUniformLocation(programObject, "bias"); // get the location of the uniform variable in the shader
			glUniformMatrix4fv(bias, 1, GL_FALSE, &biasMat[0][0]);

		c0 = glGetAttribLocation(programObject, "position");
		c1 = glGetAttribLocation(programObject, "color");
		c2 = glGetAttribLocation(programObject, "normal");
		c3 = glGetAttribLocation(programObject, "texCoord");
		glEnableVertexAttribArray(c0);
		glEnableVertexAttribArray(c1);
		glEnableVertexAttribArray(c2);
		glEnableVertexAttribArray(c3);
		glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
		//glCullFace(GL_FRONT);
		display_object(CUBE,c0,c1,c2,c3);
		//display_object(CUBE,c0,c1,c2,c3);
		glUniform1i(glGetUniformLocation(programObject, "use_texture"), 4);
		glActiveTexture(GL_TEXTURE4);
			glEnable (GL_TEXTURE_2D);
			glBindTexture (GL_TEXTURE_2D, grass_texture);
			glUniform1i(glGetUniformLocation(programObject, "grass"), 4);

			// texture for NORMAL mapping
					glActiveTexture(GL_TEXTURE5);
					glEnable (GL_TEXTURE_2D);
					glBindTexture (GL_TEXTURE_2D, normal_texture2);
					glUniform1i(glGetUniformLocation(programObject, "normal_tex2"), 5);
		///// kept static for shadow mapping

		glm::mat4 modelTemp2;
		modelTemp2 = glm::translate(modelTemp2,glm::vec3(-15.0, -15.0, -15.0));
		modelTemp2 = glm::scale(modelTemp2,glm::vec3(35.0,1.0,35.0));

		modelview = view * modelTemp2;
		modelview_projection = projection * modelview;
		m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
		m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
		m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);

		glUniform1i(glGetUniformLocation(programObject, "use_texture"), 4);
		display_object(CUBE,c0,c1,c2,c3);

		modelTemp = model;
		modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 1.0, 0.0));
		modelTemp = glm::scale(modelTemp,glm::vec3(15.0,15.0,15.0));
		modelview = view * modelTemp;
		modelview_projection = projection * modelview;
		m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
		m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);

		m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
		glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
		//display_object(BUNNY,c0,c1,c2,c3);
		glutSwapBuffers();
		glUseProgram(0);
}
void display2()
{
	glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(programObject);
	glm::mat4 projection = glm::perspective(60.0f,1.0f,.1f,100.0f);
	glm::mat4 view = glm::lookAt
			(glm::vec3(12.0, 12.0, 12.0), //position of the camera in world space
					glm::vec3(0.0, 0.0, 0.0),     // camera target
					glm::vec3(0.0, 1.0, 0.0));   // up vector
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, x_angle, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, y_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));

	GLuint c0 = glGetAttribLocation(programObject, "position");
	GLuint c1 = glGetAttribLocation(programObject, "color");
	GLuint c2 = glGetAttribLocation(programObject, "normal");
	GLuint c3 = glGetAttribLocation(programObject, "texCoord");
	glEnableVertexAttribArray(c0);
	glEnableVertexAttribArray(c1);
	glEnableVertexAttribArray(c2);

	/// setting up light
	GLuint LightID = glGetUniformLocation(programObject, "LightPosition_worldspace");
	//define the light
	glm::vec3 lightPos = glm::vec3(10.0,14.0,10.0);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
	// texture for cube mapping
	glActiveTexture(GL_TEXTURE0);
	glEnable (GL_TEXTURE_CUBE_MAP);
	glBindTexture (GL_TEXTURE_CUBE_MAP, cubemap_texture);
	glUniform1i(glGetUniformLocation(programObject, "env"), 0);
	glUniform1i(glGetUniformLocation(programObject, "use_texture"), 1);



	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	// construct the modelview and modelview projection matricesrrt
	///////////////////////////
	float angle_in_degrees = 90.0;
	//// negative y axis
	glm::mat4 modelTemp = model;

	//modelTemp = glm::rotate(modelTemp, angle_in_degrees, glm::vec3(0, 0, 1));
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, -5.0, 0.0));
	modelTemp = glm::scale(modelTemp,glm::vec3(10.0,10.0,10.0));
	glm::mat4 modelview = view * modelTemp;
	glm::mat4 modelview_projection = projection * modelview;
	GLuint m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	GLuint m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	GLuint m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);

	/////
	modelTemp = model;
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 0.0, 4.0));
	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);


	glEnableVertexAttribArray(c3);
	glVertexAttribPointer(c3,2,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+48);
	display_object(SPHERE,c0,c1,c2,c3);
	glUniform1i(glGetUniformLocation(programObject, "use_texture"), 0);

	// texture for texture mapping
	glActiveTexture(GL_TEXTURE1);
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, simple_texture);
	glUniform1i(glGetUniformLocation(programObject, "tex"), 1);


	// second sphere
	modelTemp = model;
	angle_in_degrees=90;
	modelTemp = glm::translate(modelTemp,glm::vec3(0.0, 0.0, -4.0));
	modelTemp = glm::rotate(modelTemp, angle_in_degrees, glm::vec3(1, 0, 0));
	//modelTemp = glm::scale(modelTemp,glm::vec3(10.0,10.0,10.0));
	modelview = view * modelTemp;
	modelview_projection = projection * modelview;
	m1 = glGetUniformLocation(programObject, "mvp"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m1, 1, GL_FALSE, &modelview_projection[0][0]);
	m2 = glGetUniformLocation(programObject, "model"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m2, 1, GL_FALSE, &modelTemp[0][0]);
	m3 = glGetUniformLocation(programObject, "view"); // get the location of the uniform variable in the shader
	glUniformMatrix4fv(m3, 1, GL_FALSE, &view[0][0]);
	display_object(SPHERE,c0,c1,c2,c3);
	glutSwapBuffers();

	glUseProgram(0);
}

//////////////////////////////////////////////////////////////////////////////////
//
// create VBO objects and send the triangle vertices/colors to the graphics card
//

void initVBOi(const int i)
{
	cout <<" i "<<i<<endl;
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[i]);   // bind the first handle

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertxLists[i].size(), &vertxLists[i].front(), GL_STATIC_DRAW); // allocate space and copy the position data over
	glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[i]);
	// debug
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tindicesLists[i].size(), &tindicesLists[i].front(), GL_STATIC_DRAW);  // load the index data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void InitVBO()
{

	glGenBuffers(num_of_objects,vboHandle);
	glGenBuffers(num_of_objects,indexVBO);
	for(int i=0; i<num_of_objects; i++)
		initVBOi(i);
}
GLubyte * read_Image(const string fname)
{
	FILE* in = fopen(fname.c_str(), "rb");

	// int height, width, ccv;
	char header[100];

	fscanf(in, "%s %d %d %d", header, &width, &height, &ccv);
	//cout << "header"<< width <<" "<<height <<endl;

	printf(" read image info :: %s %d %d %d\n", header, width, height, ccv);
	int r, g, b;

	for (int i=height-1; i>=0; i--)
		for (int j=0; j<width; j++)
		{
			fscanf(in, "%d %d %d", &r, &g, &b);
			readImage[i][j][0] = (GLubyte)r;
			readImage[i][j][1] = (GLubyte)g;
			readImage[i][j][2] = (GLubyte )b;
			readImage[i][j][3] = 255;
		}

	for (int i=0; i<textureL; i++)
		for ( int j=0; j<textureL; j++)
		{
			if (i<height && j <width)
			{
				texImage[i][j][0] = readImage[i][j][0];
				texImage[i][j][1] = readImage[i][j][1];
				texImage[i][j][2] = readImage[i][j][2];
				texImage[i][j][3] = 255;
			}
			else
			{
				texImage[i][j][0] = 255;
				texImage[i][j][1] = 255;
				texImage[i][j][2] = 255;
				texImage[i][j][3] = 255;
			}
		}

	fclose(in);

	return &(texImage[0][0][0]);
}

void read_Texture()
{

	for (int i=0; i<textureL; i++)
		for ( int j=0; j<textureL; j++)
		{

			texImage[i][j][0] = 255;
			texImage[i][j][1] = 0;
			texImage[i][j][2] = 0;
			texImage[i][j][3] = 255;
		}
}

static GLuint
load_texture_cube_map (const char *posx, const char *negx,
		const char *posy, const char *negy,
		const char *posz, const char *negz)
{
	GLuint texId;

	/* Generate a new cube map texture and bind to it */
	glGenTextures (1, &texId);
	glBindTexture (GL_TEXTURE_CUBE_MAP, texId);

	/* Setup some parameters for texture filters and mipmapping */
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	/* Load each face of the cube map */
	if (!load_tga_texture (posx, GL_TEXTURE_CUBE_MAP_POSITIVE_X) ||
			!load_tga_texture (negx, GL_TEXTURE_CUBE_MAP_NEGATIVE_X) ||
			!load_tga_texture (posy, GL_TEXTURE_CUBE_MAP_POSITIVE_Y) ||
			!load_tga_texture (negy, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y) ||
			!load_tga_texture (posz, GL_TEXTURE_CUBE_MAP_POSITIVE_Z) ||
			!load_tga_texture (negz, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))
	{
		glDeleteTextures (1, &texId);
		return 0;
	}

	return texId;
}


////
static GLuint
load_simple_texture (const string fname)
{
	GLuint texId;

	/* Generate a new cube map texture and bind to it */
	glGenTextures (1, &texId);
	glBindTexture (GL_TEXTURE_2D, texId);

	/* Setup some parameters for texture filters and mipmapping */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GLubyte *data = read_Image(fname.c_str());
	glTexImage2D
	(GL_TEXTURE_2D, 0, GL_RGBA, textureL, textureL,0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texId;
}

static GLuint
load_simple_textureTGA (const string fname)
{
	GLuint texId;

	/* Generate a new cube map texture and bind to it */
	glGenTextures (1, &texId);
	glBindTexture (GL_TEXTURE_2D, texId);

	/* Setup some parameters for texture filters and mipmapping */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	bool succeess = load_tga_texture (fname.c_str(),GL_TEXTURE_2D);
	return texId;
}

void InitTexture()
{
	cubemap_texture= load_texture_cube_map (
			"data/nvlobby_posx.tga", "data/nvlobby_negx.tga",
			"data/nvlobby_posy.tga", "data/nvlobby_negy.tga",
			"data/nvlobby_posz.tga", "data/nvlobby_negz.tga");

	if (!cubemap_texture)
	{
		fprintf (stderr, "Error: failed to load cube map texture!\n");
		exit (1);
	}

	/// obsolete
	simple_texture = load_simple_texture("data/piller.ppm");

	//simple_texture = load_simple_textureTGA("data/GraniteWall-ColorMap.tga");
	normal_texture = load_simple_textureTGA("data/GraniteWall-NormalMap.tga");
	grass_texture = load_simple_textureTGA("data/grass.tga");
	normal_texture2 = load_simple_textureTGA("data/normal_2.tga");

	cout <<"textures loaded"<<endl;

}
//////////////////////////////////////////////////////////////////////////////////
//
//    GLUT stuff
//

void mymouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		press_x = x;
		press_y = y;
		if (button == GLUT_LEFT_BUTTON)
			xform_mode = XFORM_ROTATE;
		else if (button == GLUT_RIGHT_BUTTON)
			xform_mode = XFORM_SCALE;
	}
	else if (state == GLUT_UP)
	{
		xform_mode = XFORM_NONE;
	}
}
void mymotion(int x, int y)
{
	if (xform_mode==XFORM_ROTATE)
	{
		x_angle += (x - press_x)/5.0;
		if (x_angle > 180) x_angle -= 360;
		else if (x_angle <-180) x_angle += 360;
		press_x = x;

		y_angle += (y - press_y)/5.0;
		if (y_angle > 180) y_angle -= 360;
		else if (y_angle <-180) y_angle += 360;
		press_y = y;
	}
	else if (xform_mode == XFORM_SCALE)
	{
		float old_size = scale_size;
		scale_size *= (1+ (y - press_y)/60.0);
		if (scale_size <0) scale_size = old_size;
		press_y = y;
	}
	glutPostRedisplay();
}


void mykey(unsigned char key, int x, int y)
{
	if (key == 'q') exit(1);
	if (key == 't')
	{
		cout <<"t";
		modelCurr=glm::translate(modelCurr,glm::vec3(1.0, 0.0, 0.0));
	}
	if (key == 'T')
	{
		cout <<"T";
		modelCurr=glm::translate(modelCurr,glm::vec3(-1.0, 0.0, 0.0));
	}
	if (key == 'r')
	{
		cout <<"rotate "<<turn;
		turn=turn+5;
		modelCurr=glm::rotate(modelCurr, turn, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (key == 'R')
	{
		cout <<"rotate "<<turn;
		turn=turn-5;
		modelCurr=glm::rotate(modelCurr, turn, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (key == 'l')
	{
		LAangle+=15;
		cout <<"langle"<<LAangle<<endl;
	}


	glutPostRedisplay();
}

void init2DTexture(GLint texName, GLint texWidth,
		GLint texHeight, GLubyte *texPtr)
{
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0,
			GL_RGB, GL_UNSIGNED_BYTE, texPtr);
}


///////////////////////////////////////////////////////////////////////////////
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(RENDER_WIDTH,RENDER_HEIGHT);

	glutCreateWindow("shader cube");
	glutDisplayFunc(display_everything); // shadow
	//glutDisplayFunc(display);
	//glutDisplayFunc(displayBumpMapping );
	glutMouseFunc(mymouse);
	glutMotionFunc(mymotion);
	glutKeyboardFunc(mykey);

	// initialize GLEW

	GLenum err = glewInit();

	if ( err != GLEW_OK)
		printf(" Error initializing GLEW! \n");
	else
		printf("Initializing GLEW succeeded!\n");

	string fileLoc = "/home/arindam/Course/lab5/src/cubeXform";
	//programObject = SetupGLSL("/home/arindam/Course/lab3/cse5542Lab3/cubeXform");  //create shaders - assume the shaders are cubeXform.vert and cubeXform.frag
	programObject = SetupGLSL("/home/arindam/workspace/cse5542lab5/cubeXform");
	InitGeometry();

	InitTexture();

	generateShadowFBO();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);

	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);



	cout <<"init geometry done "<<endl;
	InitVBO();
	cout <<" init vbo "<<endl;
	cout <<" init texture "<<endl;
	glutMainLoop();
}

