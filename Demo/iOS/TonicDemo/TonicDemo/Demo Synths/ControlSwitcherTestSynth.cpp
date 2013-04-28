//
//  ControlInputTestSynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 4/15/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef TonicDemo_ControlInputTestSynth_cpp
#define TonicDemo_ControlInputTestSynth_cpp

#include "Synth.h"
#include "RectWave.h"
#include "Filters.h"
#include "SineWave.h"
#include "ControlXYSpeed.h"
#include "StereoDelay.h"
#include "ControlMetro.h"
#include "ADSR.h"
#include "ControlSwitcher.h"
#include "ControlCounter.h"
#include "ControlStepper.h"
#include "ControlRandom.h"
#include "Filters.h"
#include "RectWave.h"
#include <functional>

using namespace Tonic;

class ControlSwitcherTestSynth : public Synth{

public:
  ControlSwitcherTestSynth(){
  
    ControlMetro metro = ControlMetro().bpm(80 * 4);
    ControlGenerator modeSwitch =  ControlMetro().bpm(4);
    
//    const int numSteps = 16;
//    ControlGenerator phraseLen = ControlRandom().min(2).max(10).trigger(modeSwitch);
//    ControlGenerator phraseStart = ControlRandom().min(0).max(10).trigger(modeSwitch);
//    ControlCounter step = ControlCounter()
//      .trigger(metro)
//      .end(
//        ControlSwitcher()
//        .addInput(numSteps)
//        .addInput( ControlRandom().min(2).max(10).trigger(modeSwitch) )
//        .inputIndex(
//          ControlCounter()
//          .end(2)
//          .trigger(modeSwitch)
//          -1
//          )
//       -1);
    
    
    const int numSteps = 20;
    ControlGenerator phraseLen = ControlRandom().min(3).max(4).trigger(modeSwitch);
    ControlGenerator phraseStart = ControlRandom().min(0).max(5).trigger(modeSwitch);
    ControlStepper step = ControlStepper()
      .start(phraseStart)
      .end(phraseStart + phraseLen)
      .trigger(metro);
    
    ControlGenerator clickVol = addParameter("clickVol");
    ADSR clickEnv =  ADSR(0, 0.01, 0, 0).trigger(metro);
    Generator clickIntensity = (1 + clickVol).ramped();
    Generator click = SineWave().freq(300 + 1000 * clickEnv ) * clickEnv * 0.03 ;
    
    ControlSwitcher fmAmount = ControlSwitcher().inputIndex(step);
    ControlSwitcher sustain = ControlSwitcher().inputIndex(step);
    ControlSwitcher decay = ControlSwitcher().inputIndex(step);
    ControlSwitcher spreadSeq = ControlSwitcher().inputIndex(step);
    
    for(int i = 0; i < numSteps; i++){
      fmAmount.addInput(randomFloat(0, 10));
      sustain.addInput(randomFloat(0, 0.7));
      decay.addInput(randomFloat(0.03, 0.1));
      spreadSeq.addInput(randomFloat(0, 0.5));
    }
    
    Generator freq = ControlValue(50).ramped();
    Generator tremelo =  1 + ( SineWave().freq(15) *  ADSR(0, 0.5, 0,0).trigger(modeSwitch) );
    Generator bassEnv = ADSR(0.001, 0.1 ,0,0).decay(decay).legato(true).sustain(sustain * sustain).trigger(metro);
    ControlGenerator spread = ControlRandom().min(0).max(0.5).trigger(modeSwitch);// * spreadSeq;
    ControlGenerator wave = ControlRandom().min(0.4).max(0.9).trigger(modeSwitch);
    
    auto makeBass = [&](Generator bassFreq){
      return RectWave()
      .pwm(wave + 0.04 * bassEnv)
      .freq(
        bassFreq
        + bassFreq
          * SineWave()
            .freq(bassFreq * 1.99)
          * (
            fmAmount
            * 0.7
            + addParameter("addtlFM")
            ).ramped()
      );
    };
    
    Generator bass =
    (
      makeBass(freq)
      >> MonoToStereoPanner().pan(-1 * spread)
    )
    +
    (
      makeBass(freq * 1.02)
      >> MonoToStereoPanner().pan(1 * spread)
    )
      
     // >> HPF12().cutoff(addParameter("hpf") * 1000)
      >> LPF12().cutoff(5500)
      >> StereoDelay(1.1, 1.2).mix(0.1) ;
    Generator bassWithAmp = bass * bassEnv * tremelo;
    outputGen = bassWithAmp  + click;
  }
};

registerSynth(ControlSwitcherTestSynth)

#endif
