#include <iostream>

using namespace std;
class Base {
    private:
        int pvt = 1;
    protected:
        int prot = 2;
    public:
        int pub = 3;
    // function to access private member
    int getPVT() {
        return pvt;
    }
};

class PublicDerived1 : public Base {
    public:
    // function to access protected member
    int getProt() {
        return prot;
    }
};

class PublicDerived2 : protected Base {
    public:
    // function to access protected member
    int getProt() {
        return prot;
    }
    // function to access public member
    int getPub() { 
        return pub;
    }
};

class PublicDerived3 : private Base {
    public:
    // function to access protected member
    int getProt() {
        return prot;
    }
    // function to access public member
    int getPub() {
        return pub;
    }
};

int main(void) {
    PublicDerived1 Object1;
    PublicDerived2 Object2;
    PublicDerived3 Object3;

    Object1.pub = 10; // OK: public inheritance
    // Object1.pvt; // ERROR: private member of Base is not accessible in PublicDerived1
    // Object1.prot; // ERROR: protected member of Base is not accessible in PublicDerived1
    cout << "Object1.pub: " << Object1.pub << endl; // OK: public inheritance
    cout << "Object1.prot: " << Object1.getProt() << endl; // OK: public inheritance
    cout << "Object1.pvt: " << Object1.getPVT() << endl; // OK: public inheritance

    // Object2.pub = 20; // ERROR: public member of Base becomes protected in PublicDerived2
    // Object2.pvt; // ERROR: private member of Base is not accessible in PublicDerived2
    // Object2.prot; // ERROR: protected member of Base is not accessible
    cout << "Object2.prot: " << Object2.getProt() << endl; // OK: protected inheritance
    cout << "Object2.pub: " << Object2.getPub() << endl; // OK: protected inheritance
    // cout << "Object2.pvt: " << Object2.getPVT() << endl; // ERROR: private member of Base is not accessible in PublicDerived2

    // Object3.pub = 30; // ERROR: public member of Base becomes private in PublicDerived3
    // Object3.pvt; // ERROR: private member of Base is not accessible in PublicDerived3
    // Object3.prot; // ERROR: protected member of Base becomes private in PublicDerived3
    cout << "Object3.prot: " << Object3.getProt() << endl; // OK: private inheritance
    cout << "Object3.pub: " << Object3.getPub() << endl; // OK: private inheritance
    // cout << "Object3.pvt: " << Object3.getPVT() << endl; // ERROR: private member of Base is not accessible in PublicDerived3
    
    return 0;
}