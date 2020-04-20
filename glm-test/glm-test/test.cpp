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

/* Integer functions */
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

TEST(BitInsert, InsertRandomNumberAtRandomIndex) {
    srand(time(NULL));

    int base = INT_MAX;//Any number can be inserted within this number
    int insert = rand() % INT_MAX;//Generate any random positive number
    int numOfBits = countBits(insert);
    int randOffset = rand() % (33 - numOfBits);

    int mask = insert;

    for (int i = 0; i < randOffset; i++) {
        mask = (mask << 1) + 0;
    }

    int res = glm::bitfieldInsert(base, insert, randOffset, numOfBits);

    EXPECT_EQ(insert, (res & mask) >> randOffset);
}

TEST(BitLSB, LSBZeroEqualsNegativeOne) {
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

TEST(BitMSB, MSBOfPowerTwo) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int exp = rand() % 10;

        EXPECT_EQ(exp, glm::findMSB((int)pow(2, exp)));
    }
}

TEST(BitMSB, MSBFloatingOne) {//Shift 1 between 0 to 32 times and verify its MSB is the number of shifts
    for (int j = 0; j < 32; j++) {
        int numOfShifts = j;
        int num = 1;
        for (int i = 0; i < numOfShifts; i++) {
            num = (num << 1);
        }

        EXPECT_EQ(numOfShifts, glm::findMSB(num));
    }
}

TEST(BitMSB, MSBOfAllOnes) {//Set number to all ones for every possible length between 0 and 32 digits and verify MSB
    for (int j = 0; j < 33; j++) {
        int numOfOnes = j;
        int num = 0;
        for (int i = 0; i < numOfOnes; i++) {
            num = (num << 1) + 1;
        }

        EXPECT_EQ(numOfOnes - 1, glm::findMSB(num));
    }
}

TEST(BitMSB, MSBOfRandomEqualsNumberOfBits) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randNum = rand() % INT_MAX;

        int numOfBits = countBits(randNum);

        EXPECT_EQ(numOfBits, glm::findMSB(randNum) + 1);
    }
}

/* Common functions */
TEST(ABS, AbsOfZeroIsZero) {
    EXPECT_EQ(0, glm::abs(0));
}

TEST(ABS, AbsOfRandomNegative) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randNum = (rand() % INT_MAX);
        int neg = -1 * randNum;

        EXPECT_EQ(randNum, glm::abs(neg));
    } 
}

TEST(ABS, AbsOfOddPowerIsPositive) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randNum = (rand() % INT_MAX);
        int neg = -1 * randNum;

        EXPECT_TRUE(glm::abs(pow(neg, 3)) > 0);
    }
}

TEST(Ceil, CeilOfZeroIsZero) {
    EXPECT_EQ(0, glm::ceil(0));
}

TEST(Ceil, CeilOfIntIsItself) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randNum = rand() % INT_MAX;

        EXPECT_EQ(randNum, glm::ceil(randNum));
    }
}

TEST(ceil, CeilOfNegativeIntIsItself) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randNum = rand() % INT_MAX;
        randNum = randNum * -1;
        EXPECT_EQ(randNum, glm::ceil(randNum));
    }
}

TEST(ceil, CeilOfRandomFloat) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        double randAfterDecimal = rand() % INT_MAX;

        while (randAfterDecimal > 1) {
            randAfterDecimal /= 10;
        }

        EXPECT_EQ(1, glm::ceil(randAfterDecimal));
    }
}