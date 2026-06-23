cmake_minimum_required(VERSION 3.15)
project(MotorVoxeles CXX)

# Usar C++17 o superior
set(CMAKE_CXX_STANDARD 17)

# Buscar las librerías instaladas por vcpkg
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)

# Crear el ejecutable
add_executable(MotorVoxeles main.cpp)

# Enlazar las librerías al ejecutable
target_link_libraries(MotorVoxeles PRIVATE glfw glm::glm glad::glad)