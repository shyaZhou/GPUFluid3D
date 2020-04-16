#version 450 core
layout(binding = 0) buffer _Density
{
	float data[];
};


uniform vec4 SmokeColor;

uniform float SmokeAbsorption;

uniform vec3 Size;
uniform vec3 WorldSpaceCameraPos;
uniform vec3 Translate;
uniform vec3 Scale;

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 worldPos;




#define NUM_SAMPLES 64



struct Ray
{
	vec3 origin;
	vec3 dir;

};

struct AABB {
	vec3 Min;
	vec3 Max;
};

//find intersection points of a ray with a box
bool intersectBox(Ray r, AABB aabb, out float t0, out float t1)
{
	vec3 invR = 1.0 / r.dir;
	vec3 tbot = invR * (aabb.Min - r.origin);
	vec3 ttop = invR * (aabb.Max - r.origin);
	vec3 tmin = min(ttop, tbot);
	vec3 tmax = max(ttop, tbot);
	vec2 t = max(tmin.xx, tmin.yz);
	t0 = max(t.x, t.y);
	t = min(tmax.xx, tmax.yz);
	t1 = min(t.x, t.y);
	return t0 <= t1;
}




float SampleBilinear(vec3 uv, vec3 size)
{
	uv = clamp(uv, 0, 1);
	uv = uv * (size - 1.0);

	int x = int(uv.x);
	int y = int(uv.y);
	int z = int(uv.z);

	int X = int(size.x);
	int XY = int(size.x*size.y);

	float fx = uv.x - x;
	float fy = uv.y - y;
	float fz = uv.z - z;

	int xp1 = int(min(size.x - 1, x + 1));
	int yp1 = int(min(size.y - 1, y + 1));
	int zp1 = int(min(size.z - 1, z + 1));

	float x0 = data[x + y * X + z * XY] * (1.0f - fx) + data[xp1 + y * X + z * XY] * fx;
	float x1 = data[x + y * X + zp1 * XY] * (1.0f - fx) + data[xp1 + y * X + zp1 * XY] * fx;

	float x2 = data[x + yp1 * X + z * XY] * (1.0f - fx) + data[xp1 + yp1 * X + z * XY] * fx;
	float x3 = data[x + yp1 * X + zp1 * XY] * (1.0f - fx) + data[xp1 + yp1 * X + zp1 * XY] * fx;

	float z0 = x0 * (1.0f - fz) + x1 * fz;
	float z1 = x2 * (1.0f - fz) + x3 * fz;

	return z0 * (1.0f - fy) + z1 * fy;

}


void main()
{
	

	vec3 pos = WorldSpaceCameraPos;

	Ray r;
	r.origin = pos;
	r.dir = normalize(worldPos - pos);

	AABB aabb;
	aabb.Min = vec3(-0.5, -0.5, -0.5)*Scale + Translate;
	aabb.Max = vec3(0.5, 0.5, 0.5)*Scale + Translate;

	//figure out where ray from eye hit front of cube
	float tnear, tfar;
	intersectBox(r, aabb, tnear, tfar);

	//if eye is in cube then start ray at eye
	if (tnear < 0.0) tnear = 0.0;

	vec3 rayStart = r.origin + r.dir * tnear;
	vec3 rayStop = r.origin + r.dir * tfar;

	//convert to texture space
	rayStart -= Translate;
	rayStop -= Translate;
	rayStart = (rayStart + 0.5*Scale) / Scale;
	rayStop = (rayStop + 0.5*Scale) / Scale;

	vec3 start = rayStart;
	float dist = distance(rayStop, rayStart);
	float stepSize = dist / float(NUM_SAMPLES);
	vec3 ds = normalize(rayStop - rayStart) * stepSize;
	float alpha = 1.0;

	for (int i = 0; i < NUM_SAMPLES; i++, start += ds)
	{

		float D = SampleBilinear(start, Size);

		alpha *= 1.0 - clamp(D*stepSize*SmokeAbsorption, 0, 1);

		if (alpha <= 0.01) break;
	}

	
	color = SmokeColor * ( 1 - alpha );
	//color = vec4(1,0,0,1);

}























