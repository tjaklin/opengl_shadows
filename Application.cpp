// TODO: Ubacit sve ove '3rd party dependencije' u Dependency folder, koji je dio
//			projekta samoga !
// TODO: Gornji tudu malo izmjenit, na nacin da zapravo ne ubacim sve dependensije u
//			zaseban Dependency folder, nego da umjesto toga, u repozitorij, ubacim
//			koristenje CMake-a, umjesto trenutnog Make-a. Zatim sa tim CMake-om mogu
//			napisat pravila za ocekivane Dependencyje koje projekt koristi i, kaj
//			napravit u slucaju da nisu pristuni (tj. napisat pravilo da se Dependencyji
//			automacki skinu s githaba) !

#include "inc/Window.hpp"
#include "inc/Scene.hpp"
#include "inc/DefaultScene.hpp"
#include "inc/DirectionalShadowMap.hpp"

/*
    #include "inc/ShadowMap_omni.h"
    #include "inc/ShadowVolume.h"
    #include "inc/ShadowMap_variance.h"
*/

/*
void checKeyState( uint* state, bool* isMoving, bool* isPCF, GLFWwindow* mWindow ){
	if (glfwGetKey( mWindow, GLFW_KEY_1 ) == GLFW_PRESS)
		*state = 1;
	if (glfwGetKey( mWindow, GLFW_KEY_2 ) == GLFW_PRESS)
		*state = 2;
	if (glfwGetKey( mWindow, GLFW_KEY_3 ) == GLFW_PRESS)
		*state = 3;
	if (glfwGetKey( mWindow, GLFW_KEY_4 ) == GLFW_PRESS)
		*state = 4;
	if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_PRESS)
		if (glfwGetKey( mWindow, GLFW_KEY_L ) == GLFW_RELEASE)
			*isMoving = !*isMoving;
	if (glfwGetKey( mWindow, GLFW_KEY_Q ) == GLFW_PRESS)
		if (glfwGetKey( mWindow, GLFW_KEY_Q ) == GLFW_RELEASE)
			*isPCF = !*isPCF;
}
*/

int main(int argc, char** argv)
{
    // Ignore input data for now.
    (void) argc; (void) argv;

    Window window(1024, 1024, "OpenGL Sjene");

	// Prepare the scene's camera object
	Camera eye;
	glm::vec3 eyePosition = glm::vec3(0.0f, 5.0f, 5.0f);
	eye.SetViewMatrix(eyePosition, glm::vec3(-3.0f, 1.5f,-1.0f), glm::vec3(0,1,0));
	eye.SetPerspectiveProjectionMatrix(45.0f, 4.0f/3.0f, 1.0f, 100.0f);
	// Set this object as the scene's main camera object.
	// This enables it to handle movement input events.
	window.SetCamera(&eye);
	
	// Prepare 3D shape data.
	const char* cube_position_filepath = "vertices/cube_position.txt";
	VertexAttribute position = VertexAttributeParser::ProcessFile(cube_position_filepath);
	if (position.data.empty()) { printf("VertexAttribute is empty. Exiting.\n"); return -1; }
	
	const char* cube_color_filepath = "vertices/cube_color.txt";
	VertexAttribute color = VertexAttributeParser::ProcessFile(cube_color_filepath);
	if (color.data.empty()) { printf("VertexAttribute is empty. Exiting.\n"); return -1; }

	const char* cube_normal_filepath = "vertices/cube_normal.txt";
	VertexAttribute normal = VertexAttributeParser::ProcessFile(cube_normal_filepath);
	if (normal.data.empty()) { printf("VertexAttribute is empty. Exiting.\n"); return -1; }

	// Prepare 'Model' data. That's about all the necessary data that's needed
	//	for a shape to get drawn on screen.
	Model kocka;
	kocka.PushVertexAttribute(position, 0);
	//kocka.PushVertexAttribute(color, 1);
	kocka.PushVertexAttribute(normal, 1);
	kocka.SetScale(glm::vec3(1.0f));
	kocka.SetTranslation(glm::vec3(-3.0f, 3.0f,-1.0f));
	auto kockaModel = kocka.GetModelMatrix();

	// Prepare a 'ground' object, on which
	//	the shadows cast by other models will be cast.
	Model podloga;
	podloga.PushVertexAttribute(position, 0);
	podloga.PushVertexAttribute(normal, 1);
	podloga.SetScale(glm::vec3(15.0f, 1.0f, 15.0f));
	podloga.SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
	auto podlogaModel = podloga.GetModelMatrix();

	// Prepare default shader data.
	const char* vertexDefaultFilepath = "shaders/default.vs";
	const char* fragmentDefaultFilepath = "shaders/default.fs";
	Shader defaultShader(vertexDefaultFilepath, fragmentDefaultFilepath);

	// Prepare Directional Shadow Map shader data.
	const char* vertexShadowMapDepthFilepath = "shaders/shadow_map/directional/sm_depth.vs";
	const char* fragmentShadowMapDepthFilepath = "shaders/shadow_map/directional/sm_depth.fs";
	Shader shadowMapDepth(vertexShadowMapDepthFilepath, fragmentShadowMapDepthFilepath);

	const char* vertexShadowMapLightFilepath = "shaders/shadow_map/directional/sm_light.vs";
	const char* fragmentShadowMapLightFilepath = "shaders/shadow_map/directional/sm_light.fs";
	Shader shadowMapLight(vertexShadowMapLightFilepath, fragmentShadowMapLightFilepath);

	// Set up the Directional Shadow Map object.
	//DirectionalShadowMap shadowMap(960, 540, 960, 540);
	DirectionalShadowMap shadowMap(1024, 1024, 1024, 1024);
	shadowMap.LoadShaders(&shadowMapDepth, &shadowMapLight);
	shadowMap.PrepareFBOandTexture();	// TODO: Ovo bi moglo biti prebacenu u neki cstr ?

	// Use the default shader for now.
	defaultShader.Bind();
	auto shader = defaultShader.Get();

	// Za potrebe debagiranja, postavljam kameru na polozaj svjetla !!
	eye.SetViewMatrix(
		glm::vec3(-3.0f, 20.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)		// Up vector.
	);

	// Main update and draw loop.
    while (!window.ShouldClose())
    {
		window.Clear();
		window.Update();

		auto eyeView = eye.GetViewMatrix();
		auto eyeProjection = eye.GetProjectionMatrix();

		// Forward View and Projection matrix data to the shader.
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &eyeView[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &eyeProjection[0][0]);

		if (false)	// Najobicnije crtanje koje radi ispravno. Komentirano za test.
		{
			// Forward View and Projection matrix data to the shader.
			glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &eyeView[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &eyeProjection[0][0]);

			// Position and draw the object.
			glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &podlogaModel[0][0]);
			podloga.Draw();

			// Position and draw the object.
			glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &kockaModel[0][0]);
			kocka.Draw();
		}

		// Test koristenja DirectionalShadowMap.
		if (true)
		{
			// Set the directional light's color data.
			glm::vec3 directional_ambient_color = glm::vec3(0.2f);
			glm::vec3 directional_diffuse_color = glm::vec3(0.8f);

			// Set the Camera object for the directional light.
			Camera directional_camera;
			directional_camera.SetOrthogonalProjectionMatrix(-10, 10,-20, 20,-10, 20);
			directional_camera.SetViewMatrix(
				glm::vec3(-3.0, 20.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)		// Up vector.
			);

			// Get ready for the First stage of drawing.
			//	In this stage, the scene is drawn from the
			//	perspective of 'directional_camera', which
			//	has a position and it's 'looking' orientation
			//	is orientated towards the objects that need
			//	to cast shadows.			
			shadowMap.FirstPassSetup();

			// Set the shader uniform data for the First drawing stage.
			shadowMap.SetLightMvpMatrix(directional_camera.GetProjectionMatrix() * directional_camera.GetViewMatrix() * kockaModel);
			// Draw all objects that need to cast shadows.
			kocka.Draw();

			// Get ready for the Second stage of drawing.
			//	In this stage, the scene is drawn regularly
			//	from the perspective of main camera.
			glm::mat4 biasMVP = glm::mat4(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);
			glm::mat4 lightBiasMVP = biasMVP * directional_camera.GetProjectionMatrix() * directional_camera.GetViewMatrix() * kockaModel;

			// Connect all the 'uniform' data to be sent to shaders.
			shadowMap.SecondPassSetup();
			shadowMap.SetViewMatrix(eye.GetViewMatrix());
			shadowMap.SetProjectionMatrix(eyeProjection);
			shadowMap.SetLightDirection(glm::vec3(-3.0, 20.0f,-1.0f));
			shadowMap.SetBiasedLightMvpMatrix(lightBiasMVP);
			shadowMap.SetIsPCF(false);
			shadowMap.BindDepthTexture();

			// Draw all objects in the scene regularly.
			shadowMap.SetModelMatrix(kockaModel);
			kocka.Draw();

			shadowMap.SetModelMatrix(podlogaModel);
			lightBiasMVP = biasMVP * directional_camera.GetProjectionMatrix() * directional_camera.GetViewMatrix() * podlogaModel;
			shadowMap.SetBiasedLightMvpMatrix(lightBiasMVP);
			podloga.Draw();
		}

		// Display Window's framebuffer.
        window.Draw();
    }

    return 0;
}
