//
//  FDN.h
//  Reverb
//
//  Created by XinquanZhou on 5/28/15.
//
//

#ifndef Reverb_FDN_h
#define Reverb_FDN_h

#include <iostream>
#include "/Users/xinquanzhou/Workspace/Source/Eigen/Core"
#include "/Users/xinquanzhou/Workspace/Source/Eigen/Dense"
#include "RingBuffer.h"
//
//static const int coff[] ={
//    919,997,1061,1093,1129,1151,1171,1187,1213,1237,1259,1283,1303,1319,1327,1361
//};

class FDN {
private:
    int _DLs_num;
    RingBuffer<float> ** _xdelay_lines;
    Eigen::MatrixXf _gains;
    Eigen::MatrixXf _matrix;
    Eigen::MatrixXf _delay_in, _delay_out, _network_out ;
    
    void _delayAllLines(){
        for (int i = 0 ; i < _DLs_num; ++i) {
            _delay_out(0,i) = _delayOneLine(_delay_in(0,i),i);
        }
    }
    
    float _delayOneLine(float sample, int idx){
        float res = _xdelay_lines[idx]->get();
        _xdelay_lines[idx]->push(sample);
        return res;
    }
    
    void _processMatrix(){
        for (int i = 0 ; i < _DLs_num; ++i) {
            float a = _delay_out(0,i);
        }
        _network_out = _delay_out * _matrix;
        for (int i = 0 ; i < _DLs_num; ++i) {
            float a = _network_out(0,i);
            _network_out(0,i) *= _gains(0,i);
        }
    }
    
    
public:
    
    const int coff[16] ={
        919,997,1061,1093,1129,1151,1171,1187,1213,1237,1259,1283,1303,1319,1327,1361
    };
    
    FDN():_DLs_num(16), _gains(1,_DLs_num),_matrix(_DLs_num, _DLs_num), _delay_in(1,_DLs_num), _delay_out(1,_DLs_num), _network_out(1,_DLs_num){
        
        Eigen::MatrixXf tmpMat1 (_DLs_num/4, _DLs_num/4);
        try {
            tmpMat1 << 1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,1;
            tmpMat1 = tmpMat1 * 0.5;
            
            // just for copy and paste
            Eigen::MatrixXf feedbackMatrix2 = tmpMat1;
            
            _matrix << feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,
            -feedbackMatrix2,feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,
            -feedbackMatrix2,-feedbackMatrix2,feedbackMatrix2,-feedbackMatrix2,
            -feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,feedbackMatrix2;
            
            _matrix = _matrix * 0.5;
            
            _gains << 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1;
            
            _xdelay_lines = new RingBuffer<float> *[_DLs_num];
            for (int i = 0 ; i < _DLs_num; ++i) {
                _xdelay_lines[i] = new RingBuffer<float>(coff[i]);
            }
            _network_out = Eigen::MatrixXf::Zero(1, _DLs_num);
            
        } catch (const std::exception& ex) {
            std::cout << "matrix initialize failed" << std::endl;
            throw;
        }
        
        
    }
    
    FDN(int DLs_num):_DLs_num(DLs_num), _gains(1,_DLs_num),_matrix(_DLs_num, _DLs_num), _delay_in(1,_DLs_num), _delay_out(1,_DLs_num), _network_out(1,_DLs_num){
        
        Eigen::MatrixXf tmpMat1 (_DLs_num/4, _DLs_num/4);
        try {
            tmpMat1 << 1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,1;
            tmpMat1 = tmpMat1 * 0.5;
            
            // just for copy and paste
            Eigen::MatrixXf feedbackMatrix2 = tmpMat1;
            
            _matrix << feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,
            -feedbackMatrix2,feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,
            -feedbackMatrix2,-feedbackMatrix2,feedbackMatrix2,-feedbackMatrix2,
            -feedbackMatrix2,-feedbackMatrix2,-feedbackMatrix2,feedbackMatrix2;
            
            _matrix = _matrix * 0.5;
            
            _gains << 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1;
            
            _xdelay_lines = new RingBuffer<float> *[_DLs_num];
            for (int i = 0 ; i < _DLs_num; ++i) {
                _xdelay_lines[i] = new RingBuffer<float>(coff[i] + 1);
            }
            _network_out = Eigen::MatrixXf::Zero(1, _DLs_num);

        } catch (const std::exception& ex) {
            std::cout << "matrix initialize failed" << std::endl;
            throw;
        }
        
        
    }
    ~FDN(){
        for (int i = 0 ; i < _DLs_num; ++i) {
            delete _xdelay_lines[i];
            _xdelay_lines[i] = 0 ;
        }
        delete _xdelay_lines;
        _xdelay_lines = 0 ;
    }
    
    Eigen::MatrixXf process(Eigen::MatrixXf& input){
        _delay_in = _network_out + input;
        _delayAllLines();
        _processMatrix();
        return _delay_out;
    }
    
    void processByBlock(Eigen::MatrixXf * input, Eigen::MatrixXf * output, int blocksize){
        for (int i = 0 ; i < blocksize; ++i) {
            output[i] = process(input[i]);
        }
    }
    
    
    
    
};

#endif
