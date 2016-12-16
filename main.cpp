#include "globals.h"

int main() {

	Application *app = new Application();

	Window *win = app->addWindow(640, 480, "Test application");

	//Object* obj = app->addObject("/Users/guestaccount/3D/Philippe_walter/AurigeDelphesLowRes.ply");
    Object* obj = app->addObject("/Users/guestaccount/suzanne.ply");
    Material* mat = app->addMaterial(MAT_TEXTURED);
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
            //w->mGUI->text("toto", 100, 100, 1.0f, glm::vec3(1,1,0));
			glfwSwapBuffers(w->mWindow);
		}
		
		glfwWaitEvents();
	};

	glfwTerminate();
}