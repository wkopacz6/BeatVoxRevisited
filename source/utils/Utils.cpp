//
// Created by Walter Kopacz on 2/23/22.
//
#include "Utils.h"

juce::AudioBuffer<float> util::absMaxNormalizeAudioBuffer (const juce::AudioBuffer<float>& audio)
{
    auto max = audio.getMagnitude (0, 0, audio.getNumSamples ());
    auto output = juce::AudioBuffer<float> (audio);
    output.applyGain (1 / max);
    return output;
}

std::vector<float> util::createHannWindowVector (size_t fftSize)
{
    std::vector<float> hannWindow (fftSize, 0);
    for (auto i = 0; i < fftSize; i++)
    {
        hannWindow[i] =
                0.5f * (1.0f - std::cos (2.0f * kPi * static_cast<float>(i) / static_cast<float>(fftSize)));
    }
    return hannWindow;
}

size_t util::pow2 (unsigned int exp)
{
    auto result = 1;
    for (unsigned i = 0; i < exp; ++i)
        result *= 2;

    return result;
}