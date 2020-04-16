#include "vec3.h"

namespace sparky {
	namespace maths {

		vec3::vec3()
		{

		}

		vec3::vec3(const float& x, const float& y, const float& z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3& vec3::add(const vec3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		vec3& vec3::subtract(const vec3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		vec3& vec3::multipy(const vec3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		vec3& vec3::divide(const vec3& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		vec3 vec3::Cross(const vec3& other) const
		{
			return vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		vec3 vec3::Normalize()const
		{
			float length = sqrt(x * x + y * y + z * z);
			return vec3(x / length, y / length, z / length);
		}

		std::ostream& operator<<(std::ostream& stream, const vec3& vector)
		{
			stream << "vec3: (" << vector.x << "," << vector.y <<","<<vector.z<< ")";
			return stream;
		}

		vec3 operator-(vec3 left, const vec3& right)
		{
			return left.subtract(right);
		}
	}
}