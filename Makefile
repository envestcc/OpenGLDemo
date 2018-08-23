glad = /Users/chen/opengl/libs/glad/src/glad.c
glad_linux = /search/odin/develop/opengl/libs/glad/src/glad.c
BUILD_PATH = build
exe = $(BUILD_PATH)/a.out
INCLUDE = -I include -I /Users/chen/opengl/libs/ -I /search/odin/develop/opengl/libs/glad/include -I /search/odin/develop/opengl/libs/glm
OBJS = $(BUILD_PATH)/obj/main.o $(BUILD_PATH)/obj/shader.o $(BUILD_PATH)/obj/resource_manager.o $(BUILD_PATH)/obj/game.o $(BUILD_PATH)/obj/texture.o $(BUILD_PATH)/obj/stb_image_use.o $(BUILD_PATH)/obj/sprite_render.o
CC = g++

run: $(exe)
	build/a.out

linux: $(OBJS) $(glad_linux)
	$(CC) $(OBJS) $(glad_linux) -g -v -lglfw -lGL -ldl -std=c++11 -o $(exe) $(INCLUDE)

osx: $(OBJS) $(glad)
	$(CC) $(OBJS) $(glad) -g -lglfw -framework OpenGL -o $(exe) $(INCLUDE)


$(BUILD_PATH)/obj/main.o: src/main.cpp
	$(CC) -c -g -std=c++11 src/main.cpp -o $(BUILD_PATH)/obj/main.o $(INCLUDE)

$(BUILD_PATH)/obj/shader.o: src/shader.cpp include/shader.h
	$(CC) -c -g -std=c++11 src/shader.cpp -o $(BUILD_PATH)/obj/shader.o $(INCLUDE) 

$(BUILD_PATH)/obj/resource_manager.o: src/resource_manager.cpp include/resource_manager.h
	$(CC) -c -g -std=c++11 src/resource_manager.cpp -o $(BUILD_PATH)/obj/resource_manager.o $(INCLUDE) 

$(BUILD_PATH)/obj/game.o: src/game.cpp include/game.h
	$(CC) -c -g -std=c++11 src/game.cpp -o $(BUILD_PATH)/obj/game.o $(INCLUDE)

$(BUILD_PATH)/obj/texture.o: src/texture.cpp include/texture.h
	$(CC) -c -g -std=c++11 src/texture.cpp -o $(BUILD_PATH)/obj/texture.o $(INCLUDE)

$(BUILD_PATH)/obj/stb_image_use.o: src/stb_image_use.cpp
	$(CC) -c -g -std=c++11 src/stb_image_use.cpp -o $(BUILD_PATH)/obj/stb_image_use.o $(INCLUDE)

$(BUILD_PATH)/obj/sprite_render.o: src/sprite_render.cpp include/sprite_render.h
	$(CC) -c -g -std=c++11 src/sprite_render.cpp -o $(BUILD_PATH)/obj/sprite_render.o $(INCLUDE)

clean:
	rm -rf build/obj/*
	rm $(exe)