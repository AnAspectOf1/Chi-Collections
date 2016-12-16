#include <chi/ptr.h>


using namespace chi;

class Base {
public:
	int number;

	Base( int num = 0 ) : number(num) {}
};

class Derived : public Base {
public:
	int another;

	Derived( int a = 0, int b = 0 ) : Base(a), another(b) {}
};


class MyClass {
public:
	ManPtr<Base> ptr;

	MyClass() {}
	MyClass( int var ) : ptr(var) {}
};


int main() {

	ManPtr<MyClass> ptr;
	ptr.alloc();
	ptr->ptr.assign( Derived( 34, 98 ) );

	ManPtr<MyClass> ptr2; ptr2.take( ptr );

	Base* int_ptr = ptr2->ptr;
	Derived* int_ptr2 = (Derived*)int_ptr;
	Derived derived = ptr2->ptr.val<Derived>();
	printf( "test %d %d\n", derived.number, derived.another );

	return 0;
}
