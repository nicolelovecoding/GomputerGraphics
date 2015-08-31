// Homework #2 for Computer Graphic, UFL
// 
// Authoer: Ruijin Wu <ruijin@cise.ufl.edu>


#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#ifdef __APPLE__
#include <GLEW/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include "utils.h"
#include "debuglib.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"


// CONTSTANTS
const float PI = 3.1415926;
const int window_width = 500, window_height = 500;
const char * window_title = "HW2";

struct Vertex  
{
    glm::vec4 pos;
    glm::vec4 color;
	glm::vec4 normal;
};
typedef vector<Vertex> V_Vertex;
double angle = 45;
double angle2 = 45;
glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
glm::mat4 ProjectionMatrix = glm::ortho(-5.0f,5.0f,-5.0f,5.0f,0.1f,100.0f);
glm::mat4 ModleMatrix = glm::mat4(1.0f);
glm::mat3 Modle_s = glm::mat3(1.0f);//just in order to transmit to the shader
glm::mat4 Inverse_view = glm::inverse(ViewMatrix);
glm::mat4 MVP = ProjectionMatrix * ViewMatrix;

glm::mat4 ModleMatrix_obj = glm::mat4(1.0f);
glm::mat4 MVP_object = MVP * ModleMatrix_obj;

glm::mat4 ModleMatrix_top = glm::mat4(1.0f);
glm::mat4 MVP_top = MVP * ModleMatrix_top;

glm::mat4 ModleMatrix_FArm = glm::mat4(1.0f);
glm::mat4 MVP_FArm = MVP * ModleMatrix_FArm;

glm::mat4 ModleMatrix_Join = glm::mat4(1.0f);
glm::mat4 MVP_Join = MVP * ModleMatrix_Join;

glm::mat4 ModleMatrix_SArm = glm::mat4(1.0f);
glm::mat4 MVP_SArm = MVP * ModleMatrix_SArm;

glm::mat4 ModleMatrix_Pen = glm::mat4(1.0f);
glm::mat4 MVP_Pen = MVP * ModleMatrix_Pen;

glm::mat4 ModleMatrix_Botton = glm::mat4(1.0f);
glm::mat4 MVP_Botton = MVP * ModleMatrix_Botton;

glm::mat4 scale_top = glm::mat4(1.0f);
glm::mat4 rotation_top = glm::mat4(1.0f);
glm::mat4 translation_top = glm::mat4(1.0f);

glm::mat4 scale_FArm = glm::mat4(1.0f);
glm::mat4 rotation_FArm = glm::mat4(1.0f);
glm::mat4 translation_FArm = glm::mat4(1.0f);

glm::mat4 scale_Join = glm::mat4(1.0f);
glm::mat4 rotation_Join = glm::mat4(1.0f);
glm::mat4 translation_Join_X = glm::mat4(1.0f);
glm::mat4 translation_Join_Y = glm::mat4(1.0f);

glm::mat4 scale_SArm = glm::mat4(1.0f);
glm::mat4 rotation_SArm = glm::mat4(1.0f);
glm::mat4 translation_SArm_X_1 = glm::mat4(1.0f);
glm::mat4 translation_SArm_X_2 = glm::mat4(1.0f);
glm::mat4 translation_SArm_Y = glm::mat4(1.0f);

glm::mat4 scale_Pen = glm::mat4(1.0f);
glm::mat4 rotation_Pen = glm::mat4(1.0f);
glm::mat4 translation_Pen_X_1 = glm::mat4(1.0f);
glm::mat4 translation_Pen_X_2 = glm::mat4(1.0f);
glm::mat4 translation_Pen_X_3 = glm::mat4(1.0f);
glm::mat4 translation_Pen_Y = glm::mat4(1.0f);

glm::mat4 scale_Botton = glm::mat4(1.0f);
glm::mat4 rotation_Botton = glm::mat4(1.0f);
glm::mat4 translation_Botton_X_1 = glm::mat4(1.0f);
glm::mat4 translation_Botton_X_2 = glm::mat4(1.0f);
glm::mat4 translation_Botton_X_3 = glm::mat4(1.0f);
glm::mat4 translation_Botton_Y = glm::mat4(1.0f);

// runtime variable
char buf[1024];
V_Vertex grid_vertices;
bool c_pressed = false;
bool p_pressed = false;
bool botton1_pressed = false;
bool botton2_pressed = false;
bool botton3_pressed = false;
bool botton4_pressed = false;
bool botton5_pressed = false;

//Vertex Cube[36];
V_Vertex base_vertices;
V_Vertex top_vertices;
V_Vertex FArm_vertices;
V_Vertex Join_vertices;
V_Vertex SArm_vertices;
V_Vertex Pen_vertices;
V_Vertex Botton_vertices;
int l_botton = 0; //control the botton of light

/*vector<glm::vec4> base_normal;
vector<glm::vec4> top_normal;
vector<glm::vec4> FArm_normal;
vector<glm::vec4> Join_normal;
vector<glm::vec4> SArm_normal;
vector<glm::vec4> Pen_normal;
vector<glm::vec4> Botton_normal;*/

GLuint vertex_shader, fragment_shader, program;

// function Prototype 
void graphics_init();
void initialize_base();
void initialize_top();
void initialize_FArm();
void initialize_Join();
void initialize_SArm();
void initialize_Pen();
void initialize_Botton();
void draw_parts(V_Vertex, GLenum,glm::mat4,glm::mat4,glm::mat3);
void draw();

Vertex test[2];
V_Vertex V_test;

void initialize_points(){
	grid_vertices.clear();
    // create the grid
    for(int i = 0; i < 11 ; i++)
    {
        Vertex start,end;
        start.color = glm::vec4(1,1,1,1);
        end.color = glm::vec4(1,1,1,1);

        start.pos = glm::vec4(i - 5.0, 0, -5.0, 1.0);//start.pos = ProjectionMatrix * ViewMatrix * start.pos; 
        end.pos = glm::vec4(i - 5.0, 0, 5.0, 1.0);//end.pos = ProjectionMatrix * ViewMatrix * end.pos;
        grid_vertices.push_back(start);
        grid_vertices.push_back(end);
            
        start.pos = glm::vec4(-5.0, 0, i  - 5.0, 1.0);//start.pos = ProjectionMatrix * ViewMatrix * start.pos;
        end.pos = glm::vec4(5.0f, 0, i - 5.0, 1.0);//end.pos = ProjectionMatrix * ViewMatrix * end.pos;
        grid_vertices.push_back(start);
        grid_vertices.push_back(end);
		//printf("%s\n",start.normal);
    }
    // create the frame at origin

    Vertex vo, vx,vy,vz;
    vo.pos = glm::vec4(0,0.01,0,1);
    
    vx.pos = glm::vec4(3,0.01,0,1);//vx.pos = ProjectionMatrix * ViewMatrix * vx.pos;
    vx.color = glm::vec4(1,0,0,1); // red
    
    vy.pos = glm::vec4(0,3.01,0,1);//vy.pos = ProjectionMatrix * ViewMatrix * vy.pos;
    vy.color = glm::vec4(0,1,0,1); // green
    
    vz.pos = glm::vec4(0,0.01,3,1);//vz.pos = ProjectionMatrix * ViewMatrix * vz.pos;
    vz.color = glm::vec4(0,0,1,1); // blue

    vo.color = vx.color;
    grid_vertices.push_back(vo);
    grid_vertices.push_back(vx);

    vo.color = vy.color;
    grid_vertices.push_back(vo);
    grid_vertices.push_back(vy);
    
    vo.color = vz.color;
    grid_vertices.push_back(vo);
    grid_vertices.push_back(vz); 

	for(int i = 0; i < grid_vertices.size(); i++)
		grid_vertices.at(i).normal = glm::normalize(glm::vec4(0.0f,1.0f,0.0f,0.0f));

	initialize_base();
	initialize_top();
	initialize_FArm();
	initialize_Join();
	initialize_SArm();
	initialize_Pen();
	initialize_Botton();
}

void initialize_base(){

	Vertex temp;
	GLfloat angle_circle = 5;

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(0.0f,0.15f,0.0f,1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
    	temp.normal = glm::vec4(0.0f,1.0f,0.0f,0.0f);
		base_vertices.push_back(temp);	

		temp.pos = glm::vec4(1.5*cos(angle_circle*i*PI/180),0.15f,1.5*sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*(i+1)*PI/180),0.15f,1.5*sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

	}
/*  test[0] = base_vertices.at(1);
	test[0].color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
	test[0].normal = glm::vec4(1.0f,0.0f,0.0f,0.0f);
	
	test[1].pos = base_vertices.at(1).normal + base_vertices.at(1).pos;
	test[1].color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
	test[1].normal = glm::vec4(1.0f,0.0f,0.0f,0.0f);*/

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(0.0f,-0.15f,0.0f,1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::vec4(0.0f,-1.0f,0.0f,0.0f);
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*i*PI/180),-0.15f,1.5*sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,-0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,-1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*(i+1)*PI/180),-0.15f,1.5*sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,-0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,-1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(1.5*cos(angle_circle*i*PI/180),0.15f,1.5*sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*i*PI/180),-0.15f,1.5*sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,-0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,-1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*(i+1)*PI/180),-0.15f,1.5*sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,-0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,-1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*i*PI/180),0.15f,1.5*sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*(i+1)*PI/180),0.15f,1.5*sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);

		temp.pos = glm::vec4(1.5*cos(angle_circle*(i+1)*PI/180),-0.15f,1.5*sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.1f,0.2f,0.1f,1.0f);
		temp.normal = glm::normalize(glm::normalize(temp.pos - glm::vec4(0.0f,-0.15f,0.0f,1.0f)) 
			+ glm::vec4(0.0f,-1.0f,0.0f,0.0f));
		base_vertices.push_back(temp);
	}
/*		V_test.clear();
	V_test.push_back(test[0]);
	V_test.push_back(test[1]);*/
}

void initialize_top(){

	for (int i = 0; i < base_vertices.size(); i++){

		top_vertices.push_back(base_vertices[i]);  
		top_vertices[i].color  = glm::vec4(0.0f,0.0f,1.0f,1.0f);
	}

	scale_top  = glm::scale(0.6f,4.0f,0.6f);
	rotation_top =  glm::rotate(90.0f,glm::vec3(1,0,0));
	translation_top = glm::translate(0.0f,0.8f,0.0f);
	ModleMatrix_top =  translation_top * rotation_top * scale_top;
	MVP_top = MVP * ModleMatrix_top;
}

void initialize_FArm(){

	Vertex temp[12];
    //0
	temp[0].pos = glm::vec4(0.0f,0.2f,-0.2f,1.0f);  
	temp[0].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[0].normal = glm::normalize(glm::vec4(-1.0f,1.0f,-1.0f,0.0f));

	//1
	temp[1].pos = glm::vec4(0.0f,-0.2f,-0.2f,1.0f);  
	temp[1].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[1].normal = glm::normalize(glm::vec4(-1.0f,-1.0f,-1.0f,0.0f));

	//2
	temp[2].pos = glm::vec4(0.0f,-0.2f,0.2f,1.0f);  
	temp[2].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[2].normal = glm::normalize(glm::vec4(-1.0f,-1.0f,1.0f,0.0f));

	//3
	temp[3].pos = glm::vec4(0.0f,0.2f,0.2f,1.0f);  
	temp[3].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[3].normal = glm::normalize(glm::vec4(-1.0f,1.0f,1.0f,0.0f));

	//4
	temp[4].pos = glm::vec4(2.5f,-0.2f,0.2f,1.0f);  
	temp[4].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[4].normal = glm::normalize(glm::vec4(1.0f,-1.0f,1.0f,0.0f));
	
	//5
	temp[5].pos = glm::vec4(2.5f,0.2f,0.2f,1.0f);  
	temp[5].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[5].normal = glm::normalize(glm::vec4(1.0f,1.0f,1.0f,0.0f));

	//6
	temp[6].pos = glm::vec4(2.5f,-0.2f,-0.2f,1.0f);  
	temp[6].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[6].normal = glm::normalize(glm::vec4(1.0f,-1.0f,-1.0f,0.0f));

	//7
	temp[7].pos = glm::vec4(2.5f,0.2f,-0.2f,1.0f);  
	temp[7].color = glm::vec4(1.0f,1.0f,0.0f,1.0f);
	temp[7].normal = glm::normalize(glm::vec4(1.0f,1.0f,-1.0f,0.0f));

	FArm_vertices.push_back(temp[0]);
	FArm_vertices.push_back(temp[1]);
	FArm_vertices.push_back(temp[2]);

	FArm_vertices.push_back(temp[0]);
	FArm_vertices.push_back(temp[2]);
	FArm_vertices.push_back(temp[3]);

	FArm_vertices.push_back(temp[3]);
	FArm_vertices.push_back(temp[2]);
	FArm_vertices.push_back(temp[4]);

	FArm_vertices.push_back(temp[3]);
	FArm_vertices.push_back(temp[4]);
	FArm_vertices.push_back(temp[5]);

	FArm_vertices.push_back(temp[5]);
	FArm_vertices.push_back(temp[4]);
	FArm_vertices.push_back(temp[6]);

	FArm_vertices.push_back(temp[5]);
	FArm_vertices.push_back(temp[6]);
	FArm_vertices.push_back(temp[7]);

	FArm_vertices.push_back(temp[0]);
	FArm_vertices.push_back(temp[5]);
	FArm_vertices.push_back(temp[7]);

	FArm_vertices.push_back(temp[0]);
	FArm_vertices.push_back(temp[3]);
	FArm_vertices.push_back(temp[5]);

	FArm_vertices.push_back(temp[2]);
	FArm_vertices.push_back(temp[6]);
	FArm_vertices.push_back(temp[4]);

	FArm_vertices.push_back(temp[2]);
	FArm_vertices.push_back(temp[1]);
	FArm_vertices.push_back(temp[6]);

	FArm_vertices.push_back(temp[1]);
	FArm_vertices.push_back(temp[7]);
	FArm_vertices.push_back(temp[6]);

	FArm_vertices.push_back(temp[1]);
	FArm_vertices.push_back(temp[0]);
	FArm_vertices.push_back(temp[7]);

	translation_FArm = glm::translate(0.0f,0.8f,0.0f);
	ModleMatrix_FArm =  translation_FArm * rotation_FArm * scale_FArm;
	MVP_FArm = MVP * ModleMatrix_FArm;
}

void initialize_Join(){

	for (int i = 0; i < base_vertices.size(); i++){

		Join_vertices.push_back(base_vertices[i]);  
		Join_vertices[i].color  = glm::vec4(0.5f,0.5f,1.0f,1.0f);
	}

	scale_Join  = glm::scale(0.2f,2.0f,0.2f);
	rotation_Join =  glm::rotate(90.0f,glm::vec3(1,0,0));
	translation_Join_X = glm::translate(2.5f,0.0f,0.0f);
	translation_Join_Y = glm::translate(0.0f,0.8f,0.0f);
	ModleMatrix_Join =  translation_Join_X * translation_Join_Y * rotation_Join * scale_Join;
	MVP_Join = MVP * ModleMatrix_Join;
}

void initialize_SArm(){

	for (int i = 0; i < base_vertices.size(); i++){

		SArm_vertices.push_back(base_vertices[i]);  
		SArm_vertices[i].color  = glm::vec4(1.0f,0.5f,0.0f,1.0f);
	}

	scale_SArm  = glm::scale(0.056f,3.6f,0.056f);
	rotation_SArm =  glm::rotate(90.0f,glm::vec3(0,0,1));
	translation_SArm_X_1 = glm::translate(0.54f,0.0f,0.0f);
	translation_SArm_X_2 = glm::translate(2.5f,0.0f,0.0f);
	translation_SArm_Y = glm::translate(0.0f,0.8f,0.0f);
	ModleMatrix_SArm =  translation_SArm_X_1 * translation_SArm_X_2 * translation_SArm_Y * rotation_SArm * scale_SArm;
	MVP_SArm = MVP * ModleMatrix_SArm;
}

void initialize_Pen(){

	for (int i = 0; i < base_vertices.size(); i++){

		Pen_vertices.push_back(base_vertices[i]);  
		Pen_vertices[i].color  = glm::vec4(0.6f,0.8f,0.6f,1.0f);
	}
    
	scale_Pen  = glm::scale(0.056f,3.6f,0.056f);
	rotation_Pen =  glm::rotate(90.0f,glm::vec3(0,0,1));
	translation_Pen_X_1 = glm::translate(2.5f,0.0f,0.0f);
	translation_Pen_X_2 = glm::translate(1.08f,0.0f,0.0f);
	translation_Pen_X_3 = glm::translate(0.54f,0.0f,0.0f);
	translation_Pen_Y = glm::translate(0.0f,0.8f,0.0f);
	ModleMatrix_Pen = translation_Pen_X_3 * translation_Pen_X_2 * translation_Pen_X_1 * translation_Pen_Y * rotation_Pen * scale_Pen;
	MVP_Pen = MVP * ModleMatrix_Pen; 
}

void initialize_Botton(){

	for (int i = 0; i < FArm_vertices.size(); i++){

		Botton_vertices.push_back(FArm_vertices[i]);  
		Botton_vertices[i].color  = glm::vec4(0.0f,0.0f,1.0f,1.0f);
	}
    
	scale_Botton  = glm::scale(0.08f,0.5f,0.5f);
	translation_Botton_X_1 = glm::translate(2.5f,0.0f,0.0f);
	translation_Botton_X_2 = glm::translate(1.08f,0.0f,0.0f);
	translation_Botton_X_3 = glm::translate(0.7f,0.0f,0.0f);
	translation_Botton_Y = glm::translate(0.0f,0.8f,0.0f);
	ModleMatrix_Botton = translation_Botton_X_3 * translation_Botton_X_2 * translation_Botton_X_1 
		* translation_Botton_Y * rotation_Botton * scale_Botton;
	MVP_Botton = MVP * ModleMatrix_Botton;
}



// MOUSE handling *******************************************

int last_x, last_y;
int selected_idx = -1;

void mouse(int button, int state, int x, int y ){
	if(state == GLUT_DOWN){
        
	}else{
        
	}

    last_x = x;
    last_y = y;
    
}


void motion( int x, int y){
    GLint viewport[4];

    glutPostRedisplay();
    
}

// KEYBOARD handling *******************************************

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r':
        graphics_init();
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
	case 'c':
		c_pressed = !c_pressed;
        break;
	case 'p':
		p_pressed = !p_pressed;
		break;
    case '1':
		botton1_pressed = !botton1_pressed;
        break;
	case '2':
		botton2_pressed = !botton2_pressed;
        break;
	case '3':
		botton3_pressed = !botton3_pressed;
		break;
	case '4':
		botton4_pressed = !botton4_pressed;
		break;
	case '5':
		botton5_pressed = !botton5_pressed;
		break;
	case 'l':
		if (l_botton == 0)
		   l_botton = 1;
		else
           l_botton = 0;	
		break;
    }
    glutPostRedisplay();
}

void special_key(int key, int x, int y)
{
    // TODO: capture the arrow key here	

	glm::mat4 static move; //store the move of the object
	glm::mat4 static round; //when the top turn left and right
	glm::mat4 static action;//when second arm act up and down
	glm::mat4 static up_down;//when the pen move up and down
	glm::mat4 static left_right;//when the pen move left and right
	
	if (c_pressed == true) {
		
	    switch (key) {
	    case GLUT_KEY_UP:
			angle2 = angle2+1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
            break;
	    case GLUT_KEY_DOWN:
			angle2 = angle2-1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
            break;
	    case GLUT_KEY_LEFT:
			angle = angle-1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
            break;
	    case GLUT_KEY_RIGHT:
			angle = angle+1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
            break;
	    }
		MVP = ProjectionMatrix * ViewMatrix;
		MVP_object = MVP * ModleMatrix_obj;
	    MVP_top = MVP * ModleMatrix_top;

		MVP_FArm = MVP * ModleMatrix_FArm;

		MVP_Join = MVP * ModleMatrix_Join;

	    MVP_SArm = MVP * ModleMatrix_SArm;

		MVP_Pen = MVP * ModleMatrix_Pen;

		MVP_Botton = MVP * ModleMatrix_Botton;

	}

	if (botton1_pressed == true) {
		c_pressed = false;
        GLfloat static x_axis = 0, z_axis = 0;
	    switch (key) {
	    case GLUT_KEY_UP:
			z_axis--;
			ModleMatrix_obj = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			translation_top = glm::translate(0.0f,0.0f,-1.0f) * translation_top;
			translation_FArm = glm::translate(0.0f,0.0f,-1.0f) * translation_FArm;
			move = glm::translate(0.0f,0.0f,-1.0f) * move;
            break;
	    case GLUT_KEY_DOWN:
			z_axis++;
			ModleMatrix_obj = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			translation_top = glm::translate(0.0f,0.0f,1.0f) * translation_top;
			translation_FArm = glm::translate(0.0f,0.0f,1.0f) * translation_FArm;
			move = glm::translate(0.0f,0.0f,1.0f) * move;
            break;
	    case GLUT_KEY_LEFT:
			x_axis--;
			ModleMatrix_obj = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			translation_top = glm::translate(-1.0f,0.0f,0.0f) * translation_top;
			translation_FArm = glm::translate(-1.0f,0.0f,0.0f) * translation_FArm;
			move = glm::translate(-1.0f,0.0f,0.0f) * move;
            break;
	    case GLUT_KEY_RIGHT:
			x_axis++;
		    ModleMatrix_obj = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			translation_top = glm::translate(1.0f,0.0f,0.0f) * translation_top;
			translation_FArm = glm::translate(1.0f,0.0f,0.0f) * translation_FArm;
			move = glm::translate(1.0f,0.0f,0.0f) * move;
            break;
	    }

	    MVP_object = MVP * ModleMatrix_obj;

		ModleMatrix_top =  translation_top * round * rotation_top * scale_top;
	    MVP_top = MVP * ModleMatrix_top;

		ModleMatrix_FArm = translation_FArm * round * rotation_FArm * scale_FArm;
		MVP_FArm = MVP * ModleMatrix_FArm;

		ModleMatrix_Join = move * translation_Join_Y * round * rotation_FArm * translation_Join_X *  rotation_Join * scale_Join;
		MVP_Join = MVP * ModleMatrix_Join;

		ModleMatrix_SArm = move * translation_SArm_Y * round * rotation_FArm * translation_SArm_X_2 * action * translation_SArm_X_1 * rotation_SArm * scale_SArm;
	    MVP_SArm = MVP * ModleMatrix_SArm;

		ModleMatrix_Pen = move * translation_Pen_Y * round * rotation_FArm * translation_Pen_X_1 
			* action * translation_Pen_X_2 * left_right * up_down * translation_Pen_X_3 * rotation_Pen * scale_Pen;
		MVP_Pen = MVP * ModleMatrix_Pen;

		ModleMatrix_Botton = move * translation_Botton_Y * round * rotation_FArm * translation_Botton_X_1 
			* action * translation_Botton_X_2 * left_right * up_down * translation_Botton_X_3 * rotation_Pen * scale_Botton;
		MVP_Botton = MVP * ModleMatrix_Botton;
	}

	if (botton2_pressed == true && c_pressed == false && botton1_pressed == false) {
		
	  switch (key) {
	    case GLUT_KEY_LEFT:	
		    round = glm::rotate(20.0f,glm::vec3(0,1,0)) * round;
            break;
	    case GLUT_KEY_RIGHT:
			round = glm::rotate(-20.0f, glm::vec3(0,1,0)) * round;
            break;
	    }
 	    ModleMatrix_top =  translation_top * round * rotation_top * scale_top;
	    MVP_top = MVP * ModleMatrix_top;

		ModleMatrix_FArm = translation_FArm * round * rotation_FArm * scale_FArm;
		MVP_FArm = MVP * ModleMatrix_FArm;

		ModleMatrix_Join = move * translation_Join_Y * round * rotation_FArm * translation_Join_X *  rotation_Join * scale_Join;
		MVP_Join = MVP * ModleMatrix_Join;

		ModleMatrix_SArm = move * translation_SArm_Y * round * rotation_FArm * translation_SArm_X_2 * action * translation_SArm_X_1 * rotation_SArm * scale_SArm;
	    MVP_SArm = MVP * ModleMatrix_SArm;

		ModleMatrix_Pen = move * translation_Pen_Y * round * rotation_FArm * translation_Pen_X_1 
			* action * translation_Pen_X_2 * left_right * up_down * translation_Pen_X_3 * rotation_Pen * scale_Pen;
		MVP_Pen = MVP * ModleMatrix_Pen;

		ModleMatrix_Botton = move * translation_Botton_Y * round * rotation_FArm * translation_Botton_X_1 
			* action * translation_Botton_X_2 * left_right * up_down * translation_Botton_X_3 * rotation_Pen * scale_Botton;
		MVP_Botton = MVP * ModleMatrix_Botton;
	}

	if (botton3_pressed == true && c_pressed == false && botton1_pressed ==false && botton2_pressed == false) {

	  switch (key) {
	    case GLUT_KEY_UP:	
		    rotation_FArm = glm::rotate(20.0f,glm::vec3(0,0,1)) * rotation_FArm;
            break;
	    case GLUT_KEY_DOWN:
			rotation_FArm = glm::rotate(-20.0f, glm::vec3(0,0,1)) * rotation_FArm;
            break;
	    }

		ModleMatrix_FArm = translation_FArm * round * rotation_FArm * scale_FArm;
		MVP_FArm = MVP * ModleMatrix_FArm;

		ModleMatrix_Join = move * translation_Join_Y * round * rotation_FArm * translation_Join_X *  rotation_Join * scale_Join;
		MVP_Join = MVP * ModleMatrix_Join;

		ModleMatrix_SArm = move * translation_SArm_Y * round * rotation_FArm * translation_SArm_X_2 * action * translation_SArm_X_1 * rotation_SArm * scale_SArm;
	    MVP_SArm = MVP * ModleMatrix_SArm;

		ModleMatrix_Pen = move * translation_Pen_Y * round * rotation_FArm * translation_Pen_X_1 
			* action * translation_Pen_X_2 * left_right * up_down * translation_Pen_X_3 * rotation_Pen * scale_Pen;
		MVP_Pen = MVP * ModleMatrix_Pen;

		ModleMatrix_Botton = move * translation_Botton_Y * round * rotation_FArm * translation_Botton_X_1 
			* action * translation_Botton_X_2 * left_right * up_down * translation_Botton_X_3 * rotation_Pen * scale_Botton;
		MVP_Botton = MVP * ModleMatrix_Botton;

	}

	if (botton4_pressed == true && c_pressed == false && botton1_pressed ==false && botton2_pressed == false && botton3_pressed == false) {

	  switch (key) {
	    case GLUT_KEY_UP:	
		    action = glm::rotate(20.0f,glm::vec3(0,0,1)) * action;
            break;
	    case GLUT_KEY_DOWN:
			action = glm::rotate(-20.0f, glm::vec3(0,0,1)) * action;
            break;
	    }

	    ModleMatrix_SArm = move * translation_SArm_Y * round * rotation_FArm * translation_SArm_X_2 * action * translation_SArm_X_1 * rotation_SArm * scale_SArm;
	    MVP_SArm = MVP * ModleMatrix_SArm;

		ModleMatrix_Pen = move * translation_Pen_Y * round * rotation_FArm * translation_Pen_X_1 
			* action * translation_Pen_X_2 * left_right * up_down * translation_Pen_X_3 * rotation_Pen * scale_Pen;
		MVP_Pen = MVP * ModleMatrix_Pen;

		ModleMatrix_Botton = move * translation_Botton_Y * round * rotation_FArm * translation_Botton_X_1 
			* action * translation_Botton_X_2 * left_right * up_down * translation_Botton_X_3 * rotation_Pen * scale_Botton;
		MVP_Botton = MVP * ModleMatrix_Botton;

	}

	if (botton5_pressed == true && c_pressed == false && botton1_pressed ==false && botton2_pressed == false 
		&& botton3_pressed == false && botton4_pressed == false) {

		 switch (key) {
			case GLUT_KEY_LEFT:						
				if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
					rotation_Pen = glm::rotate(20.0f,glm::vec3(1,0,0)) * rotation_Pen;
				else
                    left_right = glm::rotate(20.0f,glm::vec3(0,1,0)) * left_right;
				break;
			case GLUT_KEY_RIGHT:
				if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
					rotation_Pen = glm::rotate(-20.0f,glm::vec3(1,0,0)) * rotation_Pen;
				else
				    left_right = glm::rotate(-20.0f, glm::vec3(0,1,0)) * left_right;
				break;
			case GLUT_KEY_UP:
				if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
						;
				else
				     up_down = glm::rotate(20.0f,glm::vec3(0,0,1)) * up_down;
			    break;
			case GLUT_KEY_DOWN:
				if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
						;
				else
					  up_down = glm::rotate(-20.0f, glm::vec3(0,0,1)) * up_down;
				break;
			}
		}

	    ModleMatrix_Pen = move * translation_Pen_Y * round * rotation_FArm * translation_Pen_X_1 
			* action * translation_Pen_X_2 * left_right * up_down * translation_Pen_X_3 * rotation_Pen * scale_Pen;
		MVP_Pen = MVP * ModleMatrix_Pen;

		ModleMatrix_Botton = move * translation_Botton_Y * round * rotation_FArm * translation_Botton_X_1 
			* action * translation_Botton_X_2 * left_right * up_down * translation_Botton_X_3 * rotation_Pen * scale_Botton;
		MVP_Botton = MVP * ModleMatrix_Botton;

    glutPostRedisplay();

}

// DISPLAY and RENDERING functions *************************

void draw_grid(){
    
    GLuint vao;
    GLuint grid_vbo;
    
    GLuint position_location;
    GLuint color_location;
	GLuint normal_location;
    GLuint MVP_location;
	GLuint Model_matrix_location;
	GLuint inverse_Model_location;
	GLuint inverse_View_location;
	GLuint light_botton;

    // specify the shaders we want to use
    glUseProgram(program);

    // get the input variable location in this shader
    position_location = glGetAttribLocation(program, "in_position");
    color_location = glGetAttribLocation(program, "in_color");
	normal_location = glGetAttribLocation(program, "in_normal");
	MVP_location = glGetUniformLocation(program, "MVP");
	Model_matrix_location = glGetUniformLocation(program, "ModelMatrix");
	inverse_Model_location = glGetUniformLocation(program, "tr_in_Model");
	inverse_View_location = glGetUniformLocation(program, "v_inverse");
	light_botton = glGetUniformLocation(program, "light_botton");

	glUniformMatrix4fv(MVP_location,1,GL_FALSE,&MVP[0][0]);
	glUniformMatrix4fv(Model_matrix_location,1,GL_FALSE,&ModleMatrix[0][0]);
	glUniformMatrix3fv(inverse_Model_location,1,GL_FALSE,&Modle_s[0][0]);
	glUniform1i(light_botton,l_botton);
        
    // create and bind a VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
    glEnableVertexAttribArray(color_location);
	glEnableVertexAttribArray(normal_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &grid_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);

    // initialize the vertex buffer with the vertex data
    
    glBufferData(GL_ARRAY_BUFFER, grid_vertices.size() * sizeof(Vertex), &grid_vertices[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(normal_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

    // draw points
    glPointSize(10);
    glDrawArrays(GL_LINES, 0, grid_vertices.size());

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &grid_vbo);
    glDeleteVertexArrays(1, &vao);
}
void draw(){

	draw_grid();

	if (p_pressed == true){

		glm::mat3 obj = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_obj)));
		glm::mat3 top = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_top)));
		glm::mat3 FArm = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_FArm)));
		glm::mat3 Join = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_Join)));
		glm::mat3 SArm = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_SArm)));
		glm::mat3 Pen = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_Pen)));
		glm::mat3 Botton = glm::transpose(glm::inverse(glm::mat3(ModleMatrix_Botton)));

		Inverse_view = glm::inverse(ViewMatrix);

        draw_parts(base_vertices,GL_TRIANGLES,MVP_object,ModleMatrix_obj,obj);
		draw_parts(top_vertices,GL_TRIANGLES,MVP_top,ModleMatrix_top,top);//glm::mat3(rotation_top)
		draw_parts(FArm_vertices,GL_TRIANGLES,MVP_FArm,ModleMatrix_FArm,FArm);//glm::mat3(rotation_FArm)
		draw_parts(Join_vertices,GL_TRIANGLES,MVP_Join,ModleMatrix_Join,Join);//glm::mat3(rotation_Join)
		draw_parts(SArm_vertices,GL_TRIANGLES,MVP_SArm,ModleMatrix_SArm,SArm);//glm::mat3(rotation_SArm)
		draw_parts(Pen_vertices,GL_TRIANGLES,MVP_Pen,ModleMatrix_Pen,Pen);//glm::mat3(rotation_Pen)
		draw_parts(Botton_vertices,GL_TRIANGLES,MVP_Botton,ModleMatrix_Botton,Botton);//glm::mat3(rotation_Botton)

//		draw_parts(V_test,GL_LINES,MVP_object);
		
	}
}
void draw_parts(V_Vertex obj, GLenum mode, glm::mat4 mvp, glm::mat4 model_matrix,glm::mat3 inverse_model){

	GLuint vao;
	GLuint vbo;
	GLuint position_location;
    GLuint color_location;
	GLuint normal_location;
    GLuint MVP_location;
	GLuint Model_matrix_location;
	GLuint inverse_Model_location;
	GLuint inverse_View_location;
	GLuint light_botton;
	
    // specify the shaders we want to use
    glUseProgram(program);

    // get the input variable location in this shader
    position_location = glGetAttribLocation(program, "in_position");
    color_location = glGetAttribLocation(program, "in_color");
	normal_location = glGetAttribLocation(program, "in_normal");
	MVP_location = glGetUniformLocation(program, "MVP");
	Model_matrix_location = glGetUniformLocation(program, "ModelMatrix");
	inverse_Model_location = glGetUniformLocation(program, "tr_in_Model");
	inverse_View_location = glGetUniformLocation(program, "v_inverse");
	light_botton = glGetUniformLocation(program, "light_botton");

	glUniformMatrix4fv(MVP_location,1,GL_FALSE,&mvp[0][0]);
	glUniformMatrix4fv(Model_matrix_location,1,GL_FALSE,&model_matrix[0][0]);
	glUniformMatrix3fv(inverse_Model_location,1,GL_FALSE,&inverse_model[0][0]);

	glUniformMatrix4fv(inverse_View_location,1,GL_FALSE,&Inverse_view[0][0]);
	glUniform1i(light_botton,l_botton);
	  // create and bind a VAO
	
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
    glEnableVertexAttribArray(color_location);
	glEnableVertexAttribArray(normal_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // initialize the vertex buffer with the vertex data
    
	glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(Vertex), &obj[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
	glVertexAttribPointer(normal_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));

    // draw points
    glPointSize(10);
    glDrawArrays(mode, 0, obj.size());

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

}

void display(){
    // Clear Viewport
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 //   draw_grid();
    draw();
    glFlush();
    glutSwapBuffers();

}

void reshape(int width, int height){
    // Clip the view port to match our ratio
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

void graphics_init(){

    // init vertex shader
    read_shader_source_code("vs.glsl", buf, 2048);
    cout << buf << endl;
    int vertex_shader_source_length = strlen(buf);
    const char *vertex_shader_sources[] = { buf };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_sources, &vertex_shader_source_length);
    glCompileShader(vertex_shader);
    
    // init fragment shader 
    read_shader_source_code("fs.glsl", buf, 1024); 
    int fragment_shader_source_length = strlen(buf);
    const char *fragment_shader_sources[] = { buf };
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_sources, &fragment_shader_source_length);
    glCompileShader(fragment_shader);

    // init program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
  
    // enable depth test
    glEnable(GL_DEPTH_TEST);


}

void print_info()
{
    fprintf(
        stdout,
        "INFO: OpenGL Version: %s\n",
        glGetString(GL_VERSION)
        );
    fprintf(stdout, "INFO: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    if(glewIsSupported("GL_ARB_debug_output"))
    {
        fprintf(stdout, "INFO: Support ARB_debug_output\n");
    }
    else
    {
        fprintf(stdout, "INFO: Not support ARB_debug_output\n");
    }
}

int main(int argc,char * argv[]){

    // Setup GLUT
    glutInit(&argc,argv);

    glutInitContextVersion (3, 0);
    glutInitContextFlags (GLUT_DEBUG);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB     | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (window_width,window_height);
    glutCreateWindow("First step - OpenGL 3");
  
    // Setup GLEW
    glewExperimental = true;
    GLenum err = glewInit();
    if(GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    print_info();

    if(glewIsSupported("GL_ARB_debug_output"))
        VSDebugLib::init();
    // Initialize OpenGL
    graphics_init ();

    initialize_points();

    // set up callbacks
    glutReshapeFunc(&reshape);
    glutDisplayFunc(&display);
    glutKeyboardFunc(&keyboard);
    glutMouseFunc(&mouse);
    glutMotionFunc(&motion);
    glutSpecialFunc(&special_key);

    // main loop
    glutMainLoop();
    return 0;
}


/*	Vertex temp;
	GLfloat angle_circle = 5;

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(0.0f,1.0f,0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),1+sin(angle_circle*i*PI/180),0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),1+sin(angle_circle*(i+1)*PI/180),0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(0.0f,1.0f,-0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),1+sin(angle_circle*i*PI/180),-0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),1+sin(angle_circle*(i+1)*PI/180),-0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),1+sin(angle_circle*i*PI/180),0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),1+sin(angle_circle*i*PI/180),-0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),1+sin(angle_circle*(i+1)*PI/180),-0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),1+sin(angle_circle*i*PI/180),0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),1+sin(angle_circle*(i+1)*PI/180),-0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),1+sin(angle_circle*(i+1)*PI/180),0.8f,1.0f);
		temp.color = glm::vec4(0.0f,0.0f,1.0f,1.0f);
		top_vertices.push_back(temp);
	}
Vertex temp;
	GLfloat angle_circle = 5;

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(2.0f,1.0f,0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*i*PI/180),1+0.3*sin(angle_circle*i*PI/180),0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*(i+1)*PI/180),1+0.3*sin(angle_circle*(i+1)*PI/180),0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(2.0f,1.0f,-0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*i*PI/180),1+0.3*sin(angle_circle*i*PI/180),-0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*(i+1)*PI/180),1+0.3*sin(angle_circle*(i+1)*PI/180),-0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*i*PI/180),1+0.3*sin(angle_circle*i*PI/180),0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*i*PI/180),1+0.3*sin(angle_circle*i*PI/180),-0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*(i+1)*PI/180),1+0.3*sin(angle_circle*(i+1)*PI/180),0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*(i+1)*PI/180),1+0.3*sin(angle_circle*(i+1)*PI/180),0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*i*PI/180),1+0.3*sin(angle_circle*i*PI/180),-0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);

		temp.pos = glm::vec4(2+0.3*cos(angle_circle*(i+1)*PI/180),1+0.3*sin(angle_circle*(i+1)*PI/180),-0.25f,1.0f);
		temp.color = glm::vec4(0.5f,0.5f,1.0f,1.0f);
		Join_vertices.push_back(temp);
	}
Vertex temp;
	GLfloat angle_circle = 5;

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(2.0f,1.0f,0.0f,1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(2,1+0.08*sin(angle_circle*i*PI/180),0.08*cos(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(2,1+0.08*sin(angle_circle*(i+1)*PI/180),0.08*cos(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(3.0f,1.0f,0.0f,1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(3.0f,1+0.08*sin(angle_circle*i*PI/180),0.08*cos(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(3.0f,1+0.08*sin(angle_circle*(i+1)*PI/180),0.08*cos(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(2,1+0.08*sin(angle_circle*i*PI/180),0.08*cos(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(3.0f,1+0.08*sin(angle_circle*i*PI/180),0.08*cos(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(2,1+0.08*sin(angle_circle*(i+1)*PI/180),0.08*cos(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(2,1+0.08*sin(angle_circle*(i+1)*PI/180),0.08*cos(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(3.0f,1+0.08*sin(angle_circle*i*PI/180),0.08*cos(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);

		temp.pos = glm::vec4(3.0f,1+0.08*sin(angle_circle*(i+1)*PI/180),0.08*cos(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(1.0f,0.5f,0.0f,1.0f);
		SArm_vertices.push_back(temp);
	}*/