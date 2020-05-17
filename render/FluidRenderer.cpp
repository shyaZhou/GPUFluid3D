#include "FluidRenderer.h"
#include "maths/util.h"
#include "render/mesh.h"
//#include "maths/mat4.h"
#include "GLFW/glfw3.h"
#include <iostream>

//#include ""
using namespace sparky::maths;
using namespace sparky::graphics;

#include "glm/gtc/matrix_transform.hpp"

namespace sparky
{
	namespace render
	{
		FluidRenderer::FluidRenderer(FluidParam& param)
		{
			m_width = param.width;
			m_height = param.height;
			m_depth = param.depth;
			m_iterations = param.iterations;
			m_vorticityStrength = param.vorticityStrength;
			m_densityAmount = param.densityAmount;
			m_densityDissipation = param.densityDissipation;
			m_densityBuoyancy = param.densityBuoyancy;
			m_densityWeight = param.densityWeight;
			m_temperatureAmount = param.temperatureAmount;
			m_temperatureDissipation = param.temperatureDissipation;
			m_velocityDissipation = param.velocityDissipation;
			m_inputRadius = param.inputRadius;


			m_inputPos = param.inputPos;

			m_ambientTemperature = param.ambientTemperature;
			m_size = param.size;

			smokeAbsorption = param.smokeAbsorption;
			smokeColor = param.smokeColor;
		}
		FluidRenderer::FluidRenderer()
		{
		}
		FluidRenderer::FluidRenderer(const FluidRenderer& fr)
		{
			m_width = fr.m_width;
			m_height = fr.m_height;
			m_depth = fr.m_depth;
			m_iterations = fr.m_iterations;
			m_vorticityStrength = fr.m_vorticityStrength;
			m_densityAmount = fr.m_densityAmount;
			m_densityDissipation = fr.m_densityDissipation;
			m_densityBuoyancy = fr.m_densityBuoyancy;
			m_densityWeight = fr.m_densityWeight;
			m_temperatureAmount = fr.m_temperatureAmount;
			m_temperatureDissipation = fr.m_temperatureDissipation;
			m_velocityDissipation = fr.m_velocityDissipation;
			m_inputRadius = fr.m_inputRadius;


			m_inputPos = fr.m_inputPos;

			m_ambientTemperature = fr.m_ambientTemperature;
			m_size = fr.m_size;

			smokeAbsorption = fr.smokeAbsorption;
			smokeColor = fr.smokeColor;
		}
		void FluidRenderer::Initialize()
		{
			m_width = Util::ClosestPowerOfTwo(m_width);
			m_height = Util::ClosestPowerOfTwo(m_height);
			m_depth = Util::ClosestPowerOfTwo(m_depth);

			//Put all dimension sizes in a vector for easy parsing to shader and also prevents user changing
			//dimension sizes during play
			//m_size = new vec4(m_width, m_height, m_depth, 0.0f);

			//Create all the buffers needed

			m_SmokeRaycastShader = new GraphicsShader("shaders/SmokeRayCast.vert", "shaders/SmokeRayCast.frag");

			int SIZE = m_width * m_height*m_depth;


			m_density[READ] = new ComputeBuffer(SIZE, sizeof(float));
			m_density[WRITE] = new ComputeBuffer(SIZE, sizeof(float));

			m_temperature[READ] = new ComputeBuffer(SIZE, sizeof(float));
			m_temperature[WRITE] = new ComputeBuffer(SIZE, sizeof(float));


			m_phi[READ] = new ComputeBuffer(SIZE, sizeof(float));
			m_phi[WRITE] = new ComputeBuffer(SIZE, sizeof(float));


			m_velocity[READ] = new ComputeBuffer(SIZE, sizeof(float) * 3);
			m_velocity[WRITE] = new ComputeBuffer(SIZE, sizeof(float) * 3);


			m_pressure[READ] = new ComputeBuffer(SIZE, sizeof(float));
			m_pressure[WRITE] = new ComputeBuffer(SIZE, sizeof(float));

			m_obstacles = new ComputeBuffer(SIZE, sizeof(float));

			m_temp3f = new ComputeBuffer(SIZE, sizeof(float) * 3);


			m_applyImpulse = new ComputeShader("shaders/ApplyImpulse.comp");
			m_applyAdvect = new ComputeShader("shaders/ApplyAdvection.comp");
			m_applyAdvectVelocity = new ComputeShader("shaders/ApplyAdvectionVelocity.comp");
			m_computeVorticity = new ComputeShader("shaders/ComputeVorticity.comp");

			m_computeDivergence = new ComputeShader("shaders/ComputeDivergence.comp");
			m_computeJacobi = new ComputeShader("shaders/ComputeJacobi.comp");
			m_computeProjection = new ComputeShader("shaders/ComputeProjection.comp");
			m_computeConfinement = new ComputeShader("shaders/ComputeConfinement.comp");
			m_computeObstacles = new ComputeShader("shaders/ComputeObstacles.comp");
			m_applyBuoyancy = new ComputeShader("shaders/ApplyBuoyancy.comp");

			//Any areas that are obstacles need to be masked of in the obstacle buffer
			//At the moment is only the border around the edge of the buffers to enforce non-slip boundary conditions
			ComputeObstacles();

		}



		void FluidRenderer::Update()
		{



			float dt = TIME_STEP;

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			//First off advect any buffers that contain physical quantities like density or temperature by the 
			//velocity field. Advection is what moves values around.
			ApplyAdvection(dt, m_temperatureDissipation, 0.0f, m_temperature);

			//Normal advection can cause smoothing of the advected field making the results look less interesting.
			//BFECC is a method of advection that helps to prevents this smoothing at a extra performance cost but is less numerically stable.
			//MacCormack does the same as BFECC but is more (not completely) numerically stable and is more costly
			//if (m_advectionType == ADVECTION.BFECC)
			//{
			//	ApplyAdvection(dt, 1.0f, 0.0f, m_density, 1.0f); //advect forward into write buffer
			//	ApplyAdvection(dt, 1.0f, 0.0f, m_density[READ], m_phi[PHI_N_HAT], -1.0f); //advect back into phi_n_hat buffer
			//	ApplyAdvectionBFECC(dt, m_densityDissipation, 0.0f, m_density); //advect using BFECC
			//}
			//else if (m_advectionType == ADVECTION.MACCORMACK)
			//{
			//	ApplyAdvection(dt, 1.0f, 0.0f, m_density[READ], m_phi[PHI_N_1_HAT], 1.0f); //advect forward into phi_n_1_hat buffer
			//	ApplyAdvection(dt, 1.0f, 0.0f, m_phi[PHI_N_1_HAT], m_phi[PHI_N_HAT], -1.0f); //advect back into phi_n_hat buffer
			//	ApplyAdvectionMacCormack(dt, m_densityDissipation, 0.0f, m_density);
			//}
			//else
			//{
			//	ApplyAdvection(dt, m_densityDissipation, 0.0f, m_density);
			//}
			ApplyAdvection(dt, m_densityDissipation, 0.0f, m_density);
			//The velocity field also advects its self. 
			ApplyAdvectionVelocity(dt);

			//Apply the effect the sinking colder smoke has on the velocity field
			ApplyBuoyancy(dt);

			//Adds a certain amount of density (the visible smoke) and temperate
			ApplyImpulse(dt, m_densityAmount, m_density);
			ApplyImpulse(dt, m_temperatureAmount, m_temperature);

			//The fuild sim math tends to remove the swirling movement of fluids.
			//This step will try and add it back in
			ComputeVorticityConfinement(dt);

			//Compute the divergence of the velocity field. In fluid simulation the
			//fluid is modelled as being incompressible meaning that the volume of the fluid
			//does not change over time. The divergence is the amount the field has deviated from being divergence free
			ComputeDivergence();

			//This computes the pressure need return the fluid to a divergence free condition
			ComputePressure();

			//Subtract the pressure field from the velocity field enforcing the divergence free conditions
			ComputeProjection();

			//rotation of box not support because ray cast in shader uses a AABB intersection


			//transform.rotation = Quaternion.identity;

			//GetComponent<Renderer>().material.SetVector("_Translate", transform.localPosition);
			//GetComponent<Renderer>().material.SetVector("_Scale", transform.localScale);
			//GetComponent<Renderer>().material.SetBuffer("_Density", m_density[READ]);
			//GetComponent<Renderer>().material.SetVector("_Size", m_size);


		}



		void FluidRenderer::Swap(ComputeBuffer* buffer[2])
		{
			ComputeBuffer* tmp = buffer[READ];
			buffer[READ] = buffer[WRITE];
			buffer[WRITE] = tmp;
		}

		void FluidRenderer::ComputeObstacles()
		{
			m_computeObstacles->enable();
			m_computeObstacles->setUniform4f("Size", m_size);
			m_computeObstacles->setBuffer("_Write", *m_obstacles);
			m_computeObstacles->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

		}

		void FluidRenderer::ApplyImpulse(float dt, float amount, ComputeBuffer* buffer[2])
		{
			m_applyImpulse->enable();
			m_applyImpulse->setUniform4f("Size", m_size);
			m_applyImpulse->setUniform1f("Radius", m_inputRadius);
			m_applyImpulse->setUniform1f("Amount", amount);
			m_applyImpulse->setUniform1f("DeltaTime", dt);
			m_applyImpulse->setUniform4f("Pos", m_inputPos);

			m_applyImpulse->setBuffer("_Read", *buffer[READ]);
			m_applyImpulse->setBuffer("_Write", *buffer[WRITE]);

			m_applyImpulse->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			Swap(buffer);
		}

		void FluidRenderer::ApplyBuoyancy(float dt)
		{
			m_applyBuoyancy->enable();
			m_applyBuoyancy->setUniform4f("Size", m_size);
			m_applyBuoyancy->setUniform4f("Up", glm::vec4(0, 1, 0, 0));
			m_applyBuoyancy->setUniform1f("Buoyancy", m_densityBuoyancy);
			m_applyBuoyancy->setUniform1f("AmbientTemperature", m_ambientTemperature);
			m_applyBuoyancy->setUniform1f("Weight", m_densityWeight);
			m_applyBuoyancy->setUniform1f("DeltaTime", dt);

			m_applyBuoyancy->setBuffer("_Write", *m_velocity[WRITE]);
			m_applyBuoyancy->setBuffer("_Velocity", *m_velocity[READ]);
			m_applyBuoyancy->setBuffer("_Density", *m_density[READ]);
			m_applyBuoyancy->setBuffer("_Temperature", *m_temperature[READ]);

			m_applyBuoyancy->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			Swap(m_velocity);
		}

		void FluidRenderer::ApplyAdvection(float dt, float dissipation, float decay, ComputeBuffer* buffer[2], float forward)
		{
			m_applyAdvect->enable();
			m_applyAdvect->setUniform3f("Size", glm::vec3(m_size.x, m_size.y, m_size.z));
			m_applyAdvect->setUniform1f("DeltaTime", dt);
			m_applyAdvect->setUniform1f("Dissipate", dissipation);
			m_applyAdvect->setUniform1f("Forward", forward);
			m_applyAdvect->setUniform1f("Decay", decay);

			m_applyAdvect->setBuffer("_Read1f", *buffer[READ]);
			m_applyAdvect->setBuffer("_Write1f", *buffer[WRITE]);
			m_applyAdvect->setBuffer("_Velocity", *m_velocity[READ]);
			m_applyAdvect->setBuffer("_Obstacles", *m_obstacles);

			m_applyAdvect->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			Swap(buffer);
		}

		void FluidRenderer::ApplyAdvection(float dt, float dissipation, float decay, ComputeBuffer& read, ComputeBuffer& write, float forward)
		{
			m_applyAdvect->enable();
			m_applyAdvect->setUniform4f("Size", m_size);
			m_applyAdvect->setUniform1f("DeltaTime", dt);
			m_applyAdvect->setUniform1f("Dissipate", dissipation);
			m_applyAdvect->setUniform1f("Forward", forward);
			m_applyAdvect->setUniform1f("Decay", decay);

			m_applyAdvect->setBuffer("_Read1f", read);
			m_applyAdvect->setBuffer("_Write1f", write);
			m_applyAdvect->setBuffer("_Velocity", *m_velocity[READ]);
			m_applyAdvect->setBuffer("_Obstacles", *m_obstacles);

			m_applyAdvect->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);
			m_applyAdvect->disable();
		}


		void FluidRenderer::ApplyAdvectionBFECC(float dt, float dissipation, float decay, ComputeBuffer* buffer[2])
		{
			m_applyAdvect->enable();
			m_applyAdvect->setUniform4f("Size", m_size);
			m_applyAdvect->setUniform1f("DeltaTime", dt);
			m_applyAdvect->setUniform1f("Dissipate", dissipation);
			m_applyAdvect->setUniform1f("Forward", 1.0f);
			m_applyAdvect->setUniform1f("Decay", decay);

			m_applyAdvect->setBuffer("_Read1f", *buffer[READ]);
			m_applyAdvect->setBuffer("_Write1f", *buffer[WRITE]);
			m_applyAdvect->setBuffer("_Phi_n_hat", *m_phi[PHI_N_HAT]);
			m_applyAdvect->setBuffer("_Velocity", *m_velocity[READ]);
			m_applyAdvect->setBuffer("_Obstacles", *m_obstacles);

			m_applyAdvect->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);
			m_applyAdvect->disable();
			Swap(buffer);
		}

		void FluidRenderer::ApplyAdvectionMacCormack(float dt, float dissipation, float decay, ComputeBuffer* buffer[2])
		{
			m_applyAdvect->enable();
			m_applyAdvect->setUniform4f("Size", m_size);
			m_applyAdvect->setUniform1f("DeltaTime", dt);
			m_applyAdvect->setUniform1f("Dissipate", dissipation);
			m_applyAdvect->setUniform1f("Forward", 1.0f);
			m_applyAdvect->setUniform1f("Decay", decay);

			m_applyAdvect->setBuffer("_Read1f", *buffer[READ]);
			m_applyAdvect->setBuffer("_Write1f", *buffer[WRITE]);
			m_applyAdvect->setBuffer("_Phi_n_1_hat", *m_phi[PHI_N_1_HAT]);
			m_applyAdvect->setBuffer("_Phi_n_hat", *m_phi[PHI_N_HAT]);
			m_applyAdvect->setBuffer("_Velocity", *m_velocity[READ]);
			m_applyAdvect->setBuffer("_Obstacles", *m_obstacles);

			m_applyAdvect->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);
			m_applyAdvect->disable();
			Swap(buffer);
		}

		void FluidRenderer::ApplyAdvectionVelocity(float dt)
		{
			m_applyAdvectVelocity->enable();
			m_applyAdvectVelocity->setUniform3f("Size", glm::vec3(m_size.x, m_size.y, m_size.z));
			m_applyAdvectVelocity->setUniform1f("DeltaTime", dt);
			m_applyAdvectVelocity->setUniform1f("Dissipate", m_velocityDissipation);
			m_applyAdvectVelocity->setUniform1f("Forward", 1.0f);
			m_applyAdvectVelocity->setUniform1f("Decay", 0.0f);

			m_applyAdvectVelocity->setBuffer("_Read3f", *m_velocity[READ]);
			m_applyAdvectVelocity->setBuffer("_Write3f", *m_velocity[WRITE]);
			m_applyAdvectVelocity->setBuffer("_Velocity", *m_velocity[READ]);
			m_applyAdvectVelocity->setBuffer("_Obstacles", *m_obstacles);

			m_applyAdvect->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			Swap(m_velocity);
		}

		void FluidRenderer::ComputeVorticityConfinement(float dt)
		{
			m_computeVorticity->enable();
			m_computeVorticity->setUniform4f("Size", m_size);

			m_computeVorticity->setBuffer("_Write", *m_temp3f);
			m_computeVorticity->setBuffer("_Velocity", *m_velocity[READ]);

			m_computeVorticity->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			m_computeConfinement->enable();
			m_computeConfinement->setUniform4f("Size", m_size);
			m_computeConfinement->setUniform1f("DeltaTime", dt);
			m_computeConfinement->setUniform1f("Epsilon", m_vorticityStrength);

			m_computeConfinement->setBuffer("_Write", *m_velocity[WRITE]);
			m_computeConfinement->setBuffer("_Read", *m_velocity[READ]);
			m_computeConfinement->setBuffer("_Vorticity", *m_temp3f);

			m_computeConfinement->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			Swap(m_velocity);
		}

		void FluidRenderer::ComputeDivergence()
		{
			m_computeDivergence->enable();
			m_computeDivergence->setUniform4f("Size", m_size);

			m_computeDivergence->setBuffer("_Write", *m_temp3f);
			m_computeDivergence->setBuffer("_Velocity", *m_velocity[READ]);
			m_computeDivergence->setBuffer("_Obstacles", *m_obstacles);

			m_computeDivergence->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);
		}

		void FluidRenderer::ComputePressure()
		{
			m_computeJacobi->enable();
			m_computeJacobi->setUniform4f("Size", m_size);
			m_computeJacobi->setBuffer("_Divergence", *m_temp3f);
			m_computeJacobi->setBuffer("_Obstacles", *m_obstacles);

			for (int i = 0; i < m_iterations; i++)
			{
				m_computeJacobi->setBuffer("_Write", *m_pressure[WRITE]);
				m_computeJacobi->setBuffer("_Pressure", *m_pressure[READ]);

				m_computeJacobi->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

				Swap(m_pressure);
			}
		}

		void FluidRenderer::ComputeProjection()
		{
			m_computeProjection->enable();
			m_computeProjection->setUniform4f("Size", m_size);
			m_computeProjection->setBuffer("_Obstacles", *m_obstacles);

			m_computeProjection->setBuffer("_Pressure", *m_pressure[READ]);
			m_computeProjection->setBuffer("_Velocity", *m_velocity[READ]);
			m_computeProjection->setBuffer("_Write", *m_velocity[WRITE]);

			m_computeProjection->Dispatch((int)m_size.x / NUM_THREADS, (int)m_size.y / NUM_THREADS, (int)m_size.z / NUM_THREADS);

			Swap(m_velocity);
		}


		void FluidRenderer::RenderScene()
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			/*
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_FACE);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
			glEnable(GL_BLEND);
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_FRONT_FACE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			/*glDisable(GL_CULL_FACE);*/
			//glEnable(GL_DEPTH_TEST);
			m_SmokeRaycastShader->enable();

			//cameraPos = glm::vec3(0.0f, 1.0f, 1.0f);
			cameraPos = glm::vec3(.0f, 3.0f, 10.0f);

			glm::mat4 mViewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
			glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
			glm::mat4 worldMatrix = glm::mat4(1.0f);
			glm::vec3 _scale(4.0f,4.0f,4.0f);
			worldMatrix = glm::scale(worldMatrix, _scale);
			worldMatrix = glm::translate(worldMatrix, glm::vec3(2 * glm::sin(lastFrame), .0f, .0f));
			worldMatrix[3][3] = 1.0f;
			//worldMatrix = glm::scale(worldMatrix, glm::vec3(2.0f, 4.0f, 2.0f));
			glm::mat4 mProjectionMatrix = glm::perspective(90.0, 1.0, 0.1, 1000.0);
			glm::vec3 _localPosition = worldMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			m_SmokeRaycastShader->setUniformMat41("pr_matrix", mProjectionMatrix);
			m_SmokeRaycastShader->setUniformMat41("v_matrix", viewMatrix);
			m_SmokeRaycastShader->setUniformMat41("m_matrix", worldMatrix);
			//m_SmokeRaycastShader->setUniformMat4("pr_matrix", projmat); 
			//m_SmokeRaycastShader->setUniformMat4("vw_matrix", viewmat*worldmat);

			m_SmokeRaycastShader->setUniform3f("WorldSpaceCameraPos", cameraPos);
			//float3 _Translate, _Scale, _Size;
		//m_SmokeRaycastShader->setUniform3f("Translate", glm::vec3(0.0f, 0.0f, 0.0f));
			m_SmokeRaycastShader->setUniform3f("Translate", _localPosition);
			//m_SmokeRaycastShader->setUniform3f("Scale", glm::vec3(1.0f,1.0f,1.0f));
			m_SmokeRaycastShader->setUniform3f("Scale", _scale);
			m_SmokeRaycastShader->setUniform3f("Size", glm::vec3(m_size.x, m_size.y, m_size.z));

			m_SmokeRaycastShader->setUniform4f("SmokeColor", smokeColor);
			m_SmokeRaycastShader->setUniform1f("SmokeAbsorption", smokeAbsorption);
			m_SmokeRaycastShader->setBuffer("_Density", *m_density[READ]);
			for (int i = 0; i < SmokeVolumes.size(); i++)
			{
				SmokeVolumes[i]->render();
			}
			m_SmokeRaycastShader->disable();

		}

		void FluidRenderer::AddSmokeVolume(Renderable* r)
		{
			SmokeVolumes.push_back(r);

		}

	}
}
