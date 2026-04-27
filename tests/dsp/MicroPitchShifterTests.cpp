#include <dsp/MicroPitchShifter.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <juce_dsp/juce_dsp.h>

#include <cmath>

namespace
{
    juce::dsp::ProcessSpec makeSpec (double sampleRate = 44100.0,
                                     int    blockSize  = 512)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate       = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32> (blockSize);
        spec.numChannels      = 1;
        return spec;
    }
}

TEST_CASE ("MicroPitchShifter:output stays bounded for white noise input", "[dsp][shifter]")
{
    MicroPitchShifter<float> shifter;
    shifter.prepare (makeSpec());
    shifter.setCents (10.0f);

    juce::Random rng { 1234 };

    float peak = 0.0f;
    for (int i = 0; i < 44100; ++i)
    {
        const float in  = rng.nextFloat() * 2.0f - 1.0f;
        const float out = shifter.processSample (in);
        peak = std::max (peak, std::abs (out));
    }

    // Equal-power crossfade of two interpolated samples can briefly exceed
    // the input's peak. 1.5x is well within reason for [-1,1] white noise.
    REQUIRE (peak < 1.5f);
}

TEST_CASE ("MicroPitchShifter:DC input remains DC after settling", "[dsp][shifter]")
{
    MicroPitchShifter<float> shifter;
    shifter.prepare (makeSpec());
    shifter.setCents (7.0f);

    constexpr float dcLevel = 0.5f;

    // Prime the buffer with several full traversals so the read heads are
    // both pulling steady-state DC.
    for (int i = 0; i < 8192; ++i)
    {
        shifter.processSample (dcLevel);
    }

    for (int i = 0; i < 1024; ++i)
    {
        const float out = shifter.processSample (dcLevel);
        REQUIRE_THAT (out, Catch::Matchers::WithinAbs (dcLevel, 1.0e-3f));
    }
}

TEST_CASE ("MicroPitchShifter:shifts a sine wave's frequency", "[dsp][shifter]")
{
    constexpr double sampleRate = 44100.0;
    constexpr float  inputFreq  = 440.0f;   // A4
    constexpr float  cents      = 100.0f;   // one semitone up
    constexpr float  expected   = inputFreq * 1.05946309f; // 2^(1/12)

    MicroPitchShifter<float> shifter;
    shifter.prepare (makeSpec (sampleRate));
    shifter.setCents (cents);

    // Warm up past the buffer-fill latency.
    for (int i = 0; i < 8192; ++i)
    {
        const float in = std::sin (2.0f * juce::MathConstants<float>::pi
                                   * inputFreq
                                   * static_cast<float> (i)
                                   / static_cast<float> (sampleRate));
        shifter.processSample (in);
    }

    // Count zero-crossings over a 1-second window to estimate output frequency.
    int   crossings = 0;
    float prev      = 0.0f;
    for (int i = 0; i < static_cast<int> (sampleRate); ++i)
    {
        const float in = std::sin (2.0f * juce::MathConstants<float>::pi
                                   * inputFreq
                                   * static_cast<float> (i)
                                   / static_cast<float> (sampleRate));
        const float out = shifter.processSample (in);

        if ((prev <= 0.0f) != (out <= 0.0f))
        {
            ++crossings;
        }
        prev = out;
    }

    // 2 zero crossings per cycle → freq ≈ crossings/2.
    const float measured = static_cast<float> (crossings) * 0.5f;

    // Allow ±2 Hz tolerance:small chorus modulation from the wrap rate adds
    // a tiny amount of jitter to the perceived pitch.
    REQUIRE_THAT (measured, Catch::Matchers::WithinAbs (expected, 2.0f));
}
