//
// Created by Walter Kopacz on 7/31/22.
//
#pragma once

#include "juce_dsp/juce_dsp.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include "../utils/Utils.h"

class MfccExtraction
{
private:

    // Hertz to Mel conversion
    inline double hz2mel (double f)
    {
        return 2595 * std::log10 (1 + f / 700);
    }

    // Mel to Hertz conversion
    inline double mel2hz (double m)
    {
        return 700 * (std::pow (10, m / 2595) - 1);
    }

    // Apply hann window to current frame
    void applyHannWindow (void);

    // Compute power spectrum
    void computePowerSpectrum (void);

    void generateMelFilterbanks (void);

    void applyFilterbanks (void);

    void generateDct (void);

    void applyDct (void);


    juce::dsp::FFT mFft;
    // Frames gotten from segmentation need to be the same size as number of points in our FFT and mCurrent frame
    std::vector<float> mCurrentFrame;
    std::vector<float> mPowerSpectrum;
    const std::vector<float> mHannMultiplier;
    std::vector<std::vector<float>> mFilterBanks;
    std::vector<std::vector<float>> mDct;
    std::vector<float> mFbankCoeffs;
    std::vector<float> mMfccs;


    const size_t mFs;
    const size_t mNumFft;
    const size_t mNumMfccs;
    const size_t mNumFilters;

public:
    MfccExtraction (int fftOrder, size_t samplingRate, size_t numMfccs, size_t numFilters);

    std::vector<float> extractFrame (std::vector<float>& data);

    void extract ();

};


