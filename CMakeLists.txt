cmake_minimum_required(VERSION 3.18)
project(sffx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

set(effectFiles 
"src/Effects/MotionBlur.cpp"
"src/Effects/Effect.cpp"
"src/Effects/Blur.cpp"
"src/Effects/base_vt.cpp"
)

set(particleFiles
"src/Particles/Particle.cpp"
"src/Particles/ParticleEngine.cpp"
"src/Particles/ParticleTemplates.cpp"
"src/Particles/Utils/utils.cpp"
)

set(asyncFiles
"src/async/async.cpp")

set(demofiles
"examples/main.cpp"
)

add_library( sffx ${effectFiles} ${particleFiles} ${asyncFiles} )
target_include_directories(sffx PUBLIC "include/sffx/Particles")
target_include_directories(sffx PUBLIC "include/sffx")
target_include_directories(sffx PUBLIC "include/sffx/Effects")


add_executable(demo.exe ${demofiles})
target_include_directories(demo.exe PUBLIC "./src/")
target_link_libraries(demo.exe sfml-graphics sfml-window sfml-system pthread sfml-network sfml-audio)
target_link_libraries(demo.exe sffx)
target_include_directories(demo.exe PUBLIC "include/")
IF (WIN32)
  # set stuff for windows
ELSE()
target_link_libraries(demo.exe pthread)
ENDIF()
target_compile_options(demo.exe PUBLIC -g)