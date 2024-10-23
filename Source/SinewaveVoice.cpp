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

void SinewaveVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (angleDelta == 0.0)
        return;
    
    if (tailOff > 0.0)
    {
        HandleSoundWithTailOff(outputBuffer, startSample, numSamples);
    }
    else
    {
        HandleSoundWithoutTailOff(outputBuffer, startSample, numSamples);
    }
}

void SinewaveVoice::HandleSoundWithTailOff(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    while (--numSamples >= 0)
    {
        auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);
        
        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
            outputBuffer.addSample (i, startSample, currentSample);
        
        currentAngle += angleDelta;
        ++startSample;
        
        tailOff *= 0.99;
        
        if (tailOff <= 0.005)
        {
            clearCurrentNote();
            
            angleDelta = 0.0;
            break;
        }
    }
}

void SinewaveVoice::HandleSoundWithoutTailOff(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    while (--numSamples >= 0)
    {
        auto currentSample = (float) (std::sin (currentAngle) * level);
        
        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
            outputBuffer.addSample (i, startSample, currentSample);
        
        currentAngle += angleDelta;
        ++startSample;
    }
}

void SinewaveVoice::clearNote()
{
    clearCurrentNote();
    angleDelta = 0.0;
}
