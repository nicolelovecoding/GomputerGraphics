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



// CONTSTANTS
const float PI = 3.1415926;
const int window_width = 500, window_height = 500;
const char * window_title = "HW2";


struct Vertex
{
    glm::vec4 pos;
    glm::vec4 color;
};

double angle = 45;
double angle2 = 45;
glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
glm::mat4 ProjectionMatrix = glm::ortho(-5.0f,5.0f,-5.0f,5.0f,0.1f,100.0f);
glm::mat4 ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
glm::mat4 MVP = ProjectionMatrix * ViewMatrix;
glm::mat4 MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;

// runtime variable
char buf[1024];
vector<Vertex> grid_vertices;
bool C_pressed = false;
bool botton1_pressed = false;
bool botton2_pressed = false;
bool botton3_pressed = false;
Vertex Cube[36];
vector<Vertex> cylinder_vertices;
vector<Vertex> icosahedron_vertices;
GLuint vertex_shader, fragment_shader, program;

// function Prototype 
void graphics_init();
void initialize_cube();
void initialize_cylinder();
void initialize_icosahedron();

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

    initialize_cube();
	initialize_cylinder();
	initialize_icosahedron();
}

void initialize_cube(){
	MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
	Cube[0].pos = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);//1
	Cube[1].pos = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	Cube[2].pos = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);

	Cube[3].pos = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);//2
	Cube[4].pos = glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f);
	Cube[5].pos = glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f);

	Cube[6].pos = glm::vec4(1.0f,-1.0f, 1.0f, 1.0f);//3
	Cube[7].pos = glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f);
	Cube[8].pos = glm::vec4(1.0f,-1.0f,-1.0f, 1.0f);

	Cube[9].pos = glm::vec4(1.0f, 1.0f,-1.0f, 1.0f);//4
	Cube[10].pos = glm::vec4(1.0f,-1.0f,-1.0f, 1.0f);
	Cube[11].pos = glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f);

	Cube[12].pos = glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f);//5
	Cube[13].pos = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
	Cube[14].pos = glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f);

	Cube[15].pos = glm::vec4(1.0f,-1.0f, 1.0f, 1.0f);//6
	Cube[16].pos = glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f);
	Cube[17].pos = glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f);

	Cube[18].pos = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);//7
	Cube[19].pos = glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f);
	Cube[20].pos = glm::vec4(1.0f,-1.0f, 1.0f, 1.0f);

	Cube[21].pos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//8
	Cube[22].pos = glm::vec4(1.0f,-1.0f,-1.0f, 1.0f);
	Cube[23].pos = glm::vec4(1.0f, 1.0f,-1.0f, 1.0f);

	Cube[24].pos = glm::vec4(1.0f,-1.0f,-1.0f, 1.0f);//9
	Cube[25].pos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Cube[26].pos = glm::vec4(1.0f,-1.0f, 1.0f, 1.0f);

	Cube[27].pos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//10
	Cube[28].pos = glm::vec4(1.0f, 1.0f,-1.0f, 1.0f);
	Cube[29].pos = glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f);

	Cube[30].pos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//11
	Cube[31].pos = glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f);
	Cube[32].pos = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);

	Cube[33].pos = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//12
	Cube[34].pos = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
	Cube[35].pos = glm::vec4(1.0f,-1.0f, 1.0f, 1.0f);


	for (int i = 0; i < 36; i++){

	Cube[i].color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
}

void initialize_cylinder(){

	Vertex temp;
	GLfloat angle_circle = 5;

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(0.0f,2.0f,0.0f,1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),2.0f,sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),2.0f,sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(0.0f,0.0f,0.0f,1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),0.0f,sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),0.0f,sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);
	}

	for (int i = 0; i < int(360/angle_circle); i++){

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),2.0f,sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),0.0f,sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),0.0f,sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*i*PI/180),2.0f,sin(angle_circle*i*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),2.0f,sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);

		temp.pos = glm::vec4(cos(angle_circle*(i+1)*PI/180),0.0f,sin(angle_circle*(i+1)*PI/180),1.0f);
		temp.color = glm::vec4(0.0f,1.0f,1.0f,1.0f);
		cylinder_vertices.push_back(temp);
	}
}

void initialize_icosahedron(){

	Vertex temp[12];
	temp[0].pos = glm::vec4(1.0f,0.0f,(1+sqrt(5.f))/2,1.0f);
	temp[0].color = glm::vec4(1.0f,0.3f,0.0f,1.0f);

	temp[1].pos = glm::vec4(-1.0f,0.0f,(1+sqrt(5.f))/2,1.0f);
	temp[1].color = glm::vec4(1.0f,0.0f,0.2f,1.0f);

	temp[2].pos = glm::vec4(-1.0f,0.0f,-(1+sqrt(5.f))/2,1.0f);
	temp[2].color = glm::vec4(1.0f,0.2f,0.0f,1.0f);

	temp[3].pos = glm::vec4(1.0f,0.0f,-(1+sqrt(5.f))/2,1.0f);
	temp[3].color = glm::vec4(1.0f,0.1f,0.0f,1.0f);

	temp[4].pos = glm::vec4(0.0f,(1+sqrt(5.f))/2,1.0f,1.0f);
	temp[4].color = glm::vec4(1.0f,0.7f,0.0f,1.0f);

	temp[5].pos = glm::vec4(0.0f,(1+sqrt(5.f))/2,-1.0f,1.0f);
	temp[5].color = glm::vec4(0.0f,1.0f,0.0f,1.0f);

	temp[6].pos = glm::vec4(0.0f,-(1+sqrt(5.f))/2,-1.0f,1.0f);
	temp[6].color = glm::vec4(0.0f,0.0f,0.2f,1.0f);

	temp[7].pos = glm::vec4(0.0f,-(1+sqrt(5.f))/2,1.0f,1.0f);
	temp[7].color = glm::vec4(1.0f,0.0f,0.0f,1.0f);

	temp[8].pos = glm::vec4((1+sqrt(5.f))/2,1.0f,0.0f,1.0f);
	temp[8].color = glm::vec4(0.3f,0.5f,0.0f,1.0f);

	temp[9].pos = glm::vec4((1+sqrt(5.f))/2,-1.0f,0.0f,1.0f);
	temp[9].color = glm::vec4(0.2f,0.1f,0.0f,1.0f);

	temp[10].pos = glm::vec4(-(1+sqrt(5.f))/2,-1.0f,0.0f,1.0f);
	temp[10].color = glm::vec4(1.0f,0.7f,0.0f,1.0f);

	temp[11].pos = glm::vec4(-(1+sqrt(5.f))/2,1.0f,0.0f,1.0f);
	temp[11].color = glm::vec4(0.6f,0.8f,0.0f,1.0f);

	icosahedron_vertices.push_back(temp[4]);
	icosahedron_vertices.push_back(temp[0]);
	icosahedron_vertices.push_back(temp[8]);

	icosahedron_vertices.push_back(temp[4]);
	icosahedron_vertices.push_back(temp[1]);
	icosahedron_vertices.push_back(temp[0]);

	icosahedron_vertices.push_back(temp[4]);
	icosahedron_vertices.push_back(temp[1]);
	icosahedron_vertices.push_back(temp[11]);

	icosahedron_vertices.push_back(temp[1]);
	icosahedron_vertices.push_back(temp[11]);
	icosahedron_vertices.push_back(temp[10]);

	icosahedron_vertices.push_back(temp[11]);
	icosahedron_vertices.push_back(temp[4]);
	icosahedron_vertices.push_back(temp[5]);

	icosahedron_vertices.push_back(temp[11]);
	icosahedron_vertices.push_back(temp[10]);
	icosahedron_vertices.push_back(temp[2]);

	icosahedron_vertices.push_back(temp[11]);
	icosahedron_vertices.push_back(temp[2]);
	icosahedron_vertices.push_back(temp[5]);

	icosahedron_vertices.push_back(temp[5]);
	icosahedron_vertices.push_back(temp[2]);
	icosahedron_vertices.push_back(temp[3]);

	icosahedron_vertices.push_back(temp[5]);
	icosahedron_vertices.push_back(temp[8]);
	icosahedron_vertices.push_back(temp[3]);

	icosahedron_vertices.push_back(temp[4]);
	icosahedron_vertices.push_back(temp[5]);
	icosahedron_vertices.push_back(temp[8]);
/////////////////////////////////////////////////////
	icosahedron_vertices.push_back(temp[7]);
	icosahedron_vertices.push_back(temp[0]);
	icosahedron_vertices.push_back(temp[9]);

	icosahedron_vertices.push_back(temp[7]);
	icosahedron_vertices.push_back(temp[1]);
	icosahedron_vertices.push_back(temp[0]);

	icosahedron_vertices.push_back(temp[1]);
	icosahedron_vertices.push_back(temp[10]);
	icosahedron_vertices.push_back(temp[7]);

	icosahedron_vertices.push_back(temp[0]);
	icosahedron_vertices.push_back(temp[8]);
	icosahedron_vertices.push_back(temp[9]);

	icosahedron_vertices.push_back(temp[8]);
	icosahedron_vertices.push_back(temp[3]);
	icosahedron_vertices.push_back(temp[9]);

	icosahedron_vertices.push_back(temp[9]);
	icosahedron_vertices.push_back(temp[6]);
	icosahedron_vertices.push_back(temp[3]);

	icosahedron_vertices.push_back(temp[6]);
	icosahedron_vertices.push_back(temp[2]);
	icosahedron_vertices.push_back(temp[3]);

	icosahedron_vertices.push_back(temp[6]);
	icosahedron_vertices.push_back(temp[10]);
	icosahedron_vertices.push_back(temp[2]);

	icosahedron_vertices.push_back(temp[6]);
	icosahedron_vertices.push_back(temp[7]);
	icosahedron_vertices.push_back(temp[10]);

	icosahedron_vertices.push_back(temp[7]);
	icosahedron_vertices.push_back(temp[6]);
	icosahedron_vertices.push_back(temp[9]);


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
		C_pressed = !C_pressed;
        break;
    case '1':
		botton1_pressed = !botton1_pressed;
		ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
        break;
	case '2':
		botton2_pressed = !botton2_pressed;
		ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
        break;
	case '3':
		botton3_pressed = !botton3_pressed;
		ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
		break;
    }
    glutPostRedisplay();
}

void special_key(int key, int x, int y)
{
    // TODO: capture the arrow key here	
	if (C_pressed == true) {

	    switch (key) {
	    case GLUT_KEY_UP:
			angle2 = angle2+1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
			MVP = ProjectionMatrix * ViewMatrix;

            break;
	    case GLUT_KEY_DOWN:
			angle2 = angle2-1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
			MVP = ProjectionMatrix * ViewMatrix;

            break;
	    case GLUT_KEY_LEFT:
			angle = angle-1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
			MVP = ProjectionMatrix * ViewMatrix;

            break;
	    case GLUT_KEY_RIGHT:
			angle = angle+1;
			ViewMatrix = glm::lookAt(glm::vec3(cos(angle2*PI/180)*sin(angle*PI/180)*10,sin(angle2*PI/180)*10,
	                     cos(angle2*PI/180)*cos(angle*PI/180)*10),glm::vec3(0,0,0),glm::vec3(0,1,0));
			MVP = ProjectionMatrix * ViewMatrix;

            break;
	    }
	}

	if (botton1_pressed == true || botton2_pressed == true || botton3_pressed == true) {

		GLfloat static x_axis = 0, z_axis = 0;

	    switch (key) {
	    case GLUT_KEY_UP:
			z_axis--;
			ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
            break;
	    case GLUT_KEY_DOWN:
			z_axis++;
			ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
            break;
	    case GLUT_KEY_LEFT:
			x_axis--;
			ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
            break;
	    case GLUT_KEY_RIGHT:
			x_axis++;
		    ModleMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x_axis,0.0f,z_axis));
			MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
            break;
	    }
	}
        glutPostRedisplay();

}

// DISPLAY and RENDERING functions *************************

void draw_grid(){
    
    GLuint vao;
	GLuint vao2;
	GLuint vao3;
	GLuint vao4;
    GLuint grid_vbo;
	GLuint Cube_vbo2;
	GLuint cylinder_vbo3;
	GLuint icosahedron_vbo4;
    
    GLuint position_location;
    GLuint color_location;
    GLuint MVP_location;

    // specify the shaders we want to use
    glUseProgram(program);

    // get the input variable location in this shader
    position_location = glGetAttribLocation(program, "in_position");
    color_location = glGetAttribLocation(program, "in_color");
	MVP_location = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVP_location,1,GL_FALSE,&MVP[0][0]);
        
    // create and bind a VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
    glEnableVertexAttribArray(color_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &grid_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);

    // initialize the vertex buffer with the vertex data
    
    glBufferData(GL_ARRAY_BUFFER, grid_vertices.size() * sizeof(Vertex), &grid_vertices[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // draw points
    glPointSize(10);
    glDrawArrays(GL_LINES, 0, grid_vertices.size());

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &grid_vbo);
    glDeleteVertexArrays(1, &vao);

///////////////////////////////////////////////Cube//////////////////////////////////////////////////////////
	if (botton1_pressed == true){
	
	MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
	MVP_location = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVP_location,1,GL_FALSE,&MVP_cube[0][0]);

	  // create and bind a VAO
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
    glEnableVertexAttribArray(color_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &Cube_vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, Cube_vbo2);

    // initialize the vertex buffer with the vertex data
    
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vertex), &Cube[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // draw points
    glPointSize(10);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &Cube_vbo2);
	glDeleteVertexArrays(1, &vao2);
	}
  
	///////////////////////////////////////////////cylinder//////////////////////////////////////////////////////////
	if (botton2_pressed == true){
	
    MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
	MVP_location = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVP_location,1,GL_FALSE,&MVP_cube[0][0]);

	  // create and bind a VAO
	
    glGenVertexArrays(1, &vao3);
    glBindVertexArray(vao3);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
    glEnableVertexAttribArray(color_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &cylinder_vbo3);
    glBindBuffer(GL_ARRAY_BUFFER, cylinder_vbo3);

    // initialize the vertex buffer with the vertex data
    
	glBufferData(GL_ARRAY_BUFFER, cylinder_vertices.size() * sizeof(Vertex), &cylinder_vertices[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // draw points
    glPointSize(10);
    glDrawArrays(GL_TRIANGLES, 0, cylinder_vertices.size());

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &cylinder_vbo3);
	glDeleteVertexArrays(1, &vao3);
	}	

////////////////////////////////////icosahedron//////////////////////////////////////////////////
	if (botton3_pressed == true){
	
    MVP_cube = ProjectionMatrix * ViewMatrix * ModleMatrix;
	MVP_location = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(MVP_location,1,GL_FALSE,&MVP_cube[0][0]);

	  // create and bind a VAO
    glGenVertexArrays(1, &vao4);
    glBindVertexArray(vao4);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
    glEnableVertexAttribArray(color_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &icosahedron_vbo4);
    glBindBuffer(GL_ARRAY_BUFFER, icosahedron_vbo4);

    // initialize the vertex buffer with the vertex data
    
    glBufferData(GL_ARRAY_BUFFER, icosahedron_vertices.size() * sizeof(Vertex), &icosahedron_vertices[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // draw points
    glPointSize(10);
    glDrawArrays(GL_TRIANGLES, 0, icosahedron_vertices.size());

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &icosahedron_vbo4);
	glDeleteVertexArrays(1, &vao4);
	}
}


void display(){
    // Clear Viewport
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_grid();

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
    read_shader_source_code("vs.glsl", buf, 1024);
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
