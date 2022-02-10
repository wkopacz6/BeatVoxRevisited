cmake_minimum_required(VERSION 3.15)

project(BeatVOX VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)

#JUCE

add_subdirectory(submodules/JUCE)

#BeatVOX App
juce_add_gui_app(BeatVOX
        MICROPHONE_PERMISSION_ENABLED TRUE
        )

target_link_libraries(BeatVOX PRIVATE
        application
        )