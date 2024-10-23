/*
  ==============================================================================

    SynthAudioSource.h
    Created: 7 Oct 2024 11:05:25pm
    Author:  Giorgio Gamba

  ==============================================================================
*/

#pragma region Includes

#pragma once

#include <JuceHeader.h>

#include "SinewaveVoice.h"
#include "SinewaveSound.h"

#pragma endregion

class SynthAudioSource : public juce::AudioSource
{
public:
    
    SynthAudioSource(juce::MidiKeyboardState& keyState);
    
    void prepareToPlay(int samplesPerblockExpected, double sampleRate) override;
    
    void releaseResources() override {}
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    
    juce::MidiMessageCollector* getMIDICollector() { return &midiCollector; }
    
private:
    
    juce::MidiKeyboardState& keyboardState;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
    
};
