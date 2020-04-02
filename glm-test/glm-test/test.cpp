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
