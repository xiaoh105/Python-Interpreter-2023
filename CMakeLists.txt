### YOU CAN'T MODIFY THE CODE BELOW
cmake_minimum_required(VERSION 3.15.2)
project(python_interpreter)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-use")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

include_directories(/usr/include/antlr4-runtime/)
include_directories(
	${PROJECT_SOURCE_DIR}/generated/
	${PROJECT_SOURCE_DIR}/src/
)

add_subdirectory(${PROJECT_SOURCE_DIR}/generated)
### YOU CAN"T MODIFY THE CODE ABOVE

file(GLOB_RECURSE main_src src/*.cpp)

add_executable(code ${main_src}
        src/VarOperator.h
				src/BuiltinFuncs.h
				src/Utils.h
				src/Constant.h
        src/Scope.cpp
        src/Scope.h
				src/VarOperator.cpp
        src/BuiltinFuncs.cpp
        src/Utils.cpp
) # Add all *.cpp file after src/main.cpp, like src/Evalvisitor.cpp did

### YOU CAN'T MODIFY THE CODE BELOW
target_link_libraries(code PyAntlr)
target_link_libraries(code antlr4-runtime)
### YOU CAN"T MODIFY THE CODE ABOVE
