//Array.cpp
#include"Array.h"
#include<iostream>
using namespace std;

#if 0
int Compare(void* one, void* other);

int main(int argc, char* argv[]) {
	Array<int> array(2);
	Long index;
	int object;
	Long(*indexes);
	Long count;
	Array<int> one;
	Array<int> other;
	Long key;
	Long i;

	index = array.Store(0, 10);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.Store(1, 20);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.Insert(1, 40);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.AppendForFront(50);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.AppendForRear(5);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.Delete(2);
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.DeleteForFront();
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.DeleteForRear();
	if (index == -1) {
		cout << "지워졌습니다" << endl;
	}

	array.Clear();


	index = array.AppendForFront(10);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.AppendForRear(5);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.Insert(1, 40);
	object = array.GetAt(index);
	cout << object << endl;

	index = array.Modify(2, 10);
	object = array.GetAt(index);
	cout << object << endl;

	key = 40;
	index = array.LinearSearchUnique(&key, Compare);
	cout << index << endl;

	cout << "LinearSearchDuplicate" << endl;
	key = 10;
	array.LinearSearchDuplicate(&key, &indexes, &count, Compare);
	i = 0;
	while (i < count) {
		object = array.GetAt(indexes[i]);
		cout << object << endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
		indexes = 0;	
	}


	cout << "SelectionSort" << endl;
	array.SelectionSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << endl;
		i++;
	}


	cout << "BinarySearchDuplicate" << endl;
	key = 10;
	array.BinarySearchDuplicate(&key, &indexes, &count, Compare);
	i = 0;
	while (i < count) {
		object = array.GetAt(indexes[i]);
		cout << object << endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
		indexes = 0;
	}

	index = array.Modify(1, 7);
	object = array.GetAt(index);
	cout << object << endl;

	cout << "BubbleSort" << endl;
	array.BubbleSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << endl;
		i++;
	}

	cout << "BinarySearchUnique" << endl;
	key = 10;
	index = array.BinarySearchUnique(&key, Compare);
	cout << index << endl;

	index = array.AppendForRear(5);
	object = array.GetAt(index);
	cout << object << endl;


	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << endl;
		i++;
	}


	cout << "InsertSort" << endl;
	array.InsertSort(Compare);
	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << endl;
		i++;
	}

	cout << "One" << endl;
	index = one.Store(0, 10);
	object = one.GetAt(index);
	cout << object << endl;

	index = one.Store(1, 20);
	object = one.GetAt(index);
	cout << object << endl;

	cout << "Other" << endl;
	index = other.Store(0, 5);
	object = other.GetAt(index);
	cout << object << endl;

	index = other.Store(1, 15);
	object = other.GetAt(index);
	cout << object << endl;

	index = other.Store(2, 40);
	object = other.GetAt(index);
	cout << object << endl;

	cout << "Merge" << endl;
	array.Merge(one, other, Compare);
	i = 0;
	while (i < array.GetLength()) {
		object = array.GetAt(i);
		cout << object << endl;
		i++;
	}

	cout << "One" << endl;
	cout << one[0] << endl;

	cout << "other" << endl;
	other = one;
	i = 0;
	while (i < other.GetLength()) {
		object = other.GetAt(i);
		cout << object << endl;
		i++;
	}
	return 0;
}

int Compare(void* one, void* other) {
	int* one_ = (int*)one;
	int* other_ = (int*)other;
	int ret;

	if (*one_ == *other_) {
		ret = 0;
	}
	else if (*one_ > * other_) {
		ret = 1;
	}
	else if (*one_ < *other_) {
		ret = -1;
	}
	return ret;
}


#endif




















