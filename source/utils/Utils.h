//
// Created by Walter Kopacz on 2/23/22.
//

#import "juce_audio_basics/juce_audio_basics.h"

namespace util
{
    static constexpr float kPi = 3.141592653589793;

    template<typename T>
    void convertTimestampsToSamples (T* timestamps, unsigned int sampleRate)
    {
        for (auto i = 0; i < len (timestamps); i++)
        {
            timestamps[i] = timestamps[i] * sampleRate;
        }
    }

    template<typename T>
    void convertIndicesToTimestamps (T* samples, unsigned int sampleRate, unsigned int hopLength)
    {
        for (auto i = 0; i < len (samples); i++)
        {
            samples[i] = samples[i] * hopLength / sampleRate;
        }
    }

    juce::AudioBuffer<float> absMaxNormalizeAudioBuffer (const juce::AudioBuffer<float>& audio)
    {
        auto max = audio.getMagnitude (0, audio.getNumSamples ());
        auto output = juce::AudioBuffer<float> (audio);
        output.applyGain (1 / max);
        return output;
    }

    std::vector<float> createHannWindowVector (size_t fftSize)
    {
        std::vector<float> hannWindow (fftSize, 0);
        for (auto i = 0; i < fftSize; i++)
        {
            hannWindow.emplace_back (
                    0.5f * (1.0f - cos (2.0f * kPi * static_cast<float>(i) / static_cast<float>(fftSize))));
        }
        return hannWindow;
    }
}
