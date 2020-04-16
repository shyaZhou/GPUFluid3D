#pragma once
#include"graphics/buffers/vertexarray.h"
#include "render/renderable.h"
#include "graphics/buffers/indexbuffer.h"
using namespace sparky::graphics;
namespace sparky
{
	namespace render {

		class Mesh:public Renderable
		{
		public:
			void render();
			static Mesh* Load(const char* file);
		private:
			VertexArray  vao;
		};
	}
}