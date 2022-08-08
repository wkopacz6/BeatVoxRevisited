add_library(mfcc INTERFACE)

target_sources(mfcc INTERFACE
        MfccExtraction.cpp
        MfccExtraction.h
        )

