#include "MainComponent.h"

using namespace juce;

#pragma region Lifecycle

MainComponent::MainComponent()
    : synthAudioSource(keyboardState)
    , keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    Init();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::Init()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    const int NumInputChannels = 0;
    const int NumOutputChannels = 2;
    setAudioChannels(NumInputChannels, NumOutputChannels);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    addAndMakeVisible(keyboardComponent);
    
    InitUI();
    
    startTimer(400);
}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

#pragma endregion

#pragma region Audio

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

#pragma endregion

#pragma region GUI

void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    keyboardComponent.setBounds(10, getHeight() - getHeight() * 0.2, getWidth() - 20, getHeight() * 0.2);
    midiInputsList.setBounds (100, 10, getWidth() * 0.5, 30);
}

void MainComponent::InitUI()
{
    // Configure UI
    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("MIDI Input: ", dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputsList, true);
    
    Array<String> midiInputsNames;
    
    const Array<MidiDeviceInfo>& midiInputs = juce::MidiInput::getAvailableDevices();
    for (const MidiDeviceInfo& midiInput : midiInputs)
    {
        midiInputsNames.add(midiInput.name);
    }
    
    midiInputsList.addItemList(midiInputsNames, 1);
    midiInputsList.onChange = [&](){ OnMIDISelectionChanged(); };
    
    addAndMakeVisible(midiInputsList);
    midiInputsList.setTextWhenNoChoicesAvailable("No Midi Inputs Enabled");
    
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled (input.identifier))
        {
            SetMidiInput(midiInputs.indexOf (input));
            break;
        }
    }
}

void MainComponent::OnMIDISelectionChanged()
{
    SetMidiInput(midiInputsList.getSelectedItemIndex());
}

#pragma endregion

#pragma region MIDI

void MainComponent::SetMidiInput(const int Idx)
{
    const Array<MidiDeviceInfo>& devices = MidiInput::getAvailableDevices();
    
    // Disconnect previous device
    deviceManager.removeMidiInputDeviceCallback(devices[lastInputIndex].identifier, synthAudioSource.getMIDICollector());
    
    // Set new device
    const MidiDeviceInfo& input = devices[Idx];
    
    if (!deviceManager.isMidiInputDeviceEnabled(input.identifier))
    {
        deviceManager.setMidiInputDeviceEnabled(input.identifier, true);
    }
    
    deviceManager.addMidiInputDeviceCallback(input.identifier, synthAudioSource.getMIDICollector());
    midiInputsList.setSelectedId(Idx + 1, dontSendNotification);
    
    lastInputIndex = Idx;
}

#pragma endregion
