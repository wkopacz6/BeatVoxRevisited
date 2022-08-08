//
// Created by Walter Kopacz on 6/5/22.
//
#pragma once

#include <gtest/gtest.h>
#include "OnsetDetection.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include <vector>

// TODO: Add more unit tests. Perhaps modify class to allow for more thorough unit testing. Maybe something like this
//  (https://github.com/alexanderlerch/libACA/blob/main/src/Tests/Tests/Test_Novelty.cpp). For now check to make sure does not detect any onsets for sine tone.

TEST (OnsetDetectionTest, TestOnsetDetection)
{
    auto detector = OnsetDetection ();

    // Create Audio Buffer containing two sine waves of different freqs
    auto buffer = juce::AudioBuffer<float> (1, 2 * 44100);
    for (auto i = 0; i < (2 * 44100); ++i)
    {
        buffer.setSample (0, 0, std::sin (2 * juce::MathConstants<float>::pi * 1000 * i / 44100));
    }

    auto onsets = detector.detectOnsets (buffer);
    ASSERT_TRUE(onsets.size () == 0);
}