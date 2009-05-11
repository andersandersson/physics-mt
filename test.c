#include <GL/glfw.h>
#include <stdio.h>
#include <stdlib.h>

#include "solid.h"
#include "list.h"
#include "physics.h"


void draw_solid(Solid* solid) {
  float r = 0.8;
  float g = 0.1;
  float b = 0.2;
  glBegin(GL_QUADS);

  glColor3f(r*0.7, g*0.7, b*0.7);
  glVertex3f( solid->x, solid->y, 0.0f);
  glColor3f(r, g, b);
  glVertex3f( solid->x+solid->width, solid->y, 0.0f);
  glColor3f(r*0.2, g*0.9, b*0.2);
  glVertex3f( solid->x+solid->width, solid->y+solid->height, 0.0f);
  glColor3f(r*0.9, g*0.8, b*0.9);
  glVertex3f( solid->x, solid->y+solid->height, 0.0f);
  glEnd(); 
}

void draw_solid_list(List* list) {
  Node* node = list->first_node;
  Solid* item = NULL;

  while(NULL != node) {
    item = (Solid*) node->item;

    draw_solid(item);

    node = node->next;
  }
}

void setup_world(List* list, int number) {
  Solid* solid;
  int i;
  int x;
  int y;

  for(i=0; i<number; i++) {
    x = rand()%640-50;
    y = rand()%480-50;
    solid = solid_create(x, y, 50,50);
    
    x = (1-(rand()%2)*2)*(rand()%1+1);
    y = (1-(rand()%2)*2)*(rand()%1+1);
    solid_set_velocity(solid, x, y);

    list_push(list, solid);
    physics_add(solid);
  }
}

int main( int argc, char* argv[] )
{
    int running = GL_TRUE;
    int threads;
    int objects;
    double t, t2;
    double c;
    
    List* list = list_create();
    
    // Initialize GLFW
    glfwInit();

    fprintf(stderr, "GLFW initialized...\n");

    // Open an OpenGL window
    if( !glfwOpenWindow( 640,480, 0,0,0,0,0,0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        return 0;
    }
    

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();             
    glOrtho(0, 640, 480, 0, -1, 1);
    glMatrixMode( GL_MODELVIEW );         
    glLoadIdentity();                     
    glClearColor(0,0,0,1);                
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(argc == 3) {
      sscanf(argv[1], "%d", &threads);
      sscanf(argv[2], "%d", &objects);
      physics_init(threads);
      setup_world(list, objects);
    } else {
      physics_init(4);    
      setup_world(list, 1000);
    }

    
    // Main loop
    t2 = t = glfwGetTime();

    c = 0;
    while( running ) {
      c++;
      if(glfwGetTime() - t > 1) {
	fprintf(stderr, "FPS: %f\n", c / (glfwGetTime()-t2));
	t = glfwGetTime();
      }

      physics_tick();
      // OpenGL rendering goes here...
      glClear( GL_COLOR_BUFFER_BIT );
      draw_solid_list(list);

      // Swap front and back rendering buffers
      glfwSwapBuffers();
      
      // Check if ESC key was pressed or window was closed
      running = !glfwGetKey( GLFW_KEY_ESC ) &&
                   glfwGetWindowParam( GLFW_OPENED );      
    }
    
    physics_terminate();
    // Close window and terminate GLFW
    glfwTerminate();

    list_free(list);
    
    // Exit program
    return 0;
}

