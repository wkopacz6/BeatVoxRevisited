//
// Created by Walter Kopacz on 2/23/22.
//
#pragma once

#import "juce_audio_basics/juce_audio_basics.h"

namespace util
{
    static constexpr float kPi = 3.141592653589793;

    juce::AudioBuffer<float> absMaxNormalizeAudioBuffer (const juce::AudioBuffer<float>& audio);

    std::vector<float> createHannWindowVector (size_t fftSize);

    template<typename T>
    void convertTimestampsToSamples (T* timestamps, unsigned int sampleRate)
    {
        if (timestamps)
        {
            for (auto i = 0; i < sizeof (*timestamps) / sizeof (T); i++)
            {
                timestamps[i] = timestamps[i] * sampleRate;
            }
        }
    }

    template<typename T>
    void convertIndicesToTimestamps (T* samples, unsigned int sampleRate, unsigned int hopLength)
    {
        if (samples)
        {
            for (auto i = 0; i < sizeof (*samples) / sizeof (T); i++)
            {
                samples[i] = samples[i] * hopLength / sampleRate;
            }
        }
    }

    size_t pow2 (unsigned exp)
    {
        auto result = 1;
        for (unsigned i = 0; i < exp; ++i)
            result *= 2;

        return result;
    }

}
