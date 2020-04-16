#include <iostream>
#include <math.h>
#include <sndfile.h>
#include "wav.h"
#include "keyvalue.h"
#include "sndfile.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Wav::Wav(const std::string &param)
    : adsr(SamplingRate, param)
{
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
 
  static string kv_null;
  if ((file_name = kv("file")) == kv_null)
  {
    cerr << "Error: no se ha encontrado el campo con el fichero" << endl;
    throw - 1;
  }

  readcount = 0; //number of data in the buffer
  inFile = sf_open(file_name.c_str(), SFM_READ, &inFileInfo);

}

void Wav::command(long cmd, long note, long vel)
{
  if (cmd == 9)
  { //'Key' pressed: attack begin
    bActive = true;
    //adsr.start();
  }/*
  else if (cmd == 8)
  { //'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0)
  { //Sound extinguished without waiting for release to end
    adsr.end();
  }*/
}

const vector<float> &Wav::synthesize()
{
  /*if (not adsr.active())
  {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;
    
  adsr(x); //apply envelope to x and update internal status of ADSR
*/
  static double *data = new double[BSIZE];
  readcount = sf_read_double(inFile, data, BSIZE);
  if(readcount > 0)
  {
    for (int k = 0; k < readcount; k++)
    {
      x[k] = data[k]; //put data in the array
    }
  }else{
    x.assign(x.size(), 0);
    bActive = false;
    sf_close(inFile);
  }
  
  return x;
}
