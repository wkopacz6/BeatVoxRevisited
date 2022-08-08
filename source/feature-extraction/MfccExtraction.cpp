//
// Created by Walter Kopacz on 7/31/22.
//

#include "MfccExtraction.h"

MfccExtraction::MfccExtraction (int fftOrder, size_t samplingRate, size_t numMfccs, size_t numFilters) :
        mFft (fftOrder),
        mFs (samplingRate),
        mNumFft (util::pow2 (fftOrder)),
        mHannMultiplier (util::createHannWindowVector (util::pow2 (fftOrder))),
        mCurrentFrame (util::pow2 (fftOrder) * 2), // Make this twice the size of the frame size for juce::fft
        mPowerSpectrum (util::pow2 (fftOrder) * 2),
        mNumMfccs (numMfccs),
        mNumFilters (numFilters),
        mMfccs (numMfccs),
        mFbankCoeffs (numFilters)
{
    generateMelFilterbanks ();
    generateMelFilterbanks ();
}

void MfccExtraction::applyHannWindow ()
{
    for (auto i = 0; i < mHannMultiplier.size (); ++i)
    {
        mCurrentFrame[i] *= mHannMultiplier[i];
    }
}

void MfccExtraction::computePowerSpectrum ()
{
    mFft.performFrequencyOnlyForwardTransform (mCurrentFrame.data ());
    for (auto i = 0; i < mPowerSpectrum.size (); ++i)
    {
        mPowerSpectrum[i] = std::abs (mCurrentFrame[i] * mCurrentFrame[i]);
    }
}

void MfccExtraction::generateMelFilterbanks ()
{
    float lowFreqMel, highFreqMel;
    lowFreqMel = hz2mel (0);
    highFreqMel = hz2mel (mFs / 2);

    // Calculate center filter freqs
    std::vector<float> centerFreqs (mNumFilters + 2);
    for (auto i = 0; i < centerFreqs.size (); ++i)
    {
        centerFreqs[i] = mel2hz (lowFreqMel + (highFreqMel - lowFreqMel) / (mNumFilters + 1) * i);
    }

    // Calculate bin freq
    size_t numFftBins = mNumFft / 2 + 1;
    std::vector<float> fftBins (numFftBins);
    for (auto i = 0; i < numFftBins; ++i)
    {
        fftBins[i] = (mFs / 2 / (numFftBins - 1)) * i;
    }

    // Allocate memory for filterbanks
    mFilterBanks.reserve (mNumFilters * numFftBins);

    // Populate filter banks
    float weight;
    std::vector<float> filtTemp (numFftBins);
    for (auto filt = 1; filt <= mNumFilters; ++filt)
    {
        for (auto bin = 0; bin < numFftBins; bin++)
        {
            if (fftBins[bin] < centerFreqs[filt - 1])
                weight = 0;
            else if (fftBins[bin] <= centerFreqs[filt])
                weight = (fftBins[bin] - centerFreqs[filt - 1]) / (centerFreqs[filt] - centerFreqs[filt - 1]);
            else if (fftBins[bin] <= centerFreqs[filt + 1])
                weight = (centerFreqs[filt + 1] - fftBins[bin]) / (centerFreqs[filt + 1] - centerFreqs[filt]);
            else
                weight = 0;
            filtTemp[bin] = weight;
        }
        mFilterBanks[filt - 1] = filtTemp;
    }
}

void MfccExtraction::generateDct ()
{
    std::vector<float> v1 (mNumMfccs), v2 (mNumFilters);
    for (auto i = 0; i < mNumMfccs; ++i)
        v1[i] = i;
    for (auto i = 0; i < mNumFilters; ++i)
        v2[i] = i + 0.5f;

    mDct.reserve (mNumFilters * (mNumMfccs + 1));
    double c = sqrt (2.0f / mNumFilters);
    for (auto i = 0; i <= mNumMfccs; ++i)
    {
        std::vector<float> dctTemp;
        for (auto j = 0; j < mNumFilters; ++j)
        {
            dctTemp.push_back (c * cos (util::kPi / mNumFilters * v1[i] * v2[j]));
        }
        mDct.push_back (dctTemp);
    }
}

void MfccExtraction::applyDct ()
{
    for (auto i = 0; i < mNumMfccs; ++i)
    {
        for (auto j = 0; j < mNumFilters; ++j)
            mMfccs[i] += mDct[i][j] * mFbankCoeffs[j];
    }
}

void MfccExtraction::applyFilterbanks ()
{
    for (auto i = 0; i < mNumFilters; ++i)
    {
        for (auto j = 0; j < mFilterBanks.size (); ++j)
            mFbankCoeffs[i] += mFilterBanks[i][j] * mPowerSpectrum[j];
        // Apply mel flooring
        if (mFbankCoeffs[i] < 1.0f)
            mFbankCoeffs[i] = 1.0f;
    }

    for (auto i = 0; i < mFbankCoeffs.size (); ++i)
        mFbankCoeffs[i] = std::log (mFbankCoeffs[i]);
}

std::vector<float> MfccExtraction::extractFrame (std::vector<float>& data)
{
    mCurrentFrame = std::vector<float> (data);

    applyHannWindow ();
    computePowerSpectrum ();
    applyFilterbanks ();
    applyDct ();

    return mMfccs;
}