#include <GL/glfw.h>
#include <stdio.h>

#include "solid.h"
#include "list.h"

void draw_solid_list(List* list) {
  Node* node = list->first_node;
  Solid* item = NULL;

  while(NULL != node) {
    item = (Solid*) node->item;

    fprintf(stderr, "Position: %d,%d with size %dx%d\n", item->x, item->y, item->width, item->height);

    node = node->next;
  }
}

int main( void )
{
    int running = GL_TRUE;
    
    List* list = list_create();
    
    list_push(list, solid_create(1,1,5,5));
    list_push(list, solid_create(2,4,5,5));
    list_push(list, solid_create(3,5,6,6));
    
    draw_solid_list(list);

    // Initialize GLFW
    glfwInit();

    fprintf(stderr, "GLFW initialized...\n");

    // Open an OpenGL window
    if( !glfwOpenWindow( 300,300, 0,0,0,0,0,0, GLFW_WINDOW ) )
    {
        glfwTerminate();
        return 0;
    }

    // Main loop
    while( running )
    {
         // OpenGL rendering goes here...
         glClear( GL_COLOR_BUFFER_BIT );
         // Swap front and back rendering buffers
         glfwSwapBuffers();
         // Check if ESC key was pressed or window was closed
         running = !glfwGetKey( GLFW_KEY_ESC ) &&
                   glfwGetWindowParam( GLFW_OPENED );
    }
    
    // Close window and terminate GLFW
    glfwTerminate();
    
    // Exit program
    return 0;
}

