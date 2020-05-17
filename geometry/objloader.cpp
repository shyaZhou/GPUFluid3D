#include "objloader.h"
//using namespace sparky::maths;
namespace sparky {

	namespace geometry {
		void objLoader::LoadMesh(const char* file, RawMesh& rmesh)
		{
			rmesh.m_Position.push_back(glm::vec3(-0.5,-0.5,-0.5));
			rmesh.m_Position.push_back(glm::vec3(-0.5, -0.5, 0.5));
			rmesh.m_Position.push_back(glm::vec3(-0.5, 0.5, 0.5));
			rmesh.m_Position.push_back(glm::vec3(-0.5, 0.5, -0.5));
			rmesh.m_Position.push_back(glm::vec3(0.5, -0.5, -0.5));
			rmesh.m_Position.push_back(glm::vec3(0.5, -0.5, 0.5));
			rmesh.m_Position.push_back(glm::vec3(0.5, 0.5, 0.5));
			rmesh.m_Position.push_back(glm::vec3(0.5, 0.5, -0.5));

			rmesh.m_Faces.push_back(0);
			rmesh.m_Faces.push_back(1);
			rmesh.m_Faces.push_back(2);
			rmesh.m_Faces.push_back(0);
			rmesh.m_Faces.push_back(2);
			rmesh.m_Faces.push_back(3);

			rmesh.m_Faces.push_back(4);
			rmesh.m_Faces.push_back(6);
			rmesh.m_Faces.push_back(5);
			rmesh.m_Faces.push_back(4);
			rmesh.m_Faces.push_back(7);
			rmesh.m_Faces.push_back(6);

			rmesh.m_Faces.push_back(0);
			rmesh.m_Faces.push_back(4);
			rmesh.m_Faces.push_back(5);
			rmesh.m_Faces.push_back(0);
			rmesh.m_Faces.push_back(5);
			rmesh.m_Faces.push_back(1);

			rmesh.m_Faces.push_back(3);
			rmesh.m_Faces.push_back(6);
			rmesh.m_Faces.push_back(7);
			rmesh.m_Faces.push_back(3);
			rmesh.m_Faces.push_back(2);
			rmesh.m_Faces.push_back(6);


			rmesh.m_Faces.push_back(1);
			rmesh.m_Faces.push_back(5);
			rmesh.m_Faces.push_back(6);
			rmesh.m_Faces.push_back(6);
			rmesh.m_Faces.push_back(2);
			rmesh.m_Faces.push_back(1);


			rmesh.m_Faces.push_back(0);
			rmesh.m_Faces.push_back(7);
			rmesh.m_Faces.push_back(4);
			rmesh.m_Faces.push_back(0);
			rmesh.m_Faces.push_back(3);
			rmesh.m_Faces.push_back(7);



			//rmesh.m_Faces.push_back(Face(0, 1, 2));
			//rmesh.m_Faces.push_back(Face(0, 2, 3));
			//rmesh.m_Faces.push_back(Face(4, 5, 6));
			//rmesh.m_Faces.push_back(Face(4, 6, 7));

			//rmesh.m_Faces.push_back(Face(0, 4, 5));
			//rmesh.m_Faces.push_back(Face(0, 5, 1));
			//rmesh.m_Faces.push_back(Face(3, 7, 6));
			//rmesh.m_Faces.push_back(Face(3, 6, 2));

			//rmesh.m_Faces.push_back(Face(1, 5, 6));
			//rmesh.m_Faces.push_back(Face(6, 2, 1));
			//rmesh.m_Faces.push_back(Face(0, 4, 7));
			//rmesh.m_Faces.push_back(Face(0, 7, 3));

		}
	}
}