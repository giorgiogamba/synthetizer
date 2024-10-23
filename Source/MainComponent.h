
#pragma region Includes

#pragma once

#include <JuceHeader.h>

#include "AudioSource.h"

#pragma endregion

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, private juce::Timer
{

#pragma region Lifecycle
    
public:
    
    MainComponent();
    ~MainComponent() override;
    
private:
    
    void Init();
    
    void timerCallback() override;
    
#pragma endregion
    
#pragma region Audio
    
public:

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
private:
    
    SynthAudioSource synthAudioSource;
    
#pragma endregion
    
#pragma region GUI

public:
    
    void paint (juce::Graphics& g) override;
    
    void resized() override;

private:
    
    void InitUI();
    
    void OnMIDISelectionChanged();
    
#pragma endregion

#pragma region Keyboard
    
private:
    
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

#pragma endregion
    
#pragma region MIDI

private:
    
    void SetMidiInput(const int Idx);
    
    juce::ComboBox midiInputsList;
    juce::Label midiInputListLabel;
    int lastInputIndex;

#pragma endregion

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
