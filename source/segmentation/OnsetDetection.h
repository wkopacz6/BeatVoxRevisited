//
// Created by Walter Kopacz on 2/10/22.
//
#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_dsp/juce_dsp.h"
#include "../utils/Utils.h"

// Constants
static constexpr size_t kSampleRate = 44100;

class OnsetDetection
{
public:
    OnsetDetection ();

    ~OnsetDetection () = default;

    std::vector<float> detectOnsets (const juce::AudioBuffer<float>& audio);

private:
    static constexpr auto mFftOrder = 10; // The order of the fft; nfft = 2^order
    static constexpr auto mHopLength = 768;
    std::vector<float> mHannWindow{};
    std::vector<float> mMedianFilterVec;

    juce::dsp::FFT mFFT;

    std::vector<float> mNoveltyFunction{};
    std::vector<float> mAdaptiveThreshold{};
    std::vector<float> mPeaks{};

    void extractNoveltyFunction (const juce::AudioBuffer<float>& audio);

    bool checkAboveAdaptiveThreshold (size_t idx);

    void pickPeaks ();
};
