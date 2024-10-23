/*
  ==============================================================================

    SynthAudioSource.cpp
    Created: 7 Oct 2024 11:05:25pm
    Author:  Giorgio Gamba

  ==============================================================================
*/

#include "AudioSource.h"

SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState)
    : keyboardState(keyState)
{
    for (int i = 0; i < 4; ++i)
    {
        synth.addVoice(new SinewaveVoice());
    }
    
    synth.addSound(new SinewaveSound());
}

void SynthAudioSource::prepareToPlay(int samplesPerblockExpected, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiCollector.reset(sampleRate);
}

void SynthAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    MidiBuffer incomingMIDI;
    midiCollector.removeNextBlockOfMessages(incomingMIDI, bufferToFill.numSamples);
    
    keyboardState.processNextMidiBuffer(incomingMIDI, bufferToFill.startSample, bufferToFill.numSamples, true);
    
    synth.renderNextBlock(*bufferToFill.buffer, incomingMIDI, bufferToFill.startSample, bufferToFill.numSamples);
}


