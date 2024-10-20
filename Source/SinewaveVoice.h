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

using namespace juce;

class SinewaveVoice : public SynthesiserVoice
{
    
public:
    
    SinewaveVoice();
    
    bool canPlaySound(SynthesiserSound* sound) override;
    
    void startNote(const int midiNoteNumber, const float velocity, SynthesiserSound* sound, int wheelPitchPosition) override;
    void stopNote(const float velocity, const bool bAllowTailOff) override;
    void clearNote();
    
    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    
private:
    
    double currentAngle;
    
    /** Defines the space between two following points on the sinewave */
    double angleDelta;
    
    double level;
    
    double tailOff;
    
};
