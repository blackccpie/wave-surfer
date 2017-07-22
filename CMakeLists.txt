#The MIT License
#
#Copyright (c) 2017-2017 Albert Murienne
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in
#all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#THE SOFTWARE.

cmake_minimum_required (VERSION 3.2)
project (wave_surfer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (USE_EMSCRIPTEN)

    # NOTHING TO DO YET

else ()

	set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall -Wpedantic -Wno-narrowing")

endif ()

# Set compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")

# Set install dir
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/installation")

set (sources_list
main.cpp
enemy.cpp
game.cpp
obst_spawner.cpp
sprite.cpp
text.cpp
)

add_executable(wave_surfer ${sources_list} ${headers_list})

target_link_libraries(wave_surfer
    SDL2
    SDL2_image
    SDL2_ttf
)

install(
    TARGETS wave_surfer
    DESTINATION wave_surfer
)

install(
    DIRECTORY resources
    DESTINATION wave_surfer
)
