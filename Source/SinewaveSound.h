/*
  ==============================================================================

    SinewaveSound.h
    Created: 7 Oct 2024 11:19:28pm
    Author:  Giorgio Gamba

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SinewaveSound : public juce::SynthesiserSound
{
    SinewaveSound() {}
    
    bool appliesToNote (const int val) override;
    bool appliesToChannel(const int val) override;
};
