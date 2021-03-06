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

    for (int i = 0; i < 100; i++) {
        int base = INT_MAX;//Any number can be inserted within this number
        int insert = rand() % (INT_MAX);//Generate any random positive number
        int numOfBits = countBits(insert);
        int randOffset = rand() % (32 - numOfBits);

        int mask = insert;

        for (int i = 0; i < randOffset; i++) {
            mask = (mask << 1) + 0;
        }

        int res = glm::bitfieldInsert(base, insert, randOffset, numOfBits);

        EXPECT_EQ(insert, (res & mask) >> randOffset);
    }
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

TEST(ceil, CeilToIntMax) {
    int max = INT_MAX - 1;
    
    EXPECT_EQ(glm::ceil(max + 0.6), INT_MAX);
}

TEST(clamp, allEqualElementsReturnsElement) {// Computes min(max(param1, param2), param3) 
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randNum = rand() % INT_MAX;
        EXPECT_EQ(randNum, glm::clamp(randNum, randNum, randNum));
    }
}

TEST(clamp, comprateToMathMinMax) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int num1 = rand() % INT_MAX;
        int num2 = rand() % INT_MAX;
        int num3 = rand() % INT_MAX;
        EXPECT_EQ(glm::clamp(num1, num2, num3), min(max(num1, num2), num3));
    }
}

TEST(clamp, comprateToMathMinMaxWithNegatives) {
    srand(time(NULL));

    for (int i = 0; i < 1; i++) {
        int randNegativeRange = rand() % INT_MAX;//Used to generate a range so the number can be either positive or negative
        int num1 = rand() % INT_MAX - randNegativeRange;
        int num2 = rand() % INT_MAX - randNegativeRange;
        int num3 = rand() % INT_MAX - randNegativeRange;
        
        EXPECT_EQ(glm::clamp(num1, num2, num3), min(max(num1, num2), num3));
    }
}

TEST(floor, floorOfZeroIsZero) {
    EXPECT_EQ(0, glm::floor(0));
}

TEST(floor, floorOfValuesAboveHalfRoundDown) {//Values like 0.5, 0.6, 0.7, 0.8, 0.9 should all round to 0
    srand(time(NULL));
    int base = rand() % INT_MAX;

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(base, glm::floor(base + (i / 10)));
    }
}

TEST(floor, floorOfValuesAboveHalfRoundDownWithNegativeBase) {
    srand(time(NULL));
    int base = rand() % INT_MAX - INT_MAX;

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(base, glm::floor(base + (i / 10)));
    }
}

TEST(fract, fractOfZeroIsZero) {
        EXPECT_EQ(0, glm::fract(0.0));
}

TEST(fract, fractOfIntIsZero) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int randInt = rand() % INT_MAX;
        
        EXPECT_EQ(0, glm::fract(randInt * 1.0));
    }
}

TEST(fract, randomGeneratedFract) {//Generates random base abd random fraction. Verifies no base can affect the result
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int base = rand() % 10;//Small base so all numbers after decimal are accounted for
        float fraction = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);//Random between 0.0 and 1.0
        
        EXPECT_EQ(fraction, glm::fract(base * 1.0 + fraction));
    }
}//Fract was tested with negative numbers and found issues.

TEST(isInf, infinityGivesTrue) {
    EXPECT_TRUE(glm::isinf(std::numeric_limits<double>::infinity()));
}

TEST(isInf, intIsNotInfinity) {
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        double randNum = rand() % INT_MAX;

        EXPECT_FALSE(glm::isinf(randNum));

    }
}

TEST(Max, maxOfEquals) {
    srand(time(NULL));

    int num = rand() % INT_MAX;
    EXPECT_EQ(num, glm::max(num, num));
}

TEST(Max, maxOfNumAndItsSubsequent) {
    srand(time(NULL));

    int num = rand() % INT_MAX;
    EXPECT_EQ(num + 1, glm::max(num, num + 1));
}

TEST(Max, orderOfParamsDoesNotMatter) {//max(x, y) == max(y, x)
    srand(time(NULL));

    int num1 = rand() % INT_MAX;
    int num2 = rand() % INT_MAX;

    EXPECT_EQ(glm::max(num1, num2), glm::max(num2, num1));
}

TEST(Max, maxVSImplementedMax) {
    srand(time(NULL));

    int num1 = rand() % INT_MAX;
    int num2 = rand() % INT_MAX;

    if (num1 > num2) {
        EXPECT_EQ(num1, glm::max(num1, num2));
    }
    else {
        EXPECT_EQ(num2, glm::max(num1, num2));
    }
}

TEST(Min, minOfSameNumIsNum) {
    srand(time(NULL));

    int num1 = rand() % INT_MAX;

    EXPECT_EQ(num1, glm::min(num1, num1));
}

TEST(Min, minOfNumAndItsSubsequent) {
    srand(time(NULL));

    int num = rand() % INT_MAX;
    EXPECT_EQ(num, glm::min(num, num + 1));
}

TEST(Min, orderOfParamsDoesNotMatter) {//min(x, y) == min(y, x)
    srand(time(NULL));

    int num1 = rand() % INT_MAX;
    int num2 = rand() % INT_MAX;

    EXPECT_EQ(glm::min(num1, num2), glm::min(num2, num1));
}

TEST(Min, minVSImplementedMin) {
    srand(time(NULL));

    int num1 = rand() % INT_MAX;
    int num2 = rand() % INT_MAX;

    if (num1 > num2) {
        EXPECT_EQ(num2, glm::min(num1, num2));
    }
    else {
        EXPECT_EQ(num1, glm::min(num1, num2));
    }
}

TEST(Mod, modOfOneIsZero) {
    srand(time(NULL));

    int num1 = rand() % INT_MAX;
    EXPECT_EQ(0, glm::mod(num1 * 1.0, 1.0));
}

TEST(Mod, modVSSTDMod) {//Compared against the Math mod of standard library
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int num1 = rand() % INT_MAX;
        int divider = rand() % INT_MAX;

        EXPECT_EQ(num1 % divider, glm::mod(num1 * 1.0, divider * 1.0));
    }
}

TEST(Mod, modVSImplementedMod) {//Compared against mod formula
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int num1 = rand() % INT_MAX;
        int divider = rand() % INT_MAX;

        EXPECT_EQ(num1 - divider * floor(num1/ divider), glm::mod(num1 * 1.0, divider * 1.0));
    }
}
