#include <iostream>
#include <memory>
using namespace std;

void test_shared_ptr() {
	int a = 10;
	shared_ptr<int> ptrA = make_shared<int>(30);
	shared_ptr<int> ptrA2(ptrA);
	cout << ptrA.use_count() << endl;

	int b = 20;
	int* pb = &a;
	shared_ptr<int> ptrB = make_shared<int>(b);
	ptrA2 = ptrB;
	pb = ptrB.get();

	cout << ptrA.use_count() << endl;
	cout << ptrB.use_count() << endl;
	cout << *ptrA.get() << endl;
	cout << *pb << endl;
	
	return;
}

void test_unique_ptr() {
	unique_ptr<int> uptr(new int(10));
	// unique_ptr<int> uptr2 = uptr;  //不能赋值
	// unique_ptr<int> uptr2(uptr);  //不能拷贝
	unique_ptr<int> uptr2 = move(uptr);
	cout << *uptr2.get();
	uptr2.release();
	return;
}

void test_weak_ptr() {
	shared_ptr<int> sh_ptr = make_shared<int>(10);
	cout << sh_ptr.use_count() << endl;

	weak_ptr<int> wp(sh_ptr);
	cout << wp.use_count() << endl;

	if (!wp.expired()) {
		shared_ptr<int> sh_ptr2 = wp.lock();
		*sh_ptr = 100;
		cout << wp.use_count() << endl;
	}
	return;
}

template<typename T>
class SmartPointer {
private:
	T* _ptr;
	size_t* _count;
public:
	SmartPointer(T* ptr = nullptr) : _ptr(ptr) {
		if (_ptr) {
			_count = new size_t(1);
		}
		else {
			_count = new size_t(0);
		}
	}
	SmartPointer(const SmartPointer& ptr) {
		if (this != &ptr) {
			this->_ptr = ptr._ptr;
			this->_count = ptr._count;
			(*this->_count)++;
		}
	}

	SmartPointer& operator=(const SmartPointer& ptr) {
		if (this->_ptr == ptr._ptr) {
			return *this;
		}
		if (this->_ptr) {
			(*this->_count)--;
			if (this->_count == 0) {
				delete this->_ptr;
				delete this->_count;
			}
		}
		this->_ptr = ptr._ptr;
		this->_count = ptr._count;
		(*this->_count)++;
		return *this;
	}

	T& operator*() {
		assert(this->_ptr == nullptr);
		return *(this->_ptr);
	}

	T* operator->() {
		assert(this->_ptr == nullptr);
		return this->_ptr;
	}

	~SmartPointer() {
		(*this->_count)--;
		if (*this->_count == 0) {
			delete this->_ptr;
			delete this->_count;
		}
	}

	size_t use_count() {
		return *this->_count;
	}
};

int main() {
	// test_shared_ptr();
	// test_unique_ptr();
	// test_weak_ptr();
	SmartPointer<int> sp(new int(10));
	SmartPointer<int> sp2(sp);
	cout << "sp.use_count: " << sp.use_count() << endl;
	SmartPointer<int> sp3(new int(20));
	sp2 = sp3;
	cout << "sp.use_count: " << sp.use_count() << endl;
	cout << "sp3.use_count: " << sp3.use_count() << endl;
}