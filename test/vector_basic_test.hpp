#include <gtest/gtest.h>
#include "../include/LinkedList.h"
#include <string>
#include <random>
#include <stdexcept>
#include <iostream>

using namespace aisdi;

struct Node{
    int a;
    double b;
};
class LinkedListTest : public ::testing::Test
{
  protected:
    LinkedListTest() {}
    ~LinkedListTest() {}

    void SetUp() override {
        doublev = LinkedList<double>();
        intv = LinkedList<int>();
        intv.append(a); intv.append(b); intv.append(c);
    }
    void TearDown() override {

    }
    LinkedList<int> intv;
   
    LinkedList<double> doublev;
    LinkedList<std::string> stringv;
    LinkedList<Node> nodev;
    int a = 1, b = 2, c = 3;
};

TEST_F(LinkedListTest, EmptyLinkedListSizeEqualsZero){
    ASSERT_EQ(doublev.getSize(), 0);
    ASSERT_EQ(stringv.getSize(), 0);
    ASSERT_EQ(nodev.getSize(), 0);

    ASSERT_TRUE(doublev.isEmpty());
}

TEST_F(LinkedListTest, PushedValuesAreInLinkedList){
    ASSERT_EQ(intv[0], a);
    ASSERT_EQ(intv[1], b);
    ASSERT_EQ(intv[2], c);
}

TEST_F(LinkedListTest, RetrieveOperatorsThrowsIfOutOfRange){
    ASSERT_THROW(intv[-1], std::out_of_range);
    ASSERT_THROW(intv[3], std::out_of_range);
}

// TEST_F(LinkedListTest, BackAndFrontReturnesRightValues){
//     ASSERT_EQ(*(intv.begin()), a);
//     ASSERT_EQ(*(intv.end()), c);
// }



TEST_F(LinkedListTest, PopedValuesAreReallyPopped){
    for(int i = 0; i < 3; i++)
        intv.popLast();
    ASSERT_EQ(intv.getSize(), 0);
    ASSERT_THROW(intv[0], std::out_of_range);
}

TEST_F(LinkedListTest, CopyConstructorCopies){ 
     LinkedList<int> other {intv};

    EXPECT_EQ(other[0], a);
    ASSERT_EQ(other[1], b);
    ASSERT_EQ(other[2], c);

    EXPECT_EQ(intv[0], a);
    ASSERT_EQ(intv[1], b);
    ASSERT_EQ(intv[2], c);
}

TEST_F(LinkedListTest, MoveConstructorMoves){
    LinkedList<int> other {std::move(intv)};

    ASSERT_EQ(other[0], a);
    ASSERT_EQ(other[1], b);
    ASSERT_EQ(other[2], c);

    ASSERT_ANY_THROW(intv[0]);
}

TEST_F(LinkedListTest, CopyAssignmentCopies){
    LinkedList<int> other = {3, 4, 5};
    other = intv;

    ASSERT_EQ(other[0], a);
    ASSERT_EQ(other[1], b);
    ASSERT_EQ(other[2], c);

    ASSERT_EQ(intv[0], a);
    ASSERT_EQ(intv[1], b);
    ASSERT_EQ(intv[2], c);
}

TEST_F(LinkedListTest, MoveAssignmentMoves){
    LinkedList<int> other = {3, 4, 5};
    other = std::move(intv);

    ASSERT_EQ(other[0], a);
    ASSERT_EQ(other[1], b);
    ASSERT_EQ(other[2], c);

    ASSERT_ANY_THROW(intv[0]);
}

TEST_F(LinkedListTest, StoresAllValuesAfterMultipleAppends){
    for(int i = 0; i < 1000; i++){
        doublev.append(1.1 + i);
    }

    for(int i = 0; i < 1000; i++){
        ASSERT_DOUBLE_EQ(doublev[i], 1.1 + i);
    }
}

TEST_F(LinkedListTest, PrependsValueAtBegining){
    intv.prepend(11);

    ASSERT_EQ(intv[0], 11);
}

TEST_F(LinkedListTest, StoresAllValuesAfterMultiplePrepends){
    for(int i = 0; i < 1000; i++){
        doublev.prepend(1.1 + i);
    }

    for(int i = 999; i >= 0; i--){
        ASSERT_EQ(doublev[i], 999 - i + 1.1);
    }
}


TEST_F(LinkedListTest, PopFirstReturnsGoodValues){
    ASSERT_EQ(intv.popFirst(), 1);
    ASSERT_EQ(intv.popFirst(), 2);
    ASSERT_EQ(intv.popFirst(), 3);
}

TEST_F(LinkedListTest, PopLastReturnsGoodValues){
    ASSERT_EQ(intv.popLast(), 3);
    ASSERT_EQ(intv.popLast(), 2);
    ASSERT_EQ(intv.popLast(), 1);
}


TEST_F(LinkedListTest, PopThrowesExceptionWhenEmpty){
    ASSERT_ANY_THROW(doublev.popFirst());
    ASSERT_ANY_THROW(doublev.popLast());

    for(int i =0; i<3;i++)
        intv.popLast();

    ASSERT_ANY_THROW(intv.popFirst());
    ASSERT_ANY_THROW(intv.popLast());
}

TEST_F(LinkedListTest, RangeBasedLoopWorks){
    int j = 1;
    for(auto& i : intv){
        ASSERT_EQ(i, j);
        j++;
    }
}

TEST_F(LinkedListTest, SizeIsIncrementedAfterInsert){
    for(int i = 0; i < 15; i++)
        doublev.insert(doublev.begin(), 10.1 + i);

    ASSERT_EQ(doublev.getSize(), 15);
}



TEST_F(LinkedListTest, InsertingIntoVariousPositions){
    int aa = 10, bb = 20, cc = 30;

    intv.insert(intv.begin(), aa);
    EXPECT_EQ(intv[0], aa);

    intv.insert(intv.begin() + 2, bb);
    EXPECT_EQ(intv[2], bb);

    intv.insert(intv.begin() + 4, cc);
    EXPECT_EQ(intv[4], cc);
}

TEST_F(LinkedListTest, InsertingIntoVariousPositionsWithCapacityIncrease){
    for(int i = 0; i < 129; i ++)
        doublev.insert(doublev.end(), i);

    for(int i = 0; i < 129; i ++)   
        EXPECT_EQ(doublev[i], i);
}

TEST_F(LinkedListTest, EraseSingleElemets){
    intv.erase(intv.begin());
    ASSERT_EQ(intv[0], 2);

    intv.erase(intv.begin());
    ASSERT_EQ(intv[0], 3);

    intv.erase(intv.begin());
    ASSERT_ANY_THROW(intv[0]);
}

TEST_F(LinkedListTest, EraseMultipleElemets){
    intv.erase(intv.begin(), intv.begin() + 2);
    ASSERT_EQ(intv[0], 3);

    intv.erase(intv.begin(), intv.end());
    ASSERT_ANY_THROW(intv[0]);
}

TEST_F(LinkedListTest, EraseMultipleElemetsOneElement){
    intv.erase(intv.begin(), intv.begin()+1);
    ASSERT_EQ(intv[0], 2);
}

TEST_F(LinkedListTest, GivenEndIterator_WhenDereferencing_ThenOperationThrows)
{
  LinkedList<int> collection;

  ASSERT_THROW(*collection.end(), std::out_of_range);
  ASSERT_THROW(*collection.cend(), std::out_of_range);
}

