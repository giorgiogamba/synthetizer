#include "MainComponent.h"

using namespace juce;

#pragma region Lifecycle

MainComponent::MainComponent()
    : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
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

    /*// Some platforms require permissions to open input channels so request that here
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
    }*/
    
    addAndMakeVisible(keyboardComponent);
    
    InitUI();
}

#pragma endregion

#pragma region Audio

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
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
    //midiInputList.onChange = [](){/* set midi input */};
    
    addAndMakeVisible(midiInputsList);
    midiInputsList.setTextWhenNoChoicesAvailable("No Midi Inputs Enabled");
}

#pragma endregion
