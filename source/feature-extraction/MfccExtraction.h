//
// Created by Walter Kopacz on 7/31/22.
//

#include "juce_dsp/juce_dsp.h"
#include <algorithm>
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


    juce::dsp::FFT fft;
    std::vector<float> mCurrentFrame;
    const std::vector<float> mHannMultiplier;

    const size_t mFs;
    const size_t mNumFft;

public:
    MfccExtraction (int fftOrder, size_t samplingRate) :
            fft (fftOrder),
            mFs (samplingRate),
            mNumFft (util::pow2 (fftOrder)),
            mHannMultiplier (util::createHannWindowVector (util::pow2 (fftOrder)))
    {

    }

    void extractFrame (float* data);

    void extract ();

};


