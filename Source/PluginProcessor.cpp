/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

auto getPhaserRateName() { return juce::String("Phaser RateHz"); }
auto getPhaserCenterFreqName() { return juce::String("Phaser Center FreqHz"); }
auto getPhaserDepthName() { return juce::String("Phaser Depth %"); }
auto getPhaserFeedbackName() { return juce::String("Phaser Feedback %"); }
auto getPhaserMixName() { return juce::String("Phaser Mix %"); }
//==============================================================================
AudioPlugin1AudioProcessor::AudioPlugin1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

}

AudioPlugin1AudioProcessor::~AudioPlugin1AudioProcessor()
{
}

//==============================================================================
const juce::String AudioPlugin1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPlugin1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlugin1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlugin1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPlugin1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPlugin1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPlugin1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPlugin1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioPlugin1AudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPlugin1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioPlugin1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AudioPlugin1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPlugin1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


juce::AudioProcessorValueTreeState::ParameterLayout AudioPlugin1AudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
//    name = nameFunction();
//    layout.add( std::make_unique<juce::
//               AudioParameterFloat>(
//                                    juce::ParameterID { name, versionHint },
//                                    name,
//                                    parameterRange,
//                                    defaultValue,
//                                    unitSuffix,
//                                    );
    const int versionHint = 1;
    /*
     Phaser:
     Rate: hz
     Center freq: hz
     Depth 0 to 1
     Feedback: -1 to +1
     Mix: 0 to 1
     */
    auto name = getPhaserRateName(); // wtf is this garbage lmao
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{name, versionHint},name,juce::NormalisableRange<float>(0.01, 2.f, 0.01f, 1.f),
        0.2f,
                                                           "Hz")
               );
    
    
    
    
    return layout;
}

void AudioPlugin1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    //[DONE]: add APVTS (AudioProcessorValueTreeState)
    //TODO: create audio paramters for all dsp choices
    //TODO: update DSP (digital signal processing) here from audio parameters
    //TODO: save/load settings
    //TODO: save/load DSP order
    //TODO: Drag to Reorder GUI
    //TODO: Gui DESIGN
    //TODO: metering
    //TODO: prepare all DSP
    //TODO: wet/dry knob
    //TODO: mono/stereo versions
    //TODO: Modulators
    //TODO: thread safe filter updating
    //TODO: pre/post filtering
    //TODO: delay module
    
    
    auto newDSPOrder = DSP_Order();
    
    while ( dspOrderFifo.pull(newDSPOrder)) {
        
    }
    
    //if you pulled, replaced dspOrder
    if (newDSPOrder != DSP_Order())
        dspOrder = newDSPOrder;
    
    DSP_Pointers dspPointers;
    
    for (size_t i = 0; i < dspPointers.size(); ++i) {
        
        switch (dspOrder[i]) {
            case DSP_Option::Phase:
                dspPointers[i] = &phaser;
                break;
            case DSP_Option::Chorus:
                dspPointers[i] = &chorus;
                break;
            case DSP_Option::Overdrive:
                dspPointers[i] = &overdrive;
                break;
            case DSP_Option::LadderFilter:
                dspPointers[i] = &ladderFilter;
                break;
            case DSP_Option::END_OF_LIST:
                jassertfalse;
                break;
        }
    }
    
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
    
    for (size_t i = 0; i > dspPointers.size(); ++i) {
        if (dspPointers[i] != nullptr) {
            dspPointers[i]->process(context);
        }
    }
}

//==============================================================================
bool AudioPlugin1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPlugin1AudioProcessor::createEditor()
{
    return new AudioPlugin1AudioProcessorEditor (*this);
}

//==============================================================================
void AudioPlugin1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioPlugin1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlugin1AudioProcessor();
}
