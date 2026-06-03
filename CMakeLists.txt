cmake_minimum_required(VERSION 3.22)

# Crear el proyecto del Mod
project(Cps4sMod)

# Buscar y configurar el SDK de Geode instalado
find_package(Geode REQUIRED)

# Añadir los archivos fuente
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES main.cpp
)
