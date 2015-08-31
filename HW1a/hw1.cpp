// Homework #1 for Computer Graphic, UFL
// 
// Authoer: Ruijin Wu <ruijin@cise.ufl.edu>

#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#ifdef __APPLE__
#include <GLEW/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include "utils.h"

// CONTSTANTS
const int window_width = 500, window_height = 500;
const char * window_title = "HW0";

const int ncpoints =  8;

struct Vertex
{
    float pos[4];
	float color[4];
};


// runtime variable
char buf[1024];
Vertex cpoints[ncpoints];

GLuint vertex_shader, fragment_shader, program;

// function Prototype 
void graphics_init();
void draw_objects();

void initialize_points(){
	// spread points on the screen
	for(int i = 0; i < ncpoints; i++){
		cpoints[i].pos[0] = (float)0.8*sin((M_PI/4)*i);
	    cpoints[i].pos[1] = (float)0.8*cos((M_PI/4)*i);
		cpoints[i].pos[2] = 0;
		cpoints[i].pos[3] = (float)1.0;
		cpoints[i].color[0] = (float)frand();
	    cpoints[i].color[1] = (float)i/ncpoints*0.8 + 0.2;
		cpoints[i].color[2] = (float)frand();
		

	}
	
}

// return the index of the picked point
int color_code_pick(int x,int y){
	 
	float temp = 0;
  
    // draw the scene
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < ncpoints; i++){
	    cpoints[i].color[0] = (float)frand();;
	    cpoints[i].color[1] = (float)i/ncpoints*0.8 + 0.2;;
        cpoints[i].color[2] = (float)frand();
	}

	draw_objects();
	glFlush();

    // read back the green channel of pixel under the cursor into data
    // data is in range [0,255]
	GLubyte data;
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels(x,viewport[3]-y,1,1,GL_GREEN,GL_UNSIGNED_BYTE,&data);
    printf("color: %d\n", data);

    // TODO: identify the selected point and return its index
	if(data > 0)
	{  
		temp = (float)data/255;
		temp = (temp-0.2)*10;

        if ((int)temp+1-temp > temp-(int)temp)
            return (int)temp;
		else 
			return (int)temp+1;
	}
    
	return -1;
}
// MOUSE handling *******************************************

int last_x, last_y;
int selected_idx = -1;

void mouse(int button, int state, int x, int y ){
	if(state == GLUT_DOWN){
		selected_idx = color_code_pick(x,y);
		printf("pick point #%d\n", selected_idx);
	}else{
        
	}

    last_x = x;
    last_y = y;
    
}


void motion( int x, int y){
    GLint viewport[4];
    float dx,dy;
    
	glGetIntegerv(GL_VIEWPORT,viewport);
    
    dx = (x - last_x)/(float)viewport[2] * 2;
    dy = -(y - last_y)/(float)viewport[3] * 2;

    // TODO: move the picked point
    //
    // (dy, dy) is the mouse movement transferred
    // back from screen space to object space 
    cpoints[selected_idx].pos[0] += dx;
	cpoints[selected_idx].pos[1] += dy;

    last_x = x;
    last_y = y;

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
    }
}

// DISPLAY and RENDERING functions *************************


void draw_objects(){
    
    GLuint vao;
    GLuint points_vbo;
	GLuint color_location;
    
    GLuint position_location;
	

    // specify the shaders we want to use
    glUseProgram(program);

    // get the input variable location in this shader
    position_location = glGetAttribLocation(program, "in_position");
	color_location = glGetAttribLocation(program, "in_color");

    // create and bind a VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // enable the input locations we wan to use
    glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(color_location);

    // draw points

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

    // initialize the vertex buffer with the vertex data
    glBufferData(GL_ARRAY_BUFFER, ncpoints * sizeof(Vertex), &cpoints[0] , GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

    // draw points
    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, ncpoints);

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &points_vbo);
    glDeleteVertexArrays(1, &vao);
  
}

void display(){
    // Clear Viewport
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 for(int i = 0; i < ncpoints; i++){
	    cpoints[i].color[0] = 0;
	    cpoints[i].color[1] = 0;
        cpoints[i].color[2] = 1.0;
	}

    draw_objects();

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
    cout << "compiling vertex shader" << endl;
    read_shader_source_code("vs.glsl", buf, 1024);
    int vertex_shader_source_length = strlen(buf);
    const char *vertex_shader_sources[] = { buf };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_sources, &vertex_shader_source_length);
    glCompileShader(vertex_shader);
    check_shader_compile_error(vertex_shader);
    
    // init fragment shader 
    cout << "compiling fragment shader" << endl;
    read_shader_source_code("fs.glsl", buf, 1024); 
    int fragment_shader_source_length = strlen(buf);
    const char *fragment_shader_sources[] = { buf };
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_sources, &fragment_shader_source_length);
    glCompileShader(fragment_shader);
    check_shader_compile_error(fragment_shader);

    // init program
    cout << "linking program" << endl;
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    check_program_linking_error(program);
  
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
  
    // Initialize OpenGL
    graphics_init ();

    initialize_points();

    // set up callbacks
    glutReshapeFunc(&reshape);
    glutDisplayFunc(&display);
    glutKeyboardFunc(&keyboard);
    glutMouseFunc(&mouse);
    glutMotionFunc(&motion);

    // main loop
    glutMainLoop();
    return 0;
}
