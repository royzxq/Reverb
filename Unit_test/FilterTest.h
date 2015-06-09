//
//  FilterTest.h
//  Reverb
//
//  Created by XinquanZhou on 6/3/15.
//
//

#ifndef Reverb_FilterTest_h
#define Reverb_FilterTest_h


#include "gtest/gtest.h"
#include "SignalGen.h"
#include "FilterBase.h"
#include "FileReader.h"
#include <algorithm>

using namespace std;

struct Buffer {
    Buffer():size(512),input_buffer(new float[size]), output_buffer(new float[size]){
        kp = new float[16];
        bp = new float[16];
        for(int i = 0; i < 16 ; i++){
            kp[i] = powf(10.0, (-1 * float(m[i])/fs));
            float q = 1.25 /3 ;
            bp[i] = 20 * log10f(kp[i]) * logf(10.0) / 80 * (1-1/(q*q));
        }
    }
    ~Buffer(){
        delete []input_buffer;
        delete []output_buffer;
        delete []kp;
        delete [] bp;
        input_buffer = 0;
        output_buffer = 0 ;
    }

    void setSize(int new_size){
        delete input_buffer;
        delete output_buffer;
        size = new_size;
        input_buffer = new float[size];
        output_buffer = new float[size];
    }
    int size;
    float * input_buffer, *output_buffer, *kp, *bp;
    
    const int m [16] = {919, 997, 1061, 1093, 1129, 1151, 1171, 1187, 1213, 1237, 1259, 1283, 1303, 1319, 1327, 1361};
    const int fs = 44100;
};

TEST (Unit_test, DC)
{
    Buffer b;
    CSignalGen::generateDc(b.input_buffer,b.size);
    FileReader f("/Users/xinquanzhou/Workspace/Source/Rebuild/Builds/MacOSX/MatlabTest/DC.txt");
    f.split(' ');
    LowPassFilter lp(1,1);
    
    lp.SetParams(b.kp[0], b.bp[0]);
    for (int i = 0 ; i < b.size; ++i) {
        b.output_buffer[i] = lp.ProcessBySample(b.input_buffer[i]);
        ASSERT_NEAR(b.output_buffer[i],f.content[0][i], 0.001);
    }
    
    

    AllPassFilter ap(37);

    for (int i = 0 ; i < b.size; ++i) {
        b.output_buffer[i] = lp.ProcessBySample(b.input_buffer[i]);
        ASSERT_NEAR(b.output_buffer[i],f.content[1][i], 0.001);
    }
    
    ToneCorrection tc;
    
    for (int i = 0 ; i < b.size; ++i) {
        b.output_buffer[i] = tc.ProcessBySample(b.input_buffer[i]);
        ASSERT_NEAR(b.output_buffer[i],f.content[2][i], 0.001);
    }

    
}

TEST (Unit_test, LP_Impulse)
{
    Buffer b;
    //CSignalGen::generateDc(b.input_buffer,b.size);
    fill(b.input_buffer,b.input_buffer+b.size,0.0);
    b.input_buffer[0] = 1.0;
    FileReader f("/Users/xinquanzhou/Workspace/Source/Rebuild/Builds/MacOSX/MatlabTest/lp_impulse.txt");
    f.split(' ');
    LowPassFilter lp(1,1);
    
    for(int j = 0 ; j < 16 ; j++){
        lp.SetParams(b.kp[j], b.bp[j]);
        for (int i = 0 ; i < b.size; ++i) {
            b.output_buffer[i] = lp.ProcessBySample(b.input_buffer[i]);
            ASSERT_NEAR(b.output_buffer[i],f.content[j][i], 0.001);
        }
    }
    
}


TEST (Unit_test, AP_Impulse)
{
    Buffer b;
    //CSignalGen::generateDc(b.buffer,b.size);
    fill(b.input_buffer,b.input_buffer+b.size,0.0);
    b.input_buffer[0] = 1.0;
    FileReader f("/Users/xinquanzhou/Workspace/Source/Rebuild/Builds/MacOSX/MatlabTest/ap_impulse.txt");
    f.split(' ');
    AllPassFilter ap(37);

    for (int i = 0 ; i < b.size; ++i) {
        b.output_buffer[i] = ap.ProcessBySample(b.input_buffer[i]);
//        float m = b.output_buffer[i];
//        float n = f.content[0][i];
//        if( fabsf(m-n) > 0.001){
//            float flag = 0 ;
//        }
        ASSERT_NEAR(b.output_buffer[i],f.content[0][i], 0.001);
    }
    AllPassFilter ap2(113);
    
    for (int i = 0 ; i < b.size; ++i) {
        b.output_buffer[i] = ap2.ProcessBySample(b.input_buffer[i]);
        ASSERT_NEAR(b.output_buffer[i],f.content[1][i], 0.001);
    }
}


TEST (Unit_test, TC_Impulse)
{
    Buffer b;
    //CSignalGen::generateDc(b.buffer,b.size);
    fill(b.input_buffer,b.input_buffer+b.size,0.0);
    b.input_buffer[0] = 1.0;
    FileReader f("/Users/xinquanzhou/Workspace/Source/Rebuild/Builds/MacOSX/MatlabTest/tc_impulse.txt");
    f.split(' ');
    ToneCorrection tc;

    for (int i = 0 ; i < b.size; ++i) {
        b.output_buffer[i] = tc.ProcessBySample(b.input_buffer[i]);
        ASSERT_NEAR(b.output_buffer[i],f.content[0][i], 0.001);
    }

}


#endif
