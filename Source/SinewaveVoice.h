/*
  ==============================================================================

    SincewaveVoice.h
    Created: 7 Oct 2024 11:14:17pm
    Author:  Giorgio Gamba

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SinewaveSound.h"

class SinewaveVoice : public juce::SynthesiserVoice
{
    
public:
    
    SinewaveVoice();
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    
    void startNote(const int midiNoteNumber, const float velocity, juce::SynthesiserSound* sound, int wheelPitchPosition) override;
    void stopNote(const float velocity, const bool bAllowTailOff) override;
    
    // #TODO implement render next block
    
private:
    
    double currentAngle;
    
    /** Defines the space between two following points on the sinewave */
    double angleDelta;
    
    double level;
    
    double tailOff;
    
};
