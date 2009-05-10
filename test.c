#include <GL/glfw.h>

int main( void )
{
    int running = GL_TRUE;
    
    // Initialize GLFW
    glfwInit();

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

