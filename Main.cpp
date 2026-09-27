#include <iostream>
#include "Vector.h"
using namespace std;

int main()
{
    Vector a, b, c;

    cout << "enter vector a (x y z): ";
    a.Input();

    cout << "enter vector b (x y z): ";
    b.Input();

    cout << "enter vector c (x y z): ";
    c.Input();

    cout << "\na = "; a.Output(); 
    cout << "\nb = ";   b.Output();
    cout << "\nc = ";   c.Output(); 

    cout << "\n|a| = " << a.Lenght() << "\n";

    cout << "norm a = ";
    a.Normalize().Output();
    cout << "\n";

    cout << "a + b = ";
    (a + b).Output();
    cout << "\n";

    cout << "a - b = ";
    (a - b).Output();
    cout << "\n";

    double n;
    cout << "enter scalar 1: ";
    cin >> n;
    cout << "a * n = ";
    (a * n).Output();
    cout << "\n";

    double k;
    cout << "enter scalar 2: ";
    cin >> k;
    cout << "b * k = ";
    (b * k).Output();
    cout << "\n";

    cout << "a == b? ";
    if (a == b) 
        cout << "yes\n";
    else        
        cout << "no\n";

    cout << "a * b = " << a.Dot(b) << "\n";

    cout << "a x b = ";
    a.Cross(b).Output();
    cout << "\n";

    cout << "(a, b, c) = " << a.Mixed(b, c) << "\n";

    cout << "a x (b x c) = ";
    a.DoubleCross(b, c).Output();
    cout << "\n";

    return 0;
}
