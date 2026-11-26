#include <iostream>
#include <cassert>
#include <cstring>
#include "BooleanMatrix.h"


int main()
{
    
    std::cout << "1. Default constructor";
    BooleanMatrix m0;
    std::cout << "rows = " << m0.getRowsCount()
        << ", cols = " << m0.getColumnsCount()
        << ", weight = " << m0.getWeight() << "\n";

    std::cout << ("2. Constructor (rows, cols, value)");
    BooleanMatrix m1(3, 4, false);
    std::cout << "m1 (3x4, false):\n" << m1;

    BooleanMatrix m2(3, 4, true);
    std::cout << "m2 (3x4, true):\n" << m2;

    std::cout << ("3. Constructor from char matrix");
    const char* raw[3] = {
        "1010",
        "0110",
        "1111"
    };
    BooleanMatrix m3(raw, 3, 4);
    std::cout << "m3 (from char):\n" << m3;

    std::cout << ("4. Copy constructor");
    BooleanMatrix m4(m3);
    std::cout << "m4 (copy of m3):\n" << m4;
    std::cout << "m4 == m3 (visually): " << (m4.getWeight() == m3.getWeight()) << "\n";

    std::cout << ("5. Destructor");
    {
        BooleanMatrix tmp(2, 2, true);
        std::cout << "tmp created, will be destroyed at end of scope\n";
    } 
    std::cout << "tmp destroyed\n";

    std::cout << ("6. getRowsCount / getColumnsCount");
    std::cout << "m3: rows = " << m3.getRowsCount()
        << ", cols = " << m3.getColumnsCount() << "\n";

    std::cout << ("7. swap");
    BooleanMatrix a(2, 3, false);
    BooleanMatrix b(2, 3, true);
    std::cout << "before swap:\n a:\n" << a << " b:\n" << b;
    a.swap(b);
    std::cout << "after swap:\n a:\n" << a << " b:\n" << b;

    std::cout << ("8. operator<<");
    std::cout << "m3 via operator<<:\n" << m3;

    std::cout << ("9. operator>>");
    std::cout << "Enter matrix: rows cols then bits:\n";
    BooleanMatrix inputM;
    if (std::cin >> inputM)
        std::cout << "read:\n" << inputM;
    else
        std::cout << "read failed (skipped)\n";
    std::cin.clear();

    std::cout << ("10. getWeight");
    std::cout << "m3 weight = " << m3.getWeight() << "\n";

    std::cout << ("11. getAllRowsConjunction");
    BooleanVector conj = m3.getAllRowsConjunction();
    std::cout << "conjunction = " << conj << "\n";

   
    std::cout << ("12. getAllRowsDisjunction");
    BooleanVector disj = m3.getAllRowsDisjunction();
    std::cout << "disjunction = " << disj << "\n";

    
    std::cout << ("13. getRowWeight");
    for (uint32_t i = 0; i < m3.getRowsCount(); ++i)
        std::cout << "row " << i << " weight = " << m3.getRowWeight(i) << "\n";

    std::cout << ("14. invertComponent");
    BooleanMatrix m14(m3);
    std::cout << "before:\n" << m14;
    m14.invertComponent(0, 1);
    std::cout << "after invertComponent(row=0, col=1):\n" << m14;


    std::cout << ("15. invertComponents");
    BooleanMatrix m15(m3);
    std::cout << "before:\n" << m15;
    m15.invertComponents(1, 0, 2); 
    std::cout << "after invertComponents(row=1, col=0, count=2):\n" << m15;

    std::cout << ("16. setComponent");
    BooleanMatrix m16(m3);
    std::cout << "before:\n" << m16;
    m16.setComponent(0, 2, true);
    std::cout << "after setComponent(row=0, col=2, true):\n" << m16;
    m16.setComponent(0, 2, false);
    std::cout << "after setComponent(row=0, col=2, false):\n" << m16;

    std::cout << ("17. setComponents");
    BooleanMatrix m17(m3);
    std::cout << "before:\n" << m17;
    m17.setComponents(2, 1, 2, false);
    std::cout << "after setComponents(row=2, col=1, count=2, false):\n" << m17;

    std::cout << ("18. operator=");
    BooleanMatrix m18;
    m18 = m3;
    std::cout << "m18 = m3:\n" << m18;

    std::cout << ("19. operator[]");
    std::cout << "m3[0] = " << m3[0] << "\n";
    std::cout << "m3[1] = " << m3[1] << "\n";
    m3[0][0] = !m3[0][0]; 
    std::cout << "after m3[0][0] flip:\n" << m3;
    m3[0][0] = !m3[0][0]; 

   
    std::cout << ("20. operator& / &=");
    BooleanMatrix x(2, 3, false);
    BooleanMatrix y(2, 3, true);
    x.setComponent(0, 0, true);
    x.setComponent(1, 2, true);
    std::cout << "x:\n" << x << "y:\n" << y;
    std::cout << "x & y:\n" << (x & y);
    x &= y;
    std::cout << "x &= y:\n" << x;

    std::cout << ("21. operator| / |=");
    BooleanMatrix p(2, 3, false);
    BooleanMatrix q(2, 3, false);
    q.setComponent(1, 1, true);
    std::cout << "p:\n" << p << "q:\n" << q;
    std::cout << "p | q:\n" << (p | q);
    p |= q;
    std::cout << "p |= q:\n" << p;

    
    std::cout << ("22. operator^ / ^=");
    BooleanMatrix u(2, 3, true);
    BooleanMatrix v(2, 3, true);
    v.setComponent(0, 1, false);
    std::cout << "u:\n" << u << "v:\n" << v;
    std::cout << "u ^ v:\n" << (u ^ v);
    u ^= v;
    std::cout << "u ^= v:\n" << u;


    std::cout << ("23. operator~");
    std::cout << "m3:\n" << m3;
    std::cout << "~m3:\n" << (~m3);

    std::cout << ("Extra checks");
    assert(m3.getRowsCount() == 3);
    assert(m3.getColumnsCount() == 4);
    assert((~(~m3)).getWeight() == m3.getWeight());
    assert((m3 & m3).getWeight() == m3.getWeight());
    assert((m3 | m3).getWeight() == m3.getWeight());
    assert((m3 ^ m3).getWeight() == 0);

    return 0;
}