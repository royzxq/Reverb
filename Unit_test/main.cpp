//
//  main.cpp
//  Unit_test
//
//  Created by XinquanZhou on 6/2/15.
//
//

#include <iostream>
#include "FilterTest.h"
#include "FDNTest.h"


int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}