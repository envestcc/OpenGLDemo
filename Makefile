glad = /Users/chen/opengl/libs/glad/src/glad.c
glad_linux = /search/odin/develop/opengl/libs/glad/src/glad.c
BUILD_PATH = build
exe = $(BUILD_PATH)/a.out
INCLUDE = -I include -I /Users/chen/opengl/libs/ -I /search/odin/develop/opengl/libs/glad/include -I /search/odin/develop/opengl/libs/glm
OBJS = $(BUILD_PATH)/obj/main.o $(BUILD_PATH)/obj/shader.o $(BUILD_PATH)/obj/resource_manager.o 

run: $(exe)
	build/a.out

linux: $(OBJS) $(glad_linux)
	clang++ $(OBJS) $(glad_linux) -g -v -lglfw -lGL -ldl -std=c++11 -o $(exe) $(INCLUDE)

osx: $(OBJS) $(glad)
	clang++ $(OBJS) $(glad) -g -lglfw -framework OpenGL -o $(exe) $(INCLUDE)


$(BUILD_PATH)/obj/main.o: src/main.cpp
	clang++ -c -std=c++11 src/main.cpp -o $(BUILD_PATH)/obj/main.o $(INCLUDE)

$(BUILD_PATH)/obj/shader.o: src/shader.cpp include/shader.h
	clang++ -c -std=c++11 src/shader.cpp -o $(BUILD_PATH)/obj/shader.o $(INCLUDE) 

$(BUILD_PATH)/obj/resource_manager.o: src/resource_manager.cpp include/resource_manager.h
	clang++ -c -std=c++11 src/resource_manager.cpp -o $(BUILD_PATH)/obj/resource_manager.o $(INCLUDE) 

clean:
	rm -rf build/obj/*
	rm $(exe)