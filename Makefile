glad = /Users/chen/opengl/glad/src/glad.c
exe = build/a.out

run: $(exe)
	build/a.out

$(exe): main.cpp $(glad)
	clang++ main.cpp $(glad) -g -lglfw -framework OpenGL -o $(exe) -I /Users/chenchen/opengl/glad/include