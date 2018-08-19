glad = /Users/chen/opengl/libs/glad/src/glad.c
OBJS = $(BUILD_PATH)/obj/main.o $(BUILD_PATH)/obj/shader.o 
BUILD_PATH = build
exe = $(BUILD_PATH)/a.out
INCLUDE = -I include -I /Users/chen/opengl/libs/


run: $(exe)
	build/a.out

$(BUILD_PATH)/obj/main.o: src/main.cpp
	clang++ -c src/main.cpp -o $(BUILD_PATH)/obj/main.o $(INCLUDE)

$(BUILD_PATH)/obj/shader.o: src/shader.cpp include/shader.h
	clang++ -c src/shader.cpp -o $(BUILD_PATH)/obj/shader.o $(INCLUDE) 

$(exe): $(OBJS) $(glad)
	clang++ $(OBJS) $(glad) -g -lglfw -framework OpenGL -o $(exe) $(INCLUDE)