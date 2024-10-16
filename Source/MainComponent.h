#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{

#pragma region Lifecycle
    
public:
    
    MainComponent();
    ~MainComponent() override;
    
private:
    
    void Init();
    
#pragma endregion
    
#pragma region Audio

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
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
