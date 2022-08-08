cmake_minimum_required(VERSION 3.15)

project(BeatVOX VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

#JUCE

add_subdirectory(submodules/JUCE)
add_subdirectory(submodules/googletest)
add_subdirectory(source/utils)
add_subdirectory(source/segmentation)
add_subdirectory(source/feature-extraction)

#BeatVOX App
juce_add_gui_app(BeatVOX
        MICROPHONE_PERMISSION_ENABLED TRUE
        )

target_link_libraries(BeatVOX PRIVATE
        application
        segmentation
        utils
        mfcc
        juce::juce_core
        juce::juce_gui_basics
        )

# Unit tests

juce_add_console_app(UnitTests
        VERSION 1.1.1
        )

# Test source files
target_sources(UnitTests PUBLIC
        source/segmentation/OnsetDetectionTest.cpp
        source/feature-extraction/MfccExtractionTest.cpp
        )

target_compile_definitions(UnitTests PUBLIC
        JUCE_STANDALONE_APPLICATION=1 # Silence warning in juce_core
        )

# Libraries needed to run test files
target_link_libraries(UnitTests PRIVATE
        gtest
        gtest_main
        segmentation
        utils
        mfcc
        juce::juce_core
        juce::juce_dsp
        juce::juce_audio_basics
        )

