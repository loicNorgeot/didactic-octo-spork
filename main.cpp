#include "globals.h"

int main() {

	Application *app = new Application();

	Window *win = app->addWindow(640, 480, "Test application");

	Object* obj = app->addObject("/Users/guestaccount/3D/Philippe_walter/AurigeDelphesLowRes.ply");
	Material* mat = app->addMaterial(MAT_VERTEX_COLOR);
	Texture* tex = app->addTexture("/Users/guestaccount/texture.png");
	mat->addTexture(tex);
	obj->addMaterial(mat);

	Scene* sce = app->addScene();
	Camera* cam = app->addCamera();
	sce->addCamera(cam);
	sce->addObject(obj);
	win->addScene(sce);

	FPS(&chrono);
	
	while(!app->shouldClose()){
		
		FPS(&chrono, &frames);
		
		for(auto &w : app->windows){
			glfwMakeContextCurrent(w->mWindow);
			w->mScene->mCamera->updateProj();
			w->mScene->clear(glm::vec3(0.05,0.05,0.05));
			w->mScene->render();
			glfwSwapBuffers(w->mWindow);
			glFlush();
		}
		
		glfwWaitEvents();
	};

	glfwTerminate();
}