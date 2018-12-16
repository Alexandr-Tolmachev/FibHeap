#include "pch.h"
#include <iostream>
#include <gtest/gtest.h>
#include "../FibHeap/FibbonachiHeap.h"
#include <string>
#include <vector>
#include <set>
#include <ctime>


TEST(FibbonachiHeapBasic, 1_Constructor) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	EXPECT_EQ(heap.is_empty(), true);
}

TEST(FibbonachiHeapBasic, 2_Empty) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	EXPECT_EQ(heap.is_empty(), true);
	heap.insert(1);
	EXPECT_EQ(heap.is_empty(), false);
}

TEST(FibbonachiHeapBasic, 3_Insert) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	auto pointer = heap.insert(1);
	EXPECT_EQ(pointer.value(), 1);
}

TEST(FibbonachiHeapBasic, 4_GetMin) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	auto pointer = heap.insert(1);
	heap.insert(2);
	heap.insert(5);
	heap.insert(0);
	heap.insert(3);
	EXPECT_EQ(heap.get_min(), 0);
}

TEST(FibbonachiHeapBasic, 5_ExtractMin) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	heap.insert(2);
	heap.insert(3);
	heap.insert(5);
	heap.insert(10);
	heap.insert(3);
	heap.insert(11);
	EXPECT_EQ(heap.extract_min(), 2);
	EXPECT_EQ(heap.extract_min(), 3);
	EXPECT_EQ(heap.extract_min(), 3);
	heap.insert(1);
	EXPECT_EQ(heap.extract_min(), 1);
	EXPECT_EQ(heap.extract_min(), 5);
	EXPECT_EQ(heap.extract_min(), 10);
	EXPECT_EQ(heap.extract_min(), 11);
	EXPECT_EQ(heap.size(), 1);
}

TEST(FibbonachiHeapBasic, 6_Size) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	EXPECT_EQ(heap.size(), 0);
	heap.insert(1);
	EXPECT_EQ(heap.size(), 1);
	heap.insert(2);
	heap.insert(6);
	heap.insert(1);
	EXPECT_EQ(heap.size(), 4);
	heap.extract_min();
	heap.extract_min();
	EXPECT_EQ(heap.size(), 2);
}

TEST(FibbonachiHeapAdvance, 1_Change) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	heap.insert(1);
	heap.insert(3);
	auto pointer_to_2 = heap.insert(2);
	heap.insert(1);
	heap.decrease_key(pointer_to_2, 0);
	EXPECT_EQ(pointer_to_2.value(), 0);
	EXPECT_EQ(heap.get_min(), 0);
	heap.decrease_key(pointer_to_2, -1);
	EXPECT_EQ(pointer_to_2.value(), -1);
	EXPECT_EQ(heap.get_min(), -1);
}


TEST(FibbonachiHeapAdvance, 2_MergeEmptyToNonEmpty) {
	FibbonachiHeap<int> heap1 = FibbonachiHeap<int>();
	FibbonachiHeap<int> heap2 = FibbonachiHeap<int>();
	heap1.insert(1);
	heap1.insert(3);
	heap1.merge(heap2);
	EXPECT_EQ(heap1.get_min(), 1);
	EXPECT_EQ(heap1.size(), 2);
	EXPECT_EQ(heap2.size(), 0);
}

TEST(FibbonachiHeapAdvance, 3_MergeNonEmptyToEmpty) {
	FibbonachiHeap<int> heap1 = FibbonachiHeap<int>();
	FibbonachiHeap<int> heap2 = FibbonachiHeap<int>();

	heap1.insert(1);
	heap1.insert(3);
	heap1.merge(heap2);

	EXPECT_EQ(heap1.get_min(), 1);
	EXPECT_EQ(heap1.size(), 2);
	EXPECT_EQ(heap2.size(), 0);
}

TEST(FibbonachiHeapAdvance, 4_MergeNonEmptyToNonEmpty) {
	FibbonachiHeap<int> heap1 = FibbonachiHeap<int>();
	FibbonachiHeap<int> heap2 = FibbonachiHeap<int>();

	heap1.insert(1);
	heap1.insert(3);
	heap2.insert(0);
	heap2.insert(4);
	heap1.merge(heap2);

	EXPECT_EQ(heap1.get_min(), 0);
	EXPECT_EQ(heap1.size(), 4);
	EXPECT_EQ(heap2.size(), 0);
}

TEST(FibbonachiHeapExceptions, 1_RequestsToEmptyHeap) {
	FibbonachiHeap<int> heap = FibbonachiHeap<int>();
	EXPECT_THROW(heap.get_min(), std::logic_error);
	EXPECT_THROW(heap.extract_min(), std::logic_error);
}

TEST(HeapTest, 1_LargeStressTest) {
	std::vector <int> b;
	std::multiset <int> c;
	const int TEST_SIZE = 10000;
	for (int i = 0; i < TEST_SIZE; i++) {
		b.push_back(rand());
		c.insert(b[i]);
	}
	FibbonachiHeap <int> a;
	for (int i = 0; i < TEST_SIZE; i++) {
		a.insert(b[i]);
	}

	for (int i = 0; i < TEST_SIZE; i++) {
		int r1 = a.get_min(), r2 = *c.begin();
		EXPECT_EQ(r1, r2);
		a.extract_min();
		c.erase(c.begin());
	}
}

TEST(HeapTest, 2_ConstructorSpeedTest) {
	time_t start, finish;
	start = clock();

	const int TEST_SIZE = 100000;
	int temp;
	FibbonachiHeap<int> heap;

	for (int i = 0; i < TEST_SIZE; i++) {
		temp = rand();
		heap.insert(temp);
	}

	finish = clock();
	ASSERT_GE(8, ((double)finish - start) / ((double)CLOCKS_PER_SEC));
}


TEST(HeapTest, 3_ExtractMinTest) {
	FibbonachiHeap <int> heap;

	heap.insert(1), heap.insert(3), heap.insert(2);

	ASSERT_EQ(heap.extract_min(), 1);
	ASSERT_EQ(heap.extract_min(), 2);
	ASSERT_EQ(heap.extract_min(), 3);
}

TEST(HeapTest, 4_MassPointerTest) {
	std::vector<int> test;

	int temp;
	for (int i = 0; i < 10000; i++) {
		temp = rand();
		test.push_back(temp);
	}

	FibbonachiHeap <int> heap;

	for (auto now : test) {
		heap.insert(now);
	}

	sort(test.begin(), test.end());
	for (int i = 0; i < 1000; i++) {
		ASSERT_EQ(test[i], heap.extract_min());
	}
}

TEST(HeapTest, 5_MassTest) {
	std::multiset<int> test;
	int temp;

	FibbonachiHeap <int> heap;

	for (int i = 0; i < 100000; i++) {
		temp = rand();
		heap.insert(temp);
		test.insert(temp);
	}

	for (int i = 0; i < 100000; i++) {
		ASSERT_EQ(heap.extract_min(), *test.begin());
		test.erase(test.begin());
	}
}

TEST(HeapTest, 6_TimeTest) {
	time_t start, finish;
	start = clock();
	FibbonachiHeap <int> b;
	std::multiset <int> c;

	const int TEST_SIZE = 100000;
	int temp;

	for (int i = 0; i < TEST_SIZE; i++) {
		temp = rand();
		b.insert(temp);
		c.insert(temp);
	}

	for (int i = 0; i < TEST_SIZE; i++) {
		int r1 = b.get_min(), r2 = *c.begin();
		EXPECT_EQ(r1, r2);
		b.extract_min();
		c.erase(c.begin());
	}

	finish = clock();
	ASSERT_GE(10, ((double)finish - start) / ((double)CLOCKS_PER_SEC));
}

TEST(HeapTest, 7_SpeedTest) {
	FibbonachiHeap <int> heap;
	time_t start = clock(), finish;
	const int TEST_SIZE = 100000;
	int temp;

	for (int i = 0; i < TEST_SIZE; i++) {
		temp = rand();
		heap.insert(temp);
	}

	finish = clock();
	ASSERT_GE(10, ((double)finish - start) / ((double)CLOCKS_PER_SEC));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


