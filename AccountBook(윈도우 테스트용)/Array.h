//Array.h
#ifndef _ARRAY_H
#define _ARRAY_H
typedef signed long int Long;
template<typename T>

class Array {
public:
	Array(Long capacity = 256); //디폴트 생성자, 매개변수 생성자.
	~Array();//소멸자.
	Array(const Array& source); //복사생성자
	Long Store(Long index, T object);
	Long Insert(Long index, T object);
	Long AppendForFront(T object);
	Long AppendForRear(T object);
	Long Delete(Long index);
	Long DeleteForFront();
	Long DeleteForRear();
	void Clear();
	Long Modify(Long index, T object);
	Long LinearSearchUnique(void* key, int(*compare)(void*, void*));
	void LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	Long BinarySearchUnique(void* key, int(*compare)(void*, void*));
	void BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*));
	void SelectionSort(int(*compare)(void*, void*));
	void BubbleSort(int(*compare)(void*, void*));
	void InsertSort(int(*compare)(void*, void*));
	void Merge(const Array& one, const Array& other, int(*compare)(void*, void*));

	T& GetAt(Long index);
	T& operator[](Long index);
	T* operator+(Long index);
	Array& operator=(const Array& source);

	Long GetLength() const;
	Long GetCapacity() const;

private:
	T(*front);
	Long capacity;
	Long length;
};

template<typename T>
Array<T>::Array(Long capacity) {//디폴트 생성자, 매개변수 생성자.
	this->front = new T[capacity];
	this->capacity = capacity;
	this->length = 0;
}

template<typename T>
Array<T>::~Array() {//소멸자
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
}

template<typename T>
Array<T>::Array(const Array& source) { //복사생성자
	Long i = 0;

	this->front = new T[source.capacity];
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

template<typename T>
Long Array<T>::Store(Long index, T object) {
	this->front[index] = object;
	this->length++;
	
	return index;
}

template<typename T>
Long Array<T>::Insert(Long index, T object) {
	T(*temps);
	Long i = 0;
	Long j = 0;

	//1. index, object를 입력받는다.
	//2. temps에 capacity+1만큼 배열을 할당한다.
	temps = new T[this->capacity + 1];
	
	//3. array.front의 값을 index번째를 제외하고 temps에 옮겨적는다.
	while (i < index) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	j++;
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}

	//4. array.front배열을 할당해제한다.
	if (this->front != 0) {
		delete[] this->front;
	}
	this->front = temps;

	//5. capacity를 1씩 증가시킨다.
	this->capacity++;

	//6. index번째에 object를 끼운다.
	this->front[index] = object;

	//7. length를 1씩 증가시킨다.
	this->length++;

	//8. index를 출력한다,
	return index;

	//9. 끝낸다.
}

template<typename T>
Long Array<T>::AppendForFront(T object) {
	Long index = 0;
	T(*temps);
	Long i = 0;
	Long j = 1;

	//1. object를 입력받는다.
	//2. temps에 capacity+1만큼 배열을 할당한다.
	temps = new T[this->capacity + 1];

	//3. array.front를 temps의 두번째부터 옮겨적는다.
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	//4. array.front배열을 할당해제한다.
	if (this->front != 0) {
		delete[] this->front;
	}
	this->front = temps;

	//5. capacity를 1씩 증가시킨다.
	this->capacity++;

	//6. array의 index번째에 object를 적는다.
	this->front[index] = object;

	//7. length를 1씩 증가시킨다.
	this->length++;

	//8. index를 출력한다.
	return index;

	//9. 끝낸다.
}

template<typename T>
Long Array<T>::AppendForRear(T object) {
	T(*temps);
	Long index;
	Long i = 0;

	//1. object를 입력받는다.
	//2. temps에 capacity+1만큼 배열을 할당한다.
	temps = new T[this->capacity + 1];

	//3. array.front를 temps의 첫번째부터 옮겨적는다.
	while (i < this->length) {
		temps[i] = this->front[i];
		i++;
	}
	//4. array.front배열을 할당해제한다.
	if (this->front != 0) {
		delete[]this->front;
	}
	this->front = temps;

	//5. capacity를 1씩 증가시킨다.
	this->capacity++;
	index = this->capacity - 1;

	//6. array의 index번째에 object를 적는다.
	this->front[index] = object;

	//7. length를 1씩 증가시킨다.
	this->length++;

	//8. index를 출력한다.
	return  index;

	//9. 끝낸다.
}

template<typename T>
Long Array<T>::Delete(Long index) {
	T(*temps) = 0;
	Long i = 0;
	Long j = 0;

	if (this->capacity > 1) {
		temps = new T[this->capacity - 1];
	}
	while (i < index) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	i = index + 1;
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	if (this->capacity > 1) {
		this->front = temps;
	}
	this->capacity--;
	this->length--;
	index = -1;

	return index;
}

template<typename T>
Long Array<T>::DeleteForFront() {
	Long index;
	T(*temps) = 0;
	Long i = 1;
	Long j = 0;

	//1. capacity가 1보다 클 경우,
	if (this->capacity > 1) {
		//1.1 temps에 capacity-1만큼 배열을 할당한다.
		temps = new T[this->capacity - 1];
	}
	//2. array.front의 두번째부터 temps에 옮겨적는다.
	while (i < this->length) {
		temps[j] = this->front[i];
		j++;
		i++;
	}
	//3. array.front를 할당해제한다.
	if (this->front != 0) {
		delete[] this->front;
		this->front = 0;
	}
	if (this->capacity > 1) {//capacity가 1보다 더 클경우 temps와 잇는다.
		this->front = temps;
	}
	//4. capacity를 1씩 뺀다.
	this->capacity--;

	//5. length를 1씩 뺀다.
	this->length--;

	//6. index에 0을 할당한다.
	index = -1;

	//7. index를 출력한다.
	return index;

	//8. 끝낸다.
}

template<typename T>
Long Array<T>::DeleteForRear() {
	T(*temps) = 0;
	Long index;
	Long i = 0;

	//1. capacity가 1보다 클 경우,
	if (this->capacity > 1) {
		//1.1 temps에 capacity-1만큼 배열을 할당한다.
		temps = new T[this->capacity - 1];
	}
	//2. length-1만큼 array.front를 temps에 옮겨적는다.
	while (i < this->capacity - 1) {
		temps[i] = this->front[i];
		i++;
	}
	//3. array배열을 할당해제한다.
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	if (this->capacity > 1) {
		this->front = temps;
	}
	//4. capacity를 1씩 감소시킨다.
	this->capacity--;

	//5. length를 1씩 감소시킨다.
	this->length--;

	//6. index에 0을 적는다.
	index = -1;

	//7. index를 출력한다.
	return index;

	//8. 끝낸다.
}

template<typename T>
void Array<T>::Clear() {
	//1. array를 할당해제한다.
	if (this->front != 0) {
		delete[]this->front;
		this->front = 0;
	}
	//2. capacity를 0으로 원위치한다.
	this->capacity = 0;
	
	//3. length를 0으로 원위치한다.
	this->length = 0;

	//4. 끝내다.
}

template<typename T>
Long Array<T>::Modify(Long index, T object) {
	//1. index, object를 입력받는다.
	//2. array.front의 index번째에 object를 적는다.
	this->front[index] = object;

	//3. index를 출력한다.
	return index;
	//4. 끝낸다.	
}

template<typename T>
Long Array<T>::LinearSearchUnique(void* key, int(*compare)(void*, void*)) {
	Long index = -1;
	Long i = 0;

	//1. key값을 입력받는다.
	//2. index가 0이고, length보다 작거나 같은 동안 반복한다.
	while (i < this->length && compare(this->front + i, key) != 0) {
		i++;
	}
	if (i < this->length) {
		index = i;
	}
	return index;
}

template<typename T>
void Array<T>::LinearSearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	*count = 0;

	//1. key값을 입력받는다.
	//2. indexes에 length만큼 배열을 할당한다.
	*indexes = new Long[this->length];
	//3. length만큼 반복한다.
	while (i < this->length) {
		if (compare(this->front + i, key) == 0) {
			//3.1 key와 array값이 같으면, 위치를 indexes에 적는다.
			(*indexes)[j] = i;
			j++;
			//3.2 key와 array값이 같으면, 개수를 센다.
			(*count)++;
		}
		i++;
	}
	//4. indexes와 count를 출력한다.
	//5. 끝낸다.
}

template<typename T>
Long Array<T>::BinarySearchUnique(void* key, int(*compare)(void*, void*)) {
	Long index = -1;
	Long high = this->length - 1;
	Long low = 0;
	Long middle;

	//1. key값을 입력받는다.
	//2. low가 high보다 같거나 작고, array값이 key가 아닌동안 반복한다.
	middle = (low + high) / 2;
	while (low <= high && compare(this->front + middle, key) != 0) {
		if (compare(this->front + middle, key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	if (low <= high) {
		index = middle;
	}
	return index;
}





template<typename T>
void Array<T>::BinarySearchDuplicate(void* key, Long* (*indexes), Long* count, int(*compare)(void*, void*)) {
	Long low = 0;
	Long high = this->length - 1;
	Long middle;
	Long i;
	Long j = 0;
	*count = 0;

	//1. key값을 입력받는다.
	//2. indexes에 length만큼 배열을 할당한다,
	*indexes = new Long[this->length];
	middle = (low + high) / 2;

	//3. low가 high보다 같거나 적고, array값이 key가 아닌동안 반복한다.
	while (low <= high && compare(this->front + middle, key) != 0) {
		if (compare(this->front + middle, key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	//4. middle기준으로 이하 첨자 위치에도 같은 값이 있는지 확인한다.
	i = middle - 1;
	while (i >= 0 && compare(this->front + i, key) == 0) {
		i--;
	}
	i++;
	//5. front(i)이 값이 key와 같고 length보다 같거나 작은동안 반복한다.
	while (i < this->length && compare(this->front + i, key) == 0) {
		//5.1 key와 array값이 같으면, 위치를 indexes에 적는다.
		(*indexes)[j] = i;
		j++;
		//5.2. key와 array값이 같으면, 개수를 센다.
		(*count)++;
		i++;
	}
	//6. indexes, count를 출력한다.
	//7. 끝낸다.
}



template<typename T>
void Array<T>::SelectionSort(int(*compare)(void*, void*)) {
	T temp;
	Long index;
	Long i = 0;
	Long j;

	//1. length-1만큼 반복한다.
	while (i < this->length - 1) {
		index = i;
		j = i + 1;
		//1.1 가장 작은 수를 찾아낸다.
		while (j < this->length) {
			if (compare(this->front + j, this->front + index) < 0) {
				index = j;
			}
			j++;
		}
		temp = this->front[index];
		this->front[index] = this->front[i];
		this->front[i] = temp;
		i++;
	}
}

template<typename T>
void Array<T>::BubbleSort(int(*compare)(void*, void*)) {
	bool ret = false;
	T temp;
	Long i = 0;
	Long j;

	//1. length보다 적고 ret이 true가 아닌동안 반복한다.
	while (i < this->length - 1 && ret != true) {
		j = 0;
		ret = true;
		while (j < this->length - i - 1) {
			if (compare(this->front + j, this->front + (j + 1)) > 0) {
				temp = this->front[j];
				this->front[j] = this->front[j + 1];
				this->front[j + 1] = temp;
				ret = false;
			}
			j++;
		}
		i++;
	}
}

template<typename T>
void Array<T>::InsertSort(int(*compare)(void*, void*)) {
	T temp;
	Long i = 1;
	Long j;

	//1. length-1만큼 반복한다.
	while (i < this->length) {
		temp = this->front[i];
		j = i - 1;
		while (j >= 0 && compare(&temp, this->front + j) < 0) {
			this->front[j + 1] = this->front[j];
			j--;
		}
		this->front[j + 1] = temp;
		i++;
	}
}

template<typename T>
void Array<T>::Merge(const Array& one, const Array& other, int(*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	//1. one, other를 입력받는다.
	//2. array.front를 할당해제한다.
	if (this->front != 0) {
		delete[]this->front;
	}
	//3. one.length와 other.length의 합만큼 array에 배열을 할당한다.
	this->front = new T[one.length + other.length];
	this->capacity = one.length + other.length;
	this->length = 0;

	//4. one.length보다 같거나 작고, other.length보다 같거나 작은동안 반복한다.
	while (i < one.length && j < other.length) {
		//4.1 one과 other를 비교했을 때
		if (compare(one.front + i, other.front + j) < 0) {
			//4.1.1 더 작은수를 front에 쓴다.
			this->front[k] = one.front[i];
			this->length++;
			k++;
			i++;
		}
		else {
			this->front[k] = other.front[j];
			this->length++;
			k++;
			j++;
		}
	}
	//5. one.length보다 같거나 작을때 front에 쓴다.
	while (i < one.length) {
		this->front[k] = one.front[i];
		this->length++;
		k++;
		i++;
	}
	//6. other.length보다 같거나 작을때 front에 쓴다.
	while (j < other.length) {
		this->front[k] = other.front[j];
		this->length++;
		k++;
		j++;
	}
}

template <typename T>
T& Array<T>::operator[](Long index) {
	return this->front[index];
}

template<typename T>
T* Array<T>::operator+(Long index) {
	return this->front + index;
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& source) {

	Long i = 0;

	if (this->front != 0) {
		delete[]this->front;
	}
	this->front = new T[source.capacity];
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}



template<typename T>
inline Long Array<T>::GetLength() const {
	return this->length;
}

template<typename T>
inline Long Array<T>::GetCapacity()const{
	return this->capacity;
}



template<typename T>
T& Array<T>::GetAt(Long index) {
	//1. index를 입력받는다.
	//2. array.front의 index번째 값을 출력한다.
	return this->front[index];
}




#endif//_ARRAY_H







