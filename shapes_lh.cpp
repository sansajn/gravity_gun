// left hand coordinate system camera sample
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>
#include "glt/gles2/mesh_gles2.hpp"
#include "glt/shapes.hpp"
#include "glt/colors.hpp"
#include "glt/gles2/program_gles2.hpp"
#include "glt/gles2/default_shader_gles2.hpp"
#include "phys/Camera.h"

static const GLuint WIDTH = 800;
static const GLuint HEIGHT = 600;

using std::cout, std::endl;
using glm::vec3, glm::mat4, glm::mat3;
using	glm::perspective, 
	glm::lookAt, 
	glm::radians,
	glm::translate,
	glm::inverseTranspose;
using gles2::mesh;

mat4 glm_cast(phys_cookbook::mat4 const & m)
{
	return mat4{
		m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44
	};
}

phys_cookbook::mat4 phys_cast(mat4 const & m)
{
	return phys_cookbook::mat4{
		m[0].x, m[0].y, m[0].z, m[0].w,
		m[1].x, m[1].y, m[1].z, m[1].w,
		m[2].x, m[2].y, m[2].z, m[2].w,
		m[3].x, m[3].y, m[3].z, m[3].w
	};
}

int main(int argc, char * argv[])
{
	GLFWwindow * window;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
	glfwMakeContextCurrent(window);

	printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
	printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );
	
	// left handed matrices (transformations needed to be done oposite side as in glm)
	phys_cookbook::OrbitCamera cam;
	cam.Perspective(60.f, WIDTH/(float)HEIGHT, 0.01f, 1000.f);
	cam.SetTarget(phys_cookbook::vec3{0,0,0});
	cam.SetZoom(0);
	cam.SetRotation(phys_cookbook::vec2{0,0});
	cam.Update(0);
	phys_cookbook::mat4 phys_V = cam.GetViewMatrix(),
		phys_P = cam.GetProjectionMatrix();
	
	gles2::shader::program shaded;
	shaded.from_memory(gles2::flat_shaded_shader_source, 100);
	GLint pos = shaded.attribute_location("position");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnableVertexAttribArray(pos);

	gl::shape_generator<gles2::mesh> shape;
	mesh cube = shape.cube(),
		box = shape.box(vec3{.5, 1, 0.5}),
		disk = shape.disk(.5),
		cylinder = shape.cylinder(.5, .5, 30),
		open_cylinder = shape.open_cylinder(.5, 1, 20),
		cone = shape.cone(.5, 1),
		sphere = shape.sphere(.5),
		circle = shape.circle(.5),
		ring = shape.ring(.25, .5, 30);

	vec3 const light_pos = vec3{10, 20, 30};  // TODO: change light position

	shaded.use();

	GLint position_a = glGetAttribLocation(shaded.id(), "position"),
		normal_a = glGetAttribLocation(shaded.id(), "normal");

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// cube
		
// 		mat4 VP = P * V;
		mat4 VP = glm_cast(phys_P) * glm_cast(phys_V);
		mat4 M = translate(vec3{0, 0, 10});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::gray);
		shaded.uniform_variable("light_dir", normalize(light_pos));

		cube.attribute_location({position_a, normal_a});
		cube.render();

		// box
		M = translate(vec3{2, 0, 2});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::teal);
		box.attribute_location({position_a, normal_a});
		box.render();

		// disk
		M = translate(vec3{1, 0, -2});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::yellow);
		disk.attribute_location({position_a, normal_a});
		disk.render();

		// cylinder
		M = translate(vec3{-1.5, 0, .4});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::olive);
		cylinder.attribute_location({position_a, normal_a});
		cylinder.render();

		// open cylinder
		M = translate(vec3{-.2, 0, 2});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::maroon);
		open_cylinder.attribute_location({position_a, normal_a});
		open_cylinder.render();

		// cone
		M = translate(vec3{-2, 0, -1.5});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::purple);
		cone.attribute_location({position_a, normal_a});
		cone.render();

		// sphere
		M = translate(vec3{-.7, 0, -1.8});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::blue_shades::cornflower_blue);
		sphere.attribute_location({position_a, normal_a});
		sphere.render();

		// circle
		M = translate(vec3{2, 0, -1});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::white);
		circle.attribute_location({position_a, normal_a});
		circle.render();

		// ring
		M = translate(vec3{-3, 0, -.5});
		shaded.uniform_variable("local_to_screen", VP*M);
		shaded.uniform_variable("normal_to_world", mat3{inverseTranspose(M)});
		shaded.uniform_variable("color", rgb::lime);
		ring.attribute_location({position_a, normal_a});
		ring.render();

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}
