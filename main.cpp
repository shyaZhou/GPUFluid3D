
#include <iostream>
#include "graphics/window.h"
//#include "maths/vec2.h"
#include "utils/fileutils.h"
#include "graphics/shader/graphicsshader.h"
#include "graphics/shader/computeshader.h"
#include "render/FluidRenderer.h"
#include "render/mesh.h"
int main()
{
	using namespace sparky;
	using namespace graphics;
	//using namespace maths;
 
	using namespace sparky::render;
	Window window("Sparky!", 800, 600);
	 

	/*std::string file = FileUtile::read_file("main.cpp");
	std::cout << file << std::endl;*/

	/*GraphicsShader gshader("shaders/basic.vert","shaders/basic.frag");
	ComputeShader cshader("shaders/test.comp");*/

	FluidRenderer::FluidParam param;
	FluidRenderer* m_render = new FluidRenderer(param);
	m_render->Initialize();
	Mesh* smokemesh = Mesh::Load("autoload");
	m_render->AddSmokeVolume(smokemesh);

	while (!window.Closed())
	{
		glClearColor(1, 1, 1, 1);
		window.clear();
		//std::cout << window.getHeight() << "," << window.getWidth()<<std::endl;
		if (window.isKeyPress(GLFW_KEY_W))
		{
			std::cout << "press w!";
		}

		if (window.isMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "mouse press!";
		}
		//glBegin(GL_TRIANGLES);
		//glVertex2f(-0.5, -0.5);
		//glVertex2f(0 , 1);
		//glVertex2f(0.5, -0.5);
		//glEnd();
		m_render->Update();
		m_render->RenderScene();
		window.update();
	}

	system("PAUSE");
	return 0;

}