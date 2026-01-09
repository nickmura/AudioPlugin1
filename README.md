# AudioPlugin1 TODOs

This README summarizes the TODO list in `Source/PluginProcessor.cpp` and adds more detail for the next steps, especially the missing audio parameters for the DSP choices.

## Priority next steps

### 1) Create audio parameters for all DSP choices

Right now only Phaser parameters exist. Add parameter groups (and matching `AudioParameterFloat*` pointers + lookup in the constructor) for each DSP choice in the `DSP_Option` enum:

- **Chorus** (in `juce::dsp::Chorus<float>`)
  - Rate (Hz)
  - Depth (0..1)
  - Centre delay (ms)
  - Feedback (0..1)
  - Mix (0..1)

- **Overdrive** (likely a gain + waveshaper chain)
  - Drive (gain or dB)
  - Pre-gain / input trim (optional)
  - Output level (optional)
  - Mix (0..1)

- **LadderFilter** (in `juce::dsp::LadderFilter<float>`)
  - Cutoff (Hz)
  - Resonance (0..1 or 0..4 depending on JUCE range)
  - Drive (0..1)
  - Mode (LP/HP/BP) as a `AudioParameterChoice`
  - Mix (optional if filter is in parallel)

Notes:
- Keep parameter IDs unique; follow the Phaser pattern with helper name functions.
- Add the new parameters in `createParameterLayout()` and cache pointers in the processor constructor like the Phaser block.
- If you plan to expose per-DSP bypass or slot enable, add a `bool` or `choice` per DSP module.

### 2) Update DSP from audio parameters in processBlock

Map the APVTS parameters to the DSP modules each block:
- Read params (atomic reads if using `AudioParameterFloat*`).
- Set DSP values on each effect (`phaser`, `chorus`, `overdrive`, `ladderFilter`).
- If you need smoothing, add `juce::SmoothedValue` for key parameters.

### 3) Save/load settings

Implement `getStateInformation()` / `setStateInformation()` with APVTS XML:
- `apvts.copyState()` -> XML -> `destData`.
- `apvts.replaceState()` from XML.

### 4) Save/load DSP order

Persist the `DSP_Order` (enum array) alongside APVTS state.
- Serialize order to a string or integer array in the state XML.
- Restore into `dspOrder` and push into `dspOrderFifo` on load.

### 5) Drag-to-reorder GUI

Build a UI list for the DSP chain with drag handles that edits the `DSP_Order` and pushes into `dspOrderFifo`.

### 6) GUI design

Apply layout, labels, and modules for each effect. Decide if you want per-module panels or a unified strip.

### 7) Metering

Add input/output RMS + peak meters (likely in editor, fed by the processor).

### 8) Prepare all DSP

Ensure every DSP module is prepared in `prepareToPlay()` (already done for the current four). Add any future modules here too.

### 9) Wet/dry knob

Add a global mix control in the APVTS and blend processed and dry signal in `processBlock()`.

### 10) Mono/stereo versions

Decide on channel layouts or separate plugin configs. Verify `isBusesLayoutSupported()`.

### 11) Modulators

If you want LFOs/envelopes, define modulators and a routing system (per parameter mapping).

### 12) Thread-safe filter updating

Use parameter smoothing or atomics; avoid calling heavy updates from the audio thread.

### 13) Pre/post filtering

Add optional filter modules before/after the chain, with their own parameters.

### 14) Delay module

Add a delay DSP module plus its parameter set, and update the `DSP_Option` enum and chain.

## Where these TODOs live

- TODO list location: `Source/PluginProcessor.cpp`
- DSP enum and APVTS location: `Source/PluginProcessor.h`

