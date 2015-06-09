//
//  FDNTest.h
//  Reverb
//
//  Created by XinquanZhou on 6/3/15.
//
//

#ifndef Reverb_FDNTest_h
#define Reverb_FDNTest_h

#include <algorithm>
#include "gtest/gtest.h"
#include "FileReader.h"
#include "SignalGen.h"
#include "FDN.h"
#include "/Users/xinquanzhou/Workspace/Source/Eigen/Core"
#include "/Users/xinquanzhou/Workspace/Source/Eigen/Dense"



struct MatrixBuffer{
    int buffersize;
    Eigen::MatrixXf * input, * output;
    float * _input, * _output;
    
    MatrixBuffer():buffersize(512), input(new Eigen::MatrixXf[buffersize]), output(new Eigen::MatrixXf[buffersize]), _input(new float [buffersize]), _output(new float[buffersize]){
        std::fill(_input,_input+buffersize,0.0);
        for(int i = 0 ; i < buffersize; ++i){
            input[i].resize(1,16);
            output[i].resize(1,16);
        }
    }
    ~MatrixBuffer(){
        delete []_input;
        delete []_output;
        delete []input;
        delete []output;
    }
    
    void setMatrixBuffer(){
        for(int i = 0 ; i < buffersize; i++){
            input[i] = Eigen::MatrixXf::Ones(1,16) * _input[i];
            for(int j = 0 ; j < 16 ; j++){
                float x = input[i](0,j);
            }
        }
    }
};

TEST (FDNTest, DC)
{
    MatrixBuffer b;
    CSignalGen::generateDc(b._input,b.buffersize);
    b.setMatrixBuffer();
    FileReader f("/Users/xinquanzhou/Workspace/Source/Rebuild/Builds/MacOSX/MatlabTest/FDN_REVERB/FDNDC.txt");
    f.split(' ');
    FDN fdn;
    for (int i = 0 ; i < b.buffersize; ++i) {
        b.output[i] = fdn.process(b.input[i]);
        for (int j = 0 ; j < 16 ; j++) {
//            float m = b.output[i](0,j);
//            float n = f.content[i][j];
//            if(fabsf(m-n) > 0.001){
//                float flag = 1.0;
//            }
            ASSERT_NEAR(b.output[i](0,j),f.content[i][j], 0.001);
        }
    }
}


TEST (FDNTest, ArbTest)
{
    MatrixBuffer b;
    CSignalGen::generateSine(b._input,1000,44100/4,b.buffersize);
    b.setMatrixBuffer();
    FileReader f("/Users/xinquanzhou/Workspace/Source/Rebuild/Builds/MacOSX/MatlabTest/FDN_REVERB/FDNSine.txt");
    f.split(' ');
    FDN fdn;
    for (int i = 0 ; i < b.buffersize; ++i) {
        b.output[i] = fdn.process(b.input[i]);
        for (int j = 0 ; j < 16 ; j++) {
            ASSERT_NEAR(b.output[i](0,j),f.content[i][j], 0.001);
        }

    }
}


#endif
