#pragma once
#include "utils/fileutils.h"
#include "geometry/rawmesh.h"
namespace sparky {
	namespace geometry {
		class objLoader
		{
		public:
			static void LoadMesh(const char* file, RawMesh& rmesh);

		};
	}
}
