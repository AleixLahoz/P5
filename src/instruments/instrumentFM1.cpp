#include <iostream>
#include <math.h>
#include "instrumentFM1.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM1::InstrumentFM1(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */

  float fm;
  float I;

  KeyValue kv(param);
  if (!kv.to_float("I",I))
    I = 1.0;
  if (!kv.to_float("fm",fm)){
    fm = 8.0;
  }
   
  fm1 = fm;
  I1 = I;


  //if (!kv.to_int("N1",N1))
   // N1 = 1;
  //if (!kv.to_int("N2",N2))
    //N2 = 1;
  
  //Create a tbl with one period of a sinusoidal wave
}


void InstrumentFM1::command(long cmd, long note, long vel) {
  float exp;
  float f0;
  float FM;

  if (cmd == 9) {   //'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    phase = 0;
    exp = ((float)note - 69.0)/12.0;
    f0 = 440.0 * pow(2.0,exp);
    F0 = f0/SamplingRate;
    step = 2*M_PI*F0;

    cout << "f0 = " << f0 << " fm = " << fm1 << "\n";

    phase_vib = 0;
    //fm = f0*(N2/N1);
    FM = fm1/SamplingRate;
    step_vib = 2*M_PI*FM;
    
    A = 2*(vel / 127.);
   
  }
  else if (cmd == 8) {  //'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {  //Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentFM1::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A*sin(phase + I1*sin(phase_vib));
    //cout << "I= "<< I << "fm= "<< fm << "\n"; 

    phase += step;
    phase_vib += step_vib;
    while ((phase + I1*sin(phase_vib)) > 2*M_PI) phase -= 2*M_PI;
    while (phase_vib > 2*M_PI) phase_vib -= 2*M_PI;  
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
