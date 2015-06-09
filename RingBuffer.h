//
//  RingBuffer.h
//  Reverb
//
//  Created by XinquanZhou on 5/26/15.
//
//

#ifndef Reverb_RingBuffer_h
#define Reverb_RingBuffer_h

#include<iostream>
#include<cstring>
using namespace std;

template<class T>
class RingBuffer{
public:
    RingBuffer(size_t BufferLengthInSamples):_readIdx(0),_writeIdx(0),_buffer(nullptr), _frac(0),_buffer_length(BufferLengthInSamples) {
        try {
            _buffer = new T[_buffer_length];
            this->clear();
        } catch (const std::exception& ex) {
            cout << "Allocation failed" << endl;
            throw;
        }
    };
    
    virtual ~RingBuffer(){
        delete _buffer;
        _buffer = 0 ;
    }
    
    void push(T value){
        _buffer[_writeIdx] = value;
        _IncIdx(_writeIdx);
        
    }
    
    void write(T value){
        _buffer[_writeIdx] = value;
    }
    
    T get(){
        T res = _buffer[_readIdx];
        _IncIdx(_readIdx);
        return res;
    }
    
    T read() const{
        return _buffer[_readIdx];
    }
    
    T read(int offset) const{
        //setReadIdx(getReadIdx + offset);
        return _buffer[getReadIdx() + offset];
    }
    
    T getInterpolation(){
        T res = this->get() * (1-_frac) + this->read() * _frac;
        return res;
    }
    
    void reset(){
        memset(_buffer,0,sizeof(T) * _buffer_length);
        _writeIdx = 0 ;
        _readIdx = 0;
        _frac = 0;
    }
    
    int getWriteIdx() const{
        return _writeIdx;
    }
    
    int getReadIdx() const{
        return _readIdx;
    }
    
    void setWriteIdx(int t){
        _IncIdx(_writeIdx, t - _writeIdx);
    }
    
    void setReadIdx(int t){
        _IncIdx(_readIdx, t - _readIdx);
    }
    
//    void setReadOffset(int offset){
//        setReadIdx(getReadIdx + offset);
//    }
    
    
    void setFrac(float fracDelay){
        int i = static_cast<int>(fracDelay);
        if(i == fracDelay){
            _frac = 0 ;
            
        }
        else{
            _frac = fracDelay - i;
        }
        setReadIdx(_writeIdx - i);
    }
    
    
    void clear(){
        memset(_buffer, 0, sizeof(T)*_buffer_length);
    }
    
private:
    int _readIdx, _writeIdx;
    T * _buffer;
    float _frac;
    size_t _buffer_length;
    
    void _IncIdx(int& idx, int offset = 1){
        while (idx + offset < 0 ) {
            offset += _buffer_length;
        }
        idx = (idx + offset ) % _buffer_length;
    }
};


#endif
