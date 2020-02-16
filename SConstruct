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


cpp17 = create_build_environment()


glt = cpp17.Object([
	Glob('libs/glt/*.cpp'),
	Glob('libs/glt/gles2/*.cpp'),
	Glob('libs/glt/glfw3/*.cpp')
])

phys_camera = cpp17.Object([
	'phys/Camera.cpp',
	'phys/vectors.cpp',
	'phys/matrices.cpp'])

cpp17.Program(['test.cpp', glt, phys_camera])
cpp17.Program(['shapes.cpp', glt, phys_camera])
