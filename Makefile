glad = /Users/chen/opengl/libs/glad/src/glad.c
glad_linux = /search/odin/develop/opengl/libs/glad/src/glad.c
OBJS = $(BUILD_PATH)/obj/main.o $(BUILD_PATH)/obj/shader.o 
BUILD_PATH = build
exe = $(BUILD_PATH)/a.out
INCLUDE = -I include -I /Users/chen/opengl/libs/ -I /search/odin/develop/opengl/libs/glad/include -I /search/odin/develop/opengl/libs/glm


run: $(exe)
	build/a.out

linux: $(OBJS) $(glad_linux)
	clang++ $(OBJS) $(glad_linux) -g -lglfw -lGL -ldl -o $(exe) $(INCLUDE)

osx: $(OBJS) $(glad)
	clang++ $(OBJS) $(glad) -g -lglfw -framework OpenGL -o $(exe) $(INCLUDE)


$(BUILD_PATH)/obj/main.o: src/main.cpp
	clang++ -c src/main.cpp -o $(BUILD_PATH)/obj/main.o $(INCLUDE)

$(BUILD_PATH)/obj/shader.o: src/shader.cpp include/shader.h
	clang++ -c src/shader.cpp -o $(BUILD_PATH)/obj/shader.o $(INCLUDE) 


clean:
	rm -rf build/obj/*
	rm $(exe)