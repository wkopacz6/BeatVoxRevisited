//
// Created by Walter Kopacz on 2/10/22.
//

#include "OnsetDetection.h"

OnsetDetection::OnsetDetection () : mFFT (mFftOrder),
                                    mMedianFilterVec (5, 0)
{
    mHannWindow = util::createHannWindowVector (mFFT.getSize ());
}

void OnsetDetection::extractNoveltyFunction (const juce::AudioBuffer<float>& audio)
{
    auto normalizedAudio = util::absMaxNormalizeAudioBuffer (audio);
    auto audioReadPtr = normalizedAudio.getReadPointer (0);

    // Intermediate values for calculating RMS
    float squared;
    float mean;
    float sum = 0.0f;

    // Store the max flux val for absmax normalization at the end.
    float maxFluxVal = 0.0f;

    // Allocate space for all the FFT data
    auto numOfFFTs = 1 + static_cast<size_t>((audio.getNumSamples () - mFFT.getSize ()) / mHopLength);
    mNoveltyFunction.resize (numOfFFTs - 1);
    // Zero padding for median filter
    mAdaptiveThreshold.resize (mNoveltyFunction.size () + 4);
    std::vector<float> currentFft (mFFT.getSize () * 2);
    std::vector<float> previousFft (mFFT.getSize () * 2);

    // Get FFTs
    for (auto i = 0; i < numOfFFTs; i++)
    {
        for (auto j = 0; j < mFFT.getSize (); j++)
        {
            currentFft[i] = mHannWindow[j] * audioReadPtr[j + i];
        }

        mFFT.performFrequencyOnlyForwardTransform (currentFft.data ());

        // Convert to dB
        for (auto& j: currentFft)
        {
            j = 20 * log10f (j + 1e-12f);
        }

        // If first frame, we can't compute difference so store the current fft and continue
        if (i == 0)
        {
            previousFft = currentFft;
            continue;
        }

        // Take difference but do not allow difference < 0 (halfwave rectification)
        for (auto j = 0; j < mNoveltyFunction.size (); j++)
        {
            if (currentFft[j] - previousFft[j] < 0)
                squared = 0;
            else
                // Square for RMS
                squared = (currentFft[j] - previousFft[j]) * (currentFft[j] - previousFft[j]);
            sum += squared;
            // Copy currentFft val into previousFft val
            previousFft[j] = currentFft[j];
        }
        // Mean for RMS
        mean = sum / static_cast<float>(mNoveltyFunction.size ());

        // Sqrt for RMS and store
        mNoveltyFunction[i] = sqrt (mean);
        if (mNoveltyFunction[i] > maxFluxVal)
            maxFluxVal = mNoveltyFunction[i];
    }
    // Normalize novelty function.
    for (auto i = 0; i < mNoveltyFunction.size (); i++)
    {
        mNoveltyFunction[i] /= maxFluxVal;
        // Save novelty funtion values in zero padded adaptive threshold
        mAdaptiveThreshold[i + 2] = mNoveltyFunction[i];
    }
}

void OnsetDetection::pickPeaks ()
{
    for (auto i = 0; i < mNoveltyFunction.size (); i++)
    {
        if (
                mNoveltyFunction[i] > mNoveltyFunction[i - 1] &&
                mNoveltyFunction[i] > mNoveltyFunction[i + 1] &&
                checkAboveAdaptiveThreshold (i)
                )
        {
            mPeaks.emplace_back (i);
            // Skip ahead 3 indicies to avoid picking false peaks
            i += 3;
        }
    }
}

bool OnsetDetection::checkAboveAdaptiveThreshold (size_t idx)
{
    // Fill median filter vec
    for (auto i = 0; i < 5; i++)
    {
        mMedianFilterVec[i] = mNoveltyFunction[idx - 2 + i];
    }

    std::sort (mMedianFilterVec.begin (), mMedianFilterVec.end ());
    if (mNoveltyFunction[idx] > mMedianFilterVec[2] + 0.2f)
        return true;
    else
        return false;
}

std::vector<float> OnsetDetection::detectOnsets (const juce::AudioBuffer<float>& audio)
{
    extractNoveltyFunction (audio);
    pickPeaks ();
    util::convertIndicesToTimestamps (mPeaks.data (), kSampleRate, mHopLength);
    util::convertTimestampsToSamples (mPeaks.data (), kSampleRate);
    return mPeaks;
}
