#include <iostream>

using namespace std;

int main(void) {
    // old version
    char c; short s; int i; long l; long long ll;
    float f; double d; long double ld;
    scanf("%hhd %hd %d %ld %lld %f %lf %Lf",
    &c, &s, &i, &l, &ll, &f, &d, &ld);

    //assume scanf works successfully
    printf("%hhd %hd %d %ld %lld %f %lf %Lf",c,s,i,l,ll,f,d,ld);

    // scanf("%d",&c); //forget to specify length modifier
    //undefined behavior
    // printf("%hd",c); //use incorrect length modifier
    //undefined behavior

    // new version
    std::cin >> c >> s >> i >> l >> ll >> f >> d >> ld;
    std::cout << c << " " << s << " " << i << " " << l << " " << ll << " " << f << " " << d << " " << ld << std::endl;

    int a; short sh; char string[10];
    cin>>a>>string>>sh; //read input and store sequentially in a, c and s
    cout<<a<<' '<<sh<<' '<<string<<endl; //print a, s and c sequentially
    // input: 1 abcd 2
    //output: 1 2 abcd

    double pi = 3.1415926534;
    std::cout.precision(5);
    std::cout << pi << '\n';

    cin>>c; //cin and cout are objects of istream and ostream class respectively, so they can be used as variables
    cout << (int)c;
    cout << static_cast<int>(c);
    return 0;
}