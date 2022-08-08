//
// Created by Walter Kopacz on 8/6/22.
//
#pragma once

#include <gtest/gtest.h>
#include "MfccExtraction.h"
#include <vector>

TEST(MfccExtractionTest, TestMfccExtracts)
{
    auto mfccExtractor = MfccExtraction (10, 44100, 26, 40);

    // Create test vector
    auto testIn = std::vector<float> (512);

    auto results = mfccExtractor.extractFrame (testIn);
    ASSERT_EQ(results.size (), 26);
}
