#include "globals.h"

std::string rootDir = "/Users/guestaccount/dev/OpenGL2017/";
bool verbose = false;
int frames  = 0;
double chrono = 0;

void exit(std::string message){
    std::cout << message << std::endl;
    exit(1);
}
void FPS(double *t){
    (*t) = glfwGetTime();
}
void FPS(double *t, int *fr){
    if(glfwGetTime() - (*t) > 1.0){
        std::cout << "FPS = " << frames/(glfwGetTime() - (*t)) << std::endl;
        (*t) = glfwGetTime();
        frames = 0;
    }
    frames++;
}