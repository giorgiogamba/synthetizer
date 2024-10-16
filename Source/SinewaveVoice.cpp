/*
  ==============================================================================

    SincewaveVoice.cpp
    Created: 7 Oct 2024 11:14:17pm
    Author:  Giorgio Gamba

  ==============================================================================
*/

#include "SinewaveVoice.h"

using namespace juce;
using namespace std;

SinewaveVoice::SinewaveVoice()
{
    currentAngle = 0.0;
    angleDelta = 0.0;
    level = 0.0;
    tailOff = 0.0;
}

bool SinewaveVoice::canPlaySound(SynthesiserSound* sound)
{
    return dynamic_cast<SinewaveSound*> (sound) != nullptr;
}

void SinewaveVoice::startNote(const int midiNoteNumber, const float velocity, SynthesiserSound* sound, int wheelPitchPosition)
{
    // Make start the sine from the beginning in (0, 0)
    currentAngle = 0.0;
    level = velocity * 0.25;
    tailOff = 0.0;
    
    double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    double cyclesPerSample = cyclesPerSecond / getSampleRate();
    
    // Compute the distance between two following points on the wave
    angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
}

void SinewaveVoice::stopNote(const float velocity, const bool bAllowTailOff)
{
    if (bAllowTailOff && tailOff == 0.0)
    {
        tailOff = 1.0;
    }
    else
    {
        clearCurrentNote();
        angleDelta = 0.0;
    }
}
