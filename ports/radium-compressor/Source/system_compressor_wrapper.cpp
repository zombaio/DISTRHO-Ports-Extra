/* Copyright 2013 Kjetil S. Matheussen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */


#include <math.h>
#include <string.h>
#include <stdio.h>

#include "faudiostream/architecture/faust/gui/UI.h"
#include "faudiostream/architecture/faust/audio/dsp.h"

#include "system_compressor_wrapper_proc.h"



#include <vector>

inline int 	max (unsigned int a, unsigned int b) { return (a>b) ? a : b; }
inline int 	max (int a, int b)		{ return (a>b) ? a : b; }

inline long 	max (long a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (int a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (long a, int b) 		{ return (a>b) ? a : b; }

inline float 	max (float a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (int a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, int b) 		{ return (a>b) ? a : b; }
inline float 	max (long a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, long b) 		{ return (a>b) ? a : b; }

inline double 	max (double a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (int a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, int b) 		{ return (a>b) ? a : b; }
inline double 	max (long a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, long b) 		{ return (a>b) ? a : b; }
inline double 	max (float a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (double a, float b) 	{ return (a>b) ? a : b; }


inline int	min (int a, int b)		{ return (a<b) ? a : b; }

inline long 	min (long a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (int a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (long a, int b) 		{ return (a<b) ? a : b; }

inline float 	min (float a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (int a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, int b) 		{ return (a<b) ? a : b; }
inline float 	min (long a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, long b) 		{ return (a<b) ? a : b; }

inline double 	min (double a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (int a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, int b) 		{ return (a<b) ? a : b; }
inline double 	min (long a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, long b) 		{ return (a<b) ? a : b; }
inline double 	min (float a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (double a, float b) 	{ return (a<b) ? a : b; }


namespace{

struct Meta
{
    void declare (const char* key, const char* value) { }
};
}

#if BUILD_MONO
#  include "system_compressor_mono.cpp"
#else
#  include "system_compressor_stereo.cpp"
#endif


class MyUI : public UI
{

 public:
  void declare(float* control_port, const char* key, const char* value) {
  }

  const char *_curr_box_name;

  void openFrameBox(const char* label) {_curr_box_name = label;}
  void openTabBox(const char* label) {_curr_box_name = label;}
  void openHorizontalBox(const char* label) {_curr_box_name = label;}
  void openVerticalBox(const char* label) {_curr_box_name = label;}
  void closeBox() {_curr_box_name = NULL;}

  std::vector<float*> _controllers;
  std::vector<float*> _graphs;

  void remove_last_item(){
    //printf("Popping last effect\n");
    _controllers.pop_back();
  }

  void addEffect(const char *name, float* control_port, float min_value, float default_value, float max_value){
    //printf("Adding effect %s %p\n",name,control_port);
    _controllers.push_back(control_port);
  }

  void addButton(const char* label, float* zone) {
    addEffect(label, zone, 0, 0, 1);
  }
  void addToggleButton(const char* label, float* zone) {
    addEffect(label, zone, 0, 0, 1);
  }
  void addCheckButton(const char* label, float* zone) {
    addEffect(label, zone, 0, 0, 1);
  }
  void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone,  min, init, max);
  }
  void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone,  min, init, max);
  }
  void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) {
    addEffect(label, zone, min, init, max); // The INT effect format might not work. Need to go through the code first.
  }
  
  // -- passive widgets

  void addNumDisplay(const char* label, float* zone, int precision) {remove_last_item();}
  void addTextDisplay(const char* label, float* zone, const char* names[], float min, float max) {remove_last_item();}
  void addHorizontalBargraph(const char* label, float* zone, float min, float max) {
    *zone = (min+max)/2; // init
    _graphs.push_back(zone);
    //remove_last_item(); // remove metadata
    //next_peak = zone;
  }
  void addVerticalBargraph(const char* label, float* zone, float min, float max) {
    *zone = (min+max)/2; // init
    _graphs.push_back(zone);
    //remove_last_item(); // remove metadata
    //next_peak = zone;
  }

};

struct Compressor_wrapper : public Faust_system_compressor {
  MyUI ui;

  Compressor_wrapper(float sample_rate){

  init(sample_rate);

  buildUserInterface(&ui);

    //fprintf(stderr,"SETTING PARAMETER THRESHOLD to -15 from wrapping constructor. Value before: %f.\n",get_parameter(COMP_EFF_THRESHOLD));
  }

  void reset(){
    const int num_parameters = COMP_EFF_BYPASS; // Warning, reset() doesn't quite work if bypass is handled by faust. (this only happens in the standalone jack version though, which doesn't use reset())
    float parms[num_parameters];

    // Must store parameter values before calling init, since init overwrites them.
    for(int i=0;i<num_parameters;i++)
      parms[i] = get_parameter(i);

    init(fSamplingFreq);

    // Set back.
    for(int i=0;i<num_parameters;i++)
      set_parameter(i,parms[i]);    
  }

  // Called from juce. In juce, this is the only place
  void samplerate_has_changed(float new_sample_rate){

    if(fabs(fSamplingFreq-new_sample_rate) < 1.0)
      return;

    fSamplingFreq = new_sample_rate;
    reset();
  }

  float get_parameter(int num){
    float *controller = ui._controllers.at(num);
    return *controller;
  }

  void set_parameter(int num, float val){
    float *controller = ui._controllers.at(num);
    *controller = val;
  }

  bool update_parameter_if_different(int num, float val){
    float *controller = ui._controllers.at(num);
    if(fabs(*controller-val)>0.0001){
      *controller = val;
      return true;
    }else
      return false;
  }

  float get_graph_value(int num){
    float *controller = ui._graphs.at(num);
    return *controller;
  }
};

  //} // scw namespace

void *COMPRESSOR_create(float sample_rate){
  return new Compressor_wrapper(sample_rate);
}

void COMPRESSOR_delete(void *das_wrapper){
  Compressor_wrapper *wrapper = (Compressor_wrapper*)das_wrapper;
  delete wrapper;
}

float COMPRESSOR_get_parameter(void *das_wrapper,int num){
  Compressor_wrapper *wrapper = (Compressor_wrapper*)das_wrapper;
  return wrapper->get_parameter(num);
}

void COMPRESSOR_set_parameter(void *das_wrapper,int num,float value){
  Compressor_wrapper *wrapper = (Compressor_wrapper*)das_wrapper;
  wrapper->set_parameter(num,value);
  //printf("Setting controller %d (%p) to %f (%f)\n",num,controller,value,*controller);
}

float COMPRESSOR_get_graph_value(void *das_wrapper, int num){
 Compressor_wrapper *wrapper = (Compressor_wrapper*)das_wrapper;
  return wrapper->get_graph_value(num);
}

void COMPRESSOR_process(void *das_wrapper, float **inputs, float **outputs, int num_frames){
  Compressor_wrapper *wrapper = (Compressor_wrapper*)das_wrapper;
  wrapper->compute(num_frames, inputs, outputs);
}

