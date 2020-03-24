#include "pch.h"
#include "../../glm/integer.hpp"
#include <stdio.h>
#include <math.h>

using namespace std;

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
        int numOfDigits = 0;
        int num = i;
        while (num % 10 != 0) {
            num %= 10;
            numOfDigits++;
        }

        EXPECT_EQ(glm::bitfieldExtract(i, numOfDigits, 4), 0);
    }
}

TEST(BitExtract, TestNegativeOffsetReturnsZero) {
    for (int i = -1; i > -100; i--)
        EXPECT_EQ(glm::bitfieldExtract(INT_MAX, i, 3), 0);
}

TEST(BitExtract, TestBitsNegativeOffsetZeroEqualsItself) {
    for (int i = 0; i < 100; i++)
        EXPECT_EQ(glm::bitfieldExtract(i, 0, -1), i);
}

TEST(BitExtract, TestAllOdd) {
    for (int i = 1; i < 100; i + 2) {
        EXPECT_EQ(glm::bitfieldExtract(i, 0, 1), 1);
    }
}

TEST(BitExtract, TestAllEven) {
    for (int i = 0; i < 100; i + 2) {
        EXPECT_EQ(glm::bitfieldExtract(i, 0, 1), 0);
    }
}

TEST(BitInsert, TestNoChange) {
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(glm::bitfieldInsert(i, 1, 0, 1), i);
    }
}

TEST(BitInsert, TestNoAdd) {
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(glm::bitfieldInsert(i, 0, 0, 0), i);
    }
}

TEST(BitInsert, TestAddTwoOnesBeginning) {
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(glm::bitfieldInsert(i, 2, 0, 0), i);
    }
}

TEST(BitInsert, TestAddTwoOnesBeginning) {
    for (int i = 0; i < 100; i++) {
        int numOfDigits = 0;
        int num = i;
        while (num % 10 != 0) {
            num %= 10;
            numOfDigits++;
        }

        EXPECT_EQ(glm::bitfieldInsert(num, 2, 0, numOfDigits), i);
    }
}