#pragma once
//#include "maths/vec4.h"
#include "graphics/shader/computeshader.h"
#include "graphics/shader/graphicsshader.h"
#include "graphics/shader/computebuffer.h"
#include <vector>
//using namespace sparky::maths;
using namespace sparky::graphics;
using namespace std;

 
namespace sparky {
	namespace render {
	

		class Renderable;
		class FluidRenderer
		{
		public:

			struct FluidParam
			{
				FluidParam():inputPos(0.5f,0.1f,0.5f,0.0f), smokeColor(0,0,0,1)
				{
					width = 128;
					height = 128;
					depth = 128;
					size = glm::vec4(width, height, depth, 0.0f);
					iterations = 10;
					vorticityStrength = 1.0f;
					densityAmount = 1.0f;
					densityDissipation = 0.999f;
					densityBuoyancy = 1.0f;
					densityWeight = 0.0125f;
					temperatureAmount = 10.0f;
					temperatureDissipation = 0.995f;
					velocityDissipation = 0.995f;
					inputRadius = 0.04f;
				
					smokeAbsorption = 60;
					ambientTemperature = 0.0f;
					smokeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				}

				int width;
				int height;
				int depth;

				int iterations;
				float vorticityStrength;
				float densityAmount;
				float densityDissipation;
				float densityBuoyancy;
				float densityWeight;
				float temperatureAmount;
				float temperatureDissipation;
				float velocityDissipation;
				float inputRadius;
				glm::vec4 inputPos;
				glm::vec4 size;
				float  ambientTemperature;

				glm::vec4 smokeColor;
				float smokeAbsorption;

			};
			enum ADVECTION
			{
				NORMAL = 1,
				BFECC,
				MACCORMACK
			};
		
		private:
			const int READ = 0;
			const int WRITE = 1;
			const int PHI_N_HAT = 0;
			const int PHI_N_1_HAT = 1;


			const int NUM_THREADS = 8;
			const float TIME_STEP = 0.1;

			
			ADVECTION m_advectionType;
			int m_width;
			int m_height;
			int m_depth;

			int m_iterations = 10;
			float m_vorticityStrength;
			float m_densityAmount;
			float m_densityDissipation;
			float m_densityBuoyancy;
			float m_densityWeight;
			float m_temperatureAmount;
			float m_temperatureDissipation;
			float m_velocityDissipation;
			float m_inputRadius;
			glm::vec4 m_inputPos;
			glm::vec4 m_size;
			float m_ambientTemperature;
			glm::vec4 smokeColor;
			float smokeAbsorption;

			float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
			float lastFrame = 0.0f; // ��һ֡��ʱ��
			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraFront = glm::vec3(0, 0, 0);
			glm::vec3 cameraUp = glm::vec3(0, 1, 0);


			GraphicsShader *m_SmokeRaycastShader;
			GraphicsShader *m_fragmentShader;
			ComputeShader *m_applyImpulse, *m_applyAdvect, *m_computeVorticity, *m_applyAdvectVelocity;
			ComputeShader *m_computeDivergence, *m_computeJacobi, *m_computeProjection;
			ComputeShader *m_computeConfinement, *m_computeObstacles, *m_applyBuoyancy;

			ComputeBuffer *m_density[2], *m_velocity[2], *m_pressure[2], *m_temperature[2], *m_phi[2];
			ComputeBuffer *m_temp3f, *m_obstacles;

			vector<int> aa;
		public:	
			FluidRenderer(FluidParam& param);
			FluidRenderer();
			FluidRenderer(const FluidRenderer& fr);
			void Initialize();
			void Update();
			void ComputeProjection();
			void ComputePressure();
			void ComputeDivergence();
			void ComputeVorticityConfinement(float dt);
			void ApplyAdvectionVelocity(float dt);
			void ApplyAdvectionMacCormack(float dt, float dissipation, float decay, ComputeBuffer* buffer[2]);
			void ApplyAdvectionBFECC(float dt, float dissipation, float decay, ComputeBuffer* buffer[2]);
			void ApplyAdvection(float dt, float dissipation, float decay, ComputeBuffer& read, ComputeBuffer& write, float forward = 1.0f);
			void ApplyAdvection(float dt, float dissipation, float decay, ComputeBuffer *buffer[2], float forward = 1.0f);
			void ApplyBuoyancy(float dt);
			void ApplyImpulse(float dt, float amount, ComputeBuffer* buffer[2]);
			void ComputeObstacles();

			void RenderScene();
			void Swap(ComputeBuffer* buffer[2]);

			void AddSmokeVolume(Renderable* r);

		private:
			vector<Renderable*> SmokeVolumes;
		};
	}
}
