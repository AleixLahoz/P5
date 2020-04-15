#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Seno::Seno(const std::string &param)
    : adsr(SamplingRate, param)
{
  bActive = false; //Declaramos el instrumento INACTIVO
  x.resize(BSIZE); //Ubicamos el espacio para almacenar un Tick de señal

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  int N; //Parámetro de interes --> Longitud de la tabla N

  if (!kv.to_int("N", N))
    N = 40; //default value

  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI / (float)N;
  index = 0.0;
  indexStep = 0.0;
  for (int i = 0; i < N; ++i)
  {
    tbl[i] = sin(phase);
    phase += step;
  }
}

//Para cada linea del fichero MIDI, tomamos --> Comando / Nota / Actividad
void Seno::command(long cmd, long note, long vel)
{
  if (cmd == 9)
  {                 //'Key' pressed: attack begins
    bActive = true; //Declaramos activo el instrumento
    adsr.start();   //Iniciamos curva ADSR
    index = 0;      //Contador para recorrer la tabla
    A = vel / 127.;
    float exp = ((float)note - 69.0) / 12.0;
    f0 = 440.0 * pow(2.0, exp);
    indexStep = (tbl.size() * f0) / SamplingRate;
    //cout << indexStep << '\n';
  }
  else if (cmd == 8)
  { //'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0)
  { //Sound extinguished without waiting for release to end
    adsr.end();
  }
}

const vector<float> &Seno::synthesize()
{
  //Si la nota ya ha llegado a su FINAL
  if (not adsr.active())
  {
    x.assign(x.size(), 0); //Asignamos a la señal sintetizada (vector x) el valor 0
    bActive = false;       //Marcamos la nota como inactiva
    return x;
  } //Estado bug oeoeoe
  else if (not bActive)
    return x;
  //Nota ACTIVA --> Realizamos sinstesis y aplicamos envolvente ADSR  
  for (unsigned int i = 0; i < x.size(); ++i)
  {

    if ((unsigned int) ceil(index) == tbl.size())
    {
      x[i] = A * (((1.0 - (index - floor(index))) * tbl[floor(index)]) + ((1.0 - (ceil(index) - index)) * tbl[0]));
    }
    else
    {
      x[i] = A * (((1.0 - (index - floor(index))) * tbl[floor(index)]) + ((1.0 - (ceil(index) - index)) * tbl[ceil(index)]));
    }
    
    index = fmod(index + indexStep, tbl.size());
 
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
