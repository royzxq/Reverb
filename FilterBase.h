//
//  FilterBase.h
//  Reverb
//
//  Created by XinquanZhou on 5/27/15.
//
//

#ifndef Reverb_FilterBase_h
#define Reverb_FilterBase_h

#include"RingBuffer.h"

class FilterBase {
protected:
    int _delay_line;
    RingBuffer<float> _xdelay,_ydelay;
    FilterBase();
public:
    enum class FilterType {FIR, IIR};
    FilterType Type;
    FilterBase(int delay_line, FilterType f = FilterType::IIR) :_delay_line(delay_line+1),_xdelay(_delay_line ), _ydelay(f == FilterType::IIR ? (_delay_line):0), Type(f) {
        _xdelay.setReadIdx(- _delay_line);
        if (Type == FilterType::IIR) {
            _ydelay.setReadIdx(- _delay_line);

        }
    }
    
    virtual  ~FilterBase(){}
    
    virtual float ProcessBySample(float sample) = 0;
    
    virtual void ProcessByBuffer (float * input, float * output, int buffersize) = 0 ;
};


class LowPassFilter: public FilterBase{
private:
    float _kp, _bp;
    LowPassFilter();
public:
    LowPassFilter(int delay_line):FilterBase(delay_line),_kp(0), _bp(0){
        _xdelay.setReadIdx(-1);
        _ydelay.setReadIdx(-1);
    }
    
    LowPassFilter( float kp, float bp): FilterBase(1), _kp(kp), _bp(bp){}
    ~LowPassFilter(){}
    
    void SetParams(float kp, float bp){
        _kp = kp;
        _bp = bp;
       // _xdelay.clear();
       // _ydelay.clear();
    }
    
    virtual float ProcessBySample(float sample){
        float PreY = _ydelay.get();
        float CurY = (_kp - _kp * _bp) * sample + _bp * PreY;
        _ydelay.push(CurY);
        return CurY;
    }
    
    virtual void ProcessByBuffer(float * input, float * output, int buffersize){
        for (int i = 0 ; i < buffersize; ++i) {
            output[i] = ProcessBySample(input[i]);
        }
    }
};

class AllPassFilter: public FilterBase {
    float _gain;
    
public:
    AllPassFilter(int delay_line, float gain = 0.55):FilterBase(delay_line), _gain(gain){
    }
    
    virtual float ProcessBySample(float sample){
        
        float CurY = - _gain * sample + _xdelay.get() + _gain * _ydelay.get();
        _xdelay.push(sample);
        _ydelay.push(CurY);
        return CurY;
        
    }
    
    virtual void ProcessByBuffer(float * input, float * output, int buffersize){
        for (int i = 0 ; i < buffersize; ++i) {
            output[i] = ProcessBySample(input[i]);
        }
    }
};

class ToneCorrection : public FilterBase {
    float _a, _gain;
public:
    ToneCorrection():FilterBase(1,FilterType::FIR),_a(1.25/3),_gain(0){
        _gain = (1-_a)/(1+_a);
    }
    ToneCorrection(float delay_line, float a):FilterBase(delay_line, FilterType::IIR), _a(a){
        _gain = (1-_a)/(1+_a);
        _xdelay.setReadIdx(-1);
    }
    
    
    void setParam(float a){
        _a = a;
        _gain = (1-_a)/(1+_a);
        _xdelay.clear();
    }
    virtual float ProcessBySample(float sample){
        
        float curY = ( sample - _gain * _xdelay.get()) / (1- _gain);
        _xdelay.push(sample);
        return curY;
    }
    
    virtual void ProcessByBuffer(float * input, float * output, int buffersize){
        for (int i = 0 ; i < buffersize; ++i) {
            output[i] = ProcessBySample(input[i]);
        }
    }
};

#endif
