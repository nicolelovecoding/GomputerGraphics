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

// CONTSTANTS
const int window_width = 500, window_height = 500;
const char * window_title = "HW1b";

const int ncpoints =  8;

struct Vertex
{
    float pos[4];
	float color[4];
};

typedef vector<Vertex> V_Vertex;

// runtime variable
char buf[1024];
Vertex cpoints[ncpoints];
Vertex cpoints_copy[ncpoints];
Vertex cpoints2[ncpoints+1];
Vertex cpoints2_copy[ncpoints+1];
Vertex cpoints4[4*ncpoints];
Vertex cpoints4_copy[4*ncpoints];
Vertex C[5];
V_Vertex V_points;
V_Vertex V_points_copy;
V_Vertex V_initial;
V_Vertex V_old;
V_Vertex V_Bezier;
V_Vertex V_Bezier_copy;
Vertex cpoints3;
Vertex MovePoint[1];
Vertex Tangent[2];
Vertex Binormal[2];
Vertex Normal[2];
bool press_1 = false;
bool press_2 = false;
bool press_3 = false;
bool press_4 = false;

GLuint vertex_shader, fragment_shader, program;

// function Prototype 
void graphics_init();
void draw_objects();
float GetX(Vertex[],int,int,float);
float GetY(Vertex[],int,int,float);
float GetZ(Vertex[],int,int,float);
void Create(GLuint,GLuint,GLuint,GLuint);
void Delete(GLuint,GLuint);
void initialize_otherpoints();
void DrawBezier();
void SetPoint(int);
void Generate_ControllPoints();
void draw_object(Vertex *,GLenum,int,GLfloat);
void draw_Vobject(V_Vertex,GLenum,int,GLfloat);
void timer(int);
void CopyPicture();

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

		cpoints2[i].pos[0] = cpoints[i].pos[0];
		cpoints2[i].pos[1] = cpoints[i].pos[1];
		cpoints2[i].pos[2] = cpoints[i].pos[2];
		cpoints2[i].pos[3] = cpoints[i].pos[3];
 
		cpoints2[i].color[0] = 1.0; 
		cpoints2[i].color[1] = 1.0; 
		cpoints2[i].color[2] = 1.0;
	}
	cpoints2[ncpoints].pos[0] = cpoints2[0].pos[0];
	cpoints2[ncpoints].pos[1] = cpoints2[0].pos[1]; 
	cpoints2[ncpoints].pos[2] = cpoints2[0].pos[2];
	cpoints2[ncpoints].pos[3] = cpoints2[0].pos[3];

	cpoints2[ncpoints].color[0] = 1.0; 
	cpoints2[ncpoints].color[1] = 1.0;
	cpoints2[ncpoints].color[2] = 1.0;
}

void Generate_ControllPoints(){

	for (int i = 0; i < 8; i++){
         
		cpoints4[i*4].pos[0] = (float)(cpoints2[(i-2+ncpoints)%ncpoints].pos[0] + 11*cpoints2[(i-1+ncpoints)%ncpoints].pos[0]
		+ 11*cpoints2[(i+ncpoints)%ncpoints].pos[0] + cpoints2[(i+1+ncpoints)%ncpoints].pos[0])/24;
		cpoints4[i*4].pos[1] = (float)(cpoints2[(i-2+ncpoints)%ncpoints].pos[1] + 11*cpoints2[(i-1+ncpoints)%ncpoints].pos[1]
		+ 11*cpoints2[(i+ncpoints)%ncpoints].pos[1] + cpoints2[(i+1+ncpoints)%ncpoints].pos[1])/24;
		cpoints4[i*4].pos[2] = (float)(cpoints2[(i-2+ncpoints)%ncpoints].pos[2] + 11*cpoints2[(i-1+ncpoints)%ncpoints].pos[2]
		+ 11*cpoints2[(i+ncpoints)%ncpoints].pos[2] + cpoints2[(i+1+ncpoints)%ncpoints].pos[2])/24;
		
		cpoints4[i*4+1].pos[0] = (float)(8*cpoints2[(i-1+ncpoints)%ncpoints].pos[0] + 14*cpoints2[(i+ncpoints)%ncpoints].pos[0]
		+ 2*cpoints2[(i+1+ncpoints)%ncpoints].pos[0])/24;
		cpoints4[i*4+1].pos[1] = (float)(8*cpoints2[(i-1+ncpoints)%ncpoints].pos[1] + 14*cpoints2[(i+ncpoints)%ncpoints].pos[1]
		+ 2*cpoints2[(i+1+ncpoints)%ncpoints].pos[1])/24;
		cpoints4[i*4+1].pos[2] = (float)(8*cpoints2[(i-1+ncpoints)%ncpoints].pos[2] + 14*cpoints2[(i+ncpoints)%ncpoints].pos[2]
		+ 2*cpoints2[(i+1+ncpoints)%ncpoints].pos[2])/24;
	
		cpoints4[i*4+2].pos[0] = (float)(4*cpoints2[(i-1+ncpoints)%ncpoints].pos[0] + 16*cpoints2[(i+ncpoints)%ncpoints].pos[0]
		+ 4*cpoints2[(i+1+ncpoints)%ncpoints].pos[0])/24;
		cpoints4[i*4+2].pos[1] = (float)(4*cpoints2[(i-1+ncpoints)%ncpoints].pos[1] + 16*cpoints2[(i+ncpoints)%ncpoints].pos[1]
		+ 4*cpoints2[(i+1+ncpoints)%ncpoints].pos[1])/24;
		cpoints4[i*4+2].pos[2] = (float)(4*cpoints2[(i-1+ncpoints)%ncpoints].pos[2] + 16*cpoints2[(i+ncpoints)%ncpoints].pos[2]
		+ 4*cpoints2[(i+1+ncpoints)%ncpoints].pos[2])/24;

		cpoints4[i*4+3].pos[0] = (float)(2*cpoints2[(i-1+ncpoints)%ncpoints].pos[0] + 14*cpoints2[(i+ncpoints)%ncpoints].pos[0]
		+ 8*cpoints2[(i+1+ncpoints)%ncpoints].pos[0])/24;
		cpoints4[i*4+3].pos[1] = (float)(2*cpoints2[(i-1+ncpoints)%ncpoints].pos[1] + 14*cpoints2[(i+ncpoints)%ncpoints].pos[1]
		+ 8*cpoints2[(i+1+ncpoints)%ncpoints].pos[1])/24;
		cpoints4[i*4+3].pos[2] = (float)(2*cpoints2[(i-1+ncpoints)%ncpoints].pos[2] + 14*cpoints2[(i+ncpoints)%ncpoints].pos[2]
		+ 8*cpoints2[(i+1+ncpoints)%ncpoints].pos[2])/24;
		
		for (int j = 0; j < 4; j++){

		    cpoints4[i*4+j].pos[3] = 1;

		    cpoints4[i*4+j].color[0] = 1;
		    cpoints4[i*4+j].color[1] = 0;
		    cpoints4[i*4+j].color[2] = 0;
		}
	}
}

void SetPoint(int target){

	cpoints2[target].pos[0] = cpoints[target].pos[0];
	cpoints2[target].pos[1] = cpoints[target].pos[1];
	cpoints2[target].pos[2] = cpoints[target].pos[2];

	if(target == 0){

		cpoints2[ncpoints].pos[0] = cpoints2[0].pos[0];
	    cpoints2[ncpoints].pos[1] = cpoints2[0].pos[1]; 
	    cpoints2[ncpoints].pos[2] = cpoints2[0].pos[2];
	}
}

float GetX(Vertex C[],int k,int i,float t)
{
	if (k == 0){
		return C[i].pos[0];
	}

	return GetX(C,k-1,i,t)*(1-t) + GetX(C,k-1,i+1,t)*t;
}

float GetY(Vertex C[],int k,int i,float t)
{

	if (k == 0){
		return C[i].pos[1];
	}

	return GetY(C,k-1,i,t)*(1-t) + GetY(C,k-1,i+1,t)*t;
}

float GetZ(Vertex C[],int k,int i,float t)
{
	if (k == 0){
		return C[i].pos[2];
	}
	return GetZ(C,k-1,i,t)*(1-t) + GetZ(C,k-1,i+1,t)*t;
}
// return the index of the picked point
int color_code_pick(int x,int y){
	 
	float temp = 0;
  
    // draw the scene
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < ncpoints; i++){
	    cpoints[i].color[0] = (float)frand();
	    cpoints[i].color[1] = (float)i/ncpoints*0.8 + 0.2;
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
int n = 0;
	    
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
    float dz = 0;
	glGetIntegerv(GL_VIEWPORT,viewport);
    
    
    dx = (x - last_x)/(float)viewport[2] * 2;
    dy = -(y - last_y)/(float)viewport[3] * 2;

	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		dz = dx;

    // TODO: move the picked point
    //
    // (dy, dy) is the mouse movement transferred
    // back from screen space to object space 
    cpoints[selected_idx].pos[0] += dx;
	cpoints[selected_idx].pos[1] += dy;
	cpoints[selected_idx].pos[2] += dz;

	SetPoint(selected_idx);
	DrawBezier();
    last_x = x;
    last_y = y;

    glutPostRedisplay();
    
}

void DrawBezier(){

	Generate_ControllPoints();
	V_Bezier.clear();
	for (int i = 0; i < 8; i++){

	     for (int m = 0;m < 5;m++){

			  C[m] = cpoints4[i*4+m];
			  if(i == 7)
				  C[4] = cpoints4[0];
		 }

		 int n = 50;
         Vertex temp;
		 temp.pos[3] = 1;
		 temp.color[0] = 1;
		 temp.color[1] = 1;
		 temp.color[2] = 0;
		 for (int a = 0;a < n;a++){
			 temp.pos[0] = GetX(C,4,0,(float)a/n);
			 temp.pos[1] = GetY(C,4,0,(float)a/n);
			 temp.pos[2] = GetZ(C,4,0,(float)a/n);
			 V_Bezier.push_back(temp);
		 }
	 }
	 V_Bezier.push_back(V_Bezier[0]);
}
// KEYBOARD handling *******************************************
int counter = 0;
float Scale_mat1[16] = {0.5,0.0,0.0,0.0,
		                0.0,0.5,0.0,0.0,
					    0.0,0.0,0.5,0.0,
						0.0,0.0,0.0,1.0};

float Rotate_mat1[16] = {0.0,0.0,-1.0,0.0,
		                 0.0,1.0,0.0,0.0,
						 1.0,0.0,0.0,0.0,
						 0.0,0.0,0.0,1.0};
float Scale_mat2[16] = {1.0,0.0,0.0,0.0,
		                0.0,1.0,0.0,0.0,
						0.0,0.0,1.0,0.0,
						0.0,0.0,0.0,1.0};
float Rotate_mat2[16] = {1.0,0.0,0.0,0.0,
		                 0.0,1.0,0.0,0.0,
						 0.0,0.0,1.0,0.0,
						 0.0,0.0,0.0,1.0};
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
	case 49:
		press_1 = true;
		if (counter < 5){
			if (counter == 0){
				V_initial.clear();V_points.clear();
			    for (int i = 0; i < ncpoints+1; i++){
		          //  V_initial.push_back(V_points[i]);
					V_initial.push_back(cpoints2[i]);
					V_points.push_back(cpoints2[i]);
				}

		    }

			for (int i = 0; i < V_points.size()-1; i++){

				V_old.push_back(V_points[i]);
			}
			n = V_old.size();
			V_points.clear();
			cpoints3.pos[0] = 0;
	        cpoints3.pos[1] = 0;
	        cpoints3.pos[2] = 0;
	        cpoints3.pos[3] = (float)1.0;
            cpoints3.color[0] = 1.0;
	        cpoints3.color[1] = 1.0;
	        cpoints3.color[2] = 1.0;
			for (int i = 0; i < n; i++){
				cpoints3.pos[0] =(float)(V_old[(i-2+n)%n].pos[0]+10*V_old[(i-1+n)%n].pos[0]+5*V_old[(i+n)%n].pos[0])/16;
				cpoints3.pos[1] =(float)(V_old[(i-2+n)%n].pos[1]+10*V_old[(i-1+n)%n].pos[1]+5*V_old[(i+n)%n].pos[1])/16;
				V_points.push_back(cpoints3);
                cpoints3.pos[0] =(float)(5*V_old[(i-1+n)%n].pos[0]+10*V_old[(i+n)%n].pos[0]+V_old[(i+1+n)%n].pos[0])/16;
				cpoints3.pos[1] =(float)(5*V_old[(i-1+n)%n].pos[1]+10*V_old[(i+n)%n].pos[1]+V_old[(i+1+n)%n].pos[1])/16;
				V_points.push_back(cpoints3);
			}
			V_points.push_back(V_points[0]);
			V_old.clear();
			counter++;
		}
		if (counter == 5){
			press_1 = false;
			counter = 0;
			V_points.clear();
			for (int i = 0; i < ncpoints; i++){

				V_points.push_back(V_initial[i]);
			}
			V_points.push_back(V_points[0]);
		}
		glutPostRedisplay();
		break;
		case 50:
		   press_2 = !press_2;
		   DrawBezier();
	       glutPostRedisplay();
		break;
		case 51:
			press_3 = !press_3;
        	glutPostRedisplay();
	    break;
		case 52:
		   press_4 = !press_4;
		   glutPostRedisplay();
		break;
    }
}

void test(){
	  GLuint transform_vec;
	        GLuint scale_matrix;
	        GLuint rotate_matrix;

            transform_vec = glGetUniformLocation(program,"Transform");
	        scale_matrix = glGetUniformLocation(program,"Scale");
	        rotate_matrix = glGetUniformLocation(program,"Rotate");

	        if(press_3 == true)
	        {
		        glUniformMatrix4fv(scale_matrix,1,GL_TRUE,&Scale_mat1[0]);
	            glUniform4f(transform_vec,0.0,0.5,0.0,0.0);
		        draw_objects();
		
	            glUniform4f(transform_vec,0.0,-0.5,0.0,0.0);
		        glUniformMatrix4fv(rotate_matrix,1,GL_TRUE,&Rotate_mat1[0]);
		        draw_objects();
				glUniformMatrix4fv(rotate_matrix,1,GL_TRUE,&Rotate_mat2[0]);
	         }
	         else
	         {
		        glUniformMatrix4fv(scale_matrix,1,GL_TRUE,&Scale_mat2[0]);
		        glUniform4f(transform_vec,0.0,0.0,0.0,0.0);
		        glUniformMatrix4fv(rotate_matrix,1,GL_TRUE,&Rotate_mat2[0]);
				draw_objects();
	         }
}
void timer(int value)
{
    // do the update you want, e.g. update the frame position
	int static i = 0;
	float x = 0,xx = 0,y = 0,yy = 0,z = 0, zz = 0;
    if (V_Bezier.size() > 0){

	   MovePoint[0].color[0] = 1;
       MovePoint[0].color[1] = 0;
       MovePoint[0].color[2] = 0;

	   Tangent[0].color[0] = 1;
	   Tangent[0].color[1] = 0;
	   Tangent[0].color[2] = 0;

	   Tangent[1].color[0] = 1;
	   Tangent[1].color[1] = 0;
	   Tangent[1].color[2] = 0;

	   MovePoint[0].pos[0] = V_Bezier[(i+V_Bezier.size())%V_Bezier.size()].pos[0];//GetX(C,4,0,t);
	   MovePoint[0].pos[1] = V_Bezier[(i+V_Bezier.size())%V_Bezier.size()].pos[1];//GetY(C,4,0,t);
       MovePoint[0].pos[2] = V_Bezier[(i+V_Bezier.size())%V_Bezier.size()].pos[2];//GetZ(C,4,0,t);
	   MovePoint[0].pos[3] = 1;
	   
	   if (i%51 < 50){

	       for (int m = 0;m < 5;m++){

		      C[m] = cpoints4[((i/50)%8)*4+m];
		      if((  (i/50) %8) == 7){
			      C[4] = cpoints4[0];
			  }
	       }
	   }

	   Tangent[0].pos[0] = GetX(C,4,0,(float)(i%50)/50);
	   Tangent[0].pos[1] = GetY(C,4,0,(float)(i%50)/50);
	   Tangent[0].pos[2] = GetZ(C,4,0,(float)(i%50)/50);
	   Tangent[0].pos[3] = 1;

	   x = GetX(C,3,1,(float)(i%50)/50) - Tangent[0].pos[0];//GetX(C,3,0,(float)(i%50)/50);
	   y = GetY(C,3,1,(float)(i%50)/50) - Tangent[0].pos[1];//GetY(C,3,0,(float)(i%50)/50);
	   z = GetZ(C,3,1,(float)(i%50)/50) - Tangent[0].pos[2];//GetZ(C,3,0,(float)(i%50)/50);

	   xx = x/sqrt(x*x + y*y + z*z);
	   yy = y/sqrt(x*x + y*y + z*z);
	   zz = z/sqrt(x*x + y*y + z*z);

	   Tangent[1].pos[0] = xx/10 + Tangent[0].pos[0];
	   Tangent[1].pos[1] = yy/10 + Tangent[0].pos[1];
	   Tangent[1].pos[2] = zz/10 + Tangent[0].pos[2];
	   Tangent[1].pos[3] = 1;
	    
	   x = (GetY(C,2,0,(float)(i%50)/50) - GetY(C,2,1,(float)(i%50)/50)) * (GetZ(C,2,2,(float)(i%50)/50) - GetZ(C,2,1,(float)(i%50)/50)) 
		   - (GetZ(C,2,0,(float)(i%50)/50) - GetZ(C,2,1,(float)(i%50)/50)) * (GetY(C,2,2,(float)(i%50)/50) - GetY(C,2,1,(float)(i%50)/50));

	   y = (GetZ(C,2,0,(float)(i%50)/50) - GetZ(C,2,1,(float)(i%50)/50)) * (GetX(C,2,2,(float)(i%50)/50) - GetX(C,2,1,(float)(i%50)/50)) 
		   - (GetX(C,2,0,(float)(i%50)/50) - GetX(C,2,1,(float)(i%50)/50)) * (GetZ(C,2,2,(float)(i%50)/50) - GetZ(C,2,1,(float)(i%50)/50));

	   z = (GetX(C,2,0,(float)(i%50)/50) - GetX(C,2,1,(float)(i%50)/50)) * (GetY(C,2,2,(float)(i%50)/50) - GetY(C,2,1,(float)(i%50)/50)) 
		   - (GetY(C,2,0,(float)(i%50)/50) - GetY(C,2,1,(float)(i%50)/50)) * (GetX(C,2,2,(float)(i%50)/50) - GetX(C,2,1,(float)(i%50)/50));

	   xx = x/sqrt(x*x + y*y + z*z);
	   yy = y/sqrt(x*x + y*y + z*z);
	   zz = z/sqrt(x*x + y*y + z*z);

	   Binormal[0].pos[0] =  MovePoint[0].pos[0];//GetX(C,4,0,(float)(i%50)/50);
	   Binormal[0].pos[1] =  MovePoint[0].pos[1];//GetY(C,4,0,(float)(i%50)/50);
	   Binormal[0].pos[2] =  MovePoint[0].pos[2];//GetZ(C,4,0,(float)(i%50)/50);
	   Binormal[0].pos[3] = 1;

	   Binormal[0].color[0] = 0;
	   Binormal[0].color[1] = 0;
	   Binormal[0].color[2] = 1;

	   Binormal[1].pos[0] = Binormal[0].pos[0] + xx/5;//binormal.pos[0];
	   Binormal[1].pos[1] = Binormal[0].pos[1] + yy/5;//binormal.pos[1];
	   Binormal[1].pos[2] = Binormal[0].pos[2] + zz/5;//binormal.pos[2];
	   Binormal[1].pos[3] = 1;

	   Binormal[1].color[0] = 0;
	   Binormal[1].color[1] = 0;
	   Binormal[1].color[2] = 1;

	   Normal[0].pos[0] = MovePoint[0].pos[0];
	   Normal[0].pos[1] = MovePoint[0].pos[1];
	   Normal[0].pos[2] = MovePoint[0].pos[2];
	   Normal[0].pos[3] = 1;

	   Normal[0].color[0] = 0;
	   Normal[0].color[1] = 1;
	   Normal[0].color[2] = 0;

	   x = (Binormal[1].pos[1] - Binormal[0].pos[1]) * (Tangent[1].pos[2] - Tangent[0].pos[2])
		  - (Binormal[1].pos[2] - Binormal[0].pos[2]) * (Tangent[1].pos[1] - Tangent[0].pos[1]);

	   y = (Binormal[1].pos[2] - Binormal[0].pos[2]) * (Tangent[1].pos[0] - Tangent[0].pos[0])
		  - (Binormal[1].pos[0] - Binormal[0].pos[0]) * (Tangent[1].pos[2] - Tangent[0].pos[2]);

	   z = (Binormal[1].pos[0] - Binormal[0].pos[0]) * (Tangent[1].pos[1] - Tangent[0].pos[1])
		  - (Binormal[1].pos[1] - Binormal[0].pos[1]) * (Tangent[1].pos[0] - Tangent[0].pos[0]);

	   xx = x/sqrt(x*x + y*y + z*z);
	   yy = y/sqrt(x*x + y*y + z*z);
	   zz = z/sqrt(x*x + y*y + z*z);

	   Normal[1].pos[0] = xx/10 + MovePoint[0].pos[0];
	   Normal[1].pos[1] = yy/10 + MovePoint[0].pos[1];
	   Normal[1].pos[2] = zz/10 + MovePoint[0].pos[2];
	   Normal[1].pos[3] = 1;

	   Normal[1].color[0] = 0;
	   Normal[1].color[1] = 1;
	   Normal[1].color[2] = 0;

	   i++;
    }
    // reset timer to be trigger every 60 ms
    glutTimerFunc(60, &timer, 0);

    // redraw the screen
    glutPostRedisplay();
}
	
	
// DISPLAY and RENDERING functions *************************

void draw_object(Vertex * obj, GLenum mode,int num_of_points,GLfloat points_size){
   
    GLuint vao = 0;
    GLuint points_vbo = 0;

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

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

    // initialize the vertex buffer with the vertex data
    glBufferData(GL_ARRAY_BUFFER, num_of_points * sizeof(Vertex),&obj[0],GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

    // draw points
    glPointSize(points_size);
    glDrawArrays(mode,0,num_of_points);

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &points_vbo);
	glDeleteVertexArrays(1, &vao);
}

void draw_Vobject(V_Vertex obj, GLenum mode,int num_of_points,GLfloat points_size){
   
    GLuint vao = 0;
    GLuint points_vbo = 0;

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

    // generate and bind a vertex buffer to hold the vertex data on GPU
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

    // initialize the vertex buffer with the vertex data
    glBufferData(GL_ARRAY_BUFFER, num_of_points * sizeof(Vertex),&obj[0],GL_STATIC_DRAW);

    // specify the input data format
    glVertexAttribPointer(position_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

    // draw points
    glPointSize(points_size);
    glDrawArrays(mode,0,num_of_points);

    // unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Delete VAO and VBO
    glDeleteBuffers(1, &points_vbo);
	glDeleteVertexArrays(1, &vao);
}

void draw_objects(){

	draw_object(cpoints,GL_POINTS,ncpoints,10);
	draw_object(cpoints2,GL_LINE_STRIP,ncpoints+1,10);

	if (press_1 == true)
	    draw_Vobject(V_points,GL_LINE_STRIP,V_points.size(),5);

	if(press_2 == true){

       draw_object(cpoints4,GL_POINTS,ncpoints*4,5);
       draw_Vobject(V_Bezier,GL_LINE_STRIP,V_Bezier.size(),5);
	}

	if (press_4 == true){
//		draw_object(MovePoint,GL_POINTS,1,5);
		draw_object(Tangent,GL_LINE_STRIP,2,5);
		draw_object(Binormal,GL_LINE_STRIP,2,5);
		draw_object(Normal,GL_LINE_STRIP,2,5);
	}
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
	test();
//	draw_objects();
    glutSwapBuffers();
}

void reshape(int width, int height){
    // Clip the view port to match our ratio
    glViewport(0,0, width, height);
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
	glutTimerFunc(60, &timer, 0);
    // main loop
    glutMainLoop();
    return 0;
}


