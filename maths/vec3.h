#pragma once

#include <iostream>
namespace sparky {
	namespace maths {
		struct vec3
		{
			float x, y ,z;

			vec3();
			vec3(const float& x, const float& y, const float& z);

			vec3& add(const vec3& other);
			vec3& subtract(const vec3& other);
			vec3& multipy(const vec3& other);
			vec3& divide(const vec3& other);
			vec3 Cross(const vec3& other) const;
			vec3 Normalize()const;
			friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);


			friend vec3 operator-(vec3 left, const vec3& right);
		};
	}
}