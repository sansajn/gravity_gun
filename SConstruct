# dependencies:
#    libglfw3-dev (3.2.1, ubuntu 18.04)
#    libgles2-mesa-dev (18.0.5, ubuntu 18.04)
#    libglm-dev (0.9.9, ubuntu 18.04)
#    libboost-all-dev (1.65.1, ubuntu 18.04)

def create_build_environment():
	env = Environment(
		CCFLAGS=['--std=c++17', '-O0', '-g', '-Wall', '-Wno-sign-compare'],
		CPPPATH=['.', 'libs'],
		CPPDEFINES=['GLFW_INCLUDE_ES2', 'GLM_ENABLE_EXPERIMENTAL', 'GLM_FORCE_RADIANS'],
		LIBS=['boost_filesystem', 'boost_system']
	)

	env.ParseConfig('pkg-config --cflags --libs glesv2')

	# opengl wrapper
	env.Append(CPPDEFINES=['USE_GLFW3'])
	env.ParseConfig('pkg-config --cflags --libs glfw3')

	return env


env = create_build_environment()

program_objs = env.Object(['gles2/program_gles2.cpp'])

gles2_objs = [
	program_objs,
	env.Object([
		'gles2/mesh_gles2.cpp',
		'gles2/default_shader_gles2.cpp'
])]

camera_objs = env.Object(['libs/gl/camera.cpp'])

gl_objs = [
	camera_objs,
	env.Object([
		'gl/window.cpp',
		'gl/glfw3_user_input.cpp',
		'gl/glfw3_window.cpp',
])]

env.Program(['test.cpp', gles2_objs, gl_objs])
