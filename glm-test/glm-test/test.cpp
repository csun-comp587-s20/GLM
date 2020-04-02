#include "pch.h"
#include "../../glm/integer.hpp"
#include <stdio.h>
#include <math.h>
#include <time.h>

using namespace std;

int countBits(int num) {
    int count = 1;
    while (num >> 1 != 0) {//count digits
        num >>= 1;
        count++;
    }

    return count;
}

TEST(BitCount, TestZero) {
    EXPECT_EQ(glm::bitCount(0), 0);
}

TEST(BitCount, TestOne) {
    EXPECT_EQ(glm::bitCount(1), 1);
}

TEST(BitCount, TestNegativeOne) {
    EXPECT_EQ(glm::bitCount(-1), 32);
}

TEST(BitCount, TestPowersOfTwo) {
    for (int i = 1; i < 10; i++) {
        EXPECT_EQ(glm::bitCount((int)pow(2, i)), 1);
    }
}

TEST(BitExtract, TestExtractFromZero) {
    EXPECT_EQ(glm::bitfieldExtract(0, 1, 3), 0);
}

TEST(BitExtract, TestExtractFromFifteen) {
    EXPECT_EQ(glm::bitfieldExtract(15, 1, 3), 7);
}

TEST(BitExtract, TestExtractBitsZero) {
    EXPECT_EQ(glm::bitfieldExtract(15, 3, 0), 0);
}

TEST(BitExtract, TestOffsetLongerThanNumber) {
    for (int i = 0; i < 100; i++) {
        int num = i;
        int numOfBits = countBits(i);

        EXPECT_EQ(glm::bitfieldExtract(i, numOfBits + 1, 4), 0);
    }
}

TEST(BitExtract, TestNegativeBitsOffsetZeroReturnsItself) {
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        int rndNum = rand() % INT_MAX;

        EXPECT_EQ(glm::bitfieldExtract(rndNum, 0, -3), rndNum);
    }
}

TEST(BitExtract, TestAllOdd) {
    for (int i = 1; i < 100; i += 2) {
        EXPECT_EQ(glm::bitfieldExtract(i, 0, 1), 1);
    }
}

TEST(BitExtract, TestAllEven) {
    for (int i = 0; i < 100; i += 2) {
        EXPECT_EQ(glm::bitfieldExtract(i, 0, 1), 0);
    }
}

TEST(BitInsert, TestNoChange) {
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(glm::bitfieldInsert(i, 0, 0, 0), i);
    }
}

TEST(BitInsert, TestAddOneLeastSignificantBit) {
    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        int rndNum = rand() % INT_MAX;
        int num = glm::bitfieldInsert(rndNum, 1, 0, 1);

        EXPECT_EQ(num % 2, 1);
    }
}

TEST(BitInsert, TestAddOneMostSignificantBit) {
    for (int i = 0; i < 100; i++) {
        int mask = 1;

        int numOfBits = countBits(i);
        mask <<= numOfBits - 1;

        int num = glm::bitfieldInsert(i, 1, numOfBits - 1, 1);
       
        EXPECT_EQ(num & mask, (int)pow(2, numOfBits - 1));//Bitwise AND should leave only most significant bit set
    }
}

TEST(BitInsert, InsertAllOnes) {
    int numAllOnes = pow(2, 10) - 1;//Used to extract all ones (number has to be less than 1M for this test)
    for (int i = 0; i < 1000; i++) {
        int numOfBits = countBits(i);
        int num = glm::bitfieldInsert(i, numAllOnes, 0, numOfBits);
       
        EXPECT_EQ(num + 1, (int)pow(2, numOfBits));//Adding 1 should zero everything and add 1 at the most significant bit
    }
}

//TEST(BitInsert, InsertRandomNumberAtRandomIndex) {//TODO
//    srand(time(NULL));
//    
//    int base = INT_MAX;//Any number can be inserted within this number
//    int insert = rand() % INT_MAX;//Generate any random positive number
//    int numOfBits = countBits(insert);
//    int randOffset = rand() % (33 - numOfBits);
//    
//    //base = 31;//11111
//    //insert = 5;//101
//    //numOfBits = 3;
//    //randOffset = 2;
//    ////mask should be 1110
//    ////10111
//    
//    int res = glm::bitfieldInsert(base, insert, randOffset, numOfBits);
//    cout << randOffset << " " << numOfBits << " " << insert;
//    int mask = 0;
//    for (int i = randOffset; i < randOffset + numOfBits; i++) {
//        mask = (mask << 1) + 1;
//    }
//
//    mask <<= randOffset;
//    cout << '\n' << mask << " " << res;
//    int x = 2130710527;
//    int y = 33550336;
//    int bla = x & y;
//    cout << '\n' << bla;
//    EXPECT_EQ(insert, (res & mask) >> 2);
//}

TEST(BitLSB, LSBZeroNegativeOne) {
    EXPECT_EQ(-1, glm::findLSB(0));
}

TEST(BitLSB, LSBOneEqualsZero) {
    EXPECT_EQ(0, glm::findLSB(1));
}

TEST(BitLSB, LSBPowerOfTwo) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int exp = rand() % 10;
        
        EXPECT_EQ(exp, glm::findLSB((int)pow(2, exp)));
    }
}

TEST(BitLSB, AllOddsReturnZero) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int odd = (rand() % (INT_MAX / 2)) * 2 + 1;//all odd numbers

        EXPECT_EQ(0, glm::findLSB(odd));
    }
}

TEST(BitLSB, AllEvensReturnNonZero) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int even = (rand() % (INT_MAX / 2)) * 2;//all even numbers

        EXPECT_NE(0, glm::findLSB(even));
    }
}

TEST(BitLSB, ShiftOneNTimesReturnN) {
    srand(time(NULL));

    int shift = rand() % 32;
    int num = 1 << shift;

    EXPECT_EQ(shift, glm::findLSB(num));
}

TEST(BitMSB, MSBOfMaxIntIs30) {
    EXPECT_EQ(30, glm::findMSB(INT_MAX));
}

TEST(BitMSB, MSBOfZeroIsNegativeOne) {
    EXPECT_EQ(-1, glm::findMSB(0));
}

TEST(BitMSB, MSBOfOneIsZero) {
    EXPECT_EQ(0, glm::findMSB(1));
}

//