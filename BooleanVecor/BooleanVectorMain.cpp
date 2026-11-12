#include "BooleanVector.h"
#include <iostream>

int main()
{
    std::cout << "=== 1. Default ctor ===\n";
    {
        BooleanVector v;
        std::cout << "bits=" << v.getBitsCount() << " weight=" << v.getWeight() << "\n";
    }

    std::cout << "\n=== 2. Ctor(numBits, value) ===\n";
    {
        BooleanVector a(10, false);
        std::cout << "false: bits=" << a.getBitsCount() << " weight=" << a.getWeight() << "\n";

        BooleanVector b(10, true);
        std::cout << "true:  bits=" << b.getBitsCount() << " weight=" << b.getWeight() << "\n";

        BooleanVector c(0, true);
        std::cout << "zero:  bits=" << c.getBitsCount() << " weight=" << c.getWeight() << "\n";
    }

    std::cout << "\n=== 3. Ctor from const char* ===\n";
    {
        BooleanVector v("10110");
        std::cout << "bits=" << v.getBitsCount() << " weight=" << v.getWeight() << "\n";
    }

    std::cout << "\n=== 4. Copy ctor ===\n";
    {
        BooleanVector a("1010");
        BooleanVector b(a);
        std::cout << "copy: bits=" << b.getBitsCount() << " weight=" << b.getWeight() << "\n";
    }

    std::cout << "\n=== 5. operator= ===\n";
    {
        BooleanVector a("1010");
        BooleanVector b;
        b = a;
        std::cout << "assign: bits=" << b.getBitsCount() << " weight=" << b.getWeight() << "\n";
    }

    std::cout << "\n=== 6. getBitsCount / getWeight ===\n";
    {
        BooleanVector v("1010110011");
        std::cout << "bits=" << v.getBitsCount() << " weight=" << v.getWeight() << "\n";
    }

    std::cout << "\n=== 7. swap ===\n";
    {
        BooleanVector a("1010");
        BooleanVector b("11110000");
        a.swap(b);
        std::cout << "a: bits=" << a.getBitsCount() << " weight=" << a.getWeight() << "\n";
        std::cout << "b: bits=" << b.getBitsCount() << " weight=" << b.getWeight() << "\n";
    }

    std::cout << "\n=== 8. operator[] (read) ===\n";
    {
        BooleanVector v("10110");
        const BooleanVector& cv = v;
        std::cout << "cv[0]=" << cv[0] << " cv[1]=" << cv[1] << " cv[4]=" << cv[4] << "\n";
    }

    std::cout << "\n=== 9. operator[] (write, BitReference) ===\n";
    {
        BooleanVector v(8, false);
        v[0] = true;
        v[7] = true;
        std::cout << "v[0]=" << v[0] << " v[7]=" << v[7] << " weight=" << v.getWeight() << "\n";
    }

    std::cout << "\n=== 10. setValue ===\n";
    {
        BooleanVector v(8, false);
        v.setValue(3, true);
        std::cout << "v[3]=" << v[3] << " weight=" << v.getWeight() << "\n";
        v.setValue(3, false);
        std::cout << "v[3]=" << v[3] << " weight=" << v.getWeight() << "\n";
    }

    std::cout << "\n=== 11. setRange ===\n";
    {
        BooleanVector v(16, false);
        v.setRange(3, 5, true);
        std::cout << v << "\n";
    }

    std::cout << "\n=== 12. setAll ===\n";
    {
        BooleanVector v(10, false);
        v.setAll(true);
        std::cout << "all true:  weight=" << v.getWeight() << "\n";
        v.setAll(false);
        std::cout << "all false: weight=" << v.getWeight() << "\n";
    }

    std::cout << "\n=== 13. invert(bit) ===\n";
    {
        BooleanVector v("10110");
        v.invert(1);
        std::cout << v << "\n";
    }

    std::cout << "\n=== 14. invertRange ===\n";
    {
        BooleanVector v(10, false);
        v.invertRange(2, 5);
        std::cout << v << "\n";
    }

    std::cout << "\n=== 15. invertAll ===\n";
    {
        BooleanVector v("10101010");
        v.invertAll();
        std::cout << v << "\n";
    }

    std::cout << "\n=== 16. operator~ ===\n";
    {
        BooleanVector v("10110");
        std::cout << "~v = " << (~v) << "\n";
    }

    std::cout << "\n=== 17. operator& / &= ===\n";
    {
        BooleanVector a("1100");
        BooleanVector b("1010");
        std::cout << "a&b = " << (a & b) << "\n";
        BooleanVector c = a;
        c &= b;
        std::cout << "c&=b: " << c << "\n";
    }

    std::cout << "\n=== 18. operator| / |= ===\n";
    {
        BooleanVector a("1100");
        BooleanVector b("1010");
        std::cout << "a|b = " << (a | b) << "\n";
        BooleanVector c = a;
        c |= b;
        std::cout << "c|=b: " << c << "\n";
    }

    std::cout << "\n=== 19. operator^ / ^= ===\n";
    {
        BooleanVector a("1100");
        BooleanVector b("1010");
        std::cout << "a^b = " << (a ^ b) << "\n";
        BooleanVector c = a;
        c ^= b;
        std::cout << "c^=b: " << c << "\n";
    }

    std::cout << "\n=== 20. shifts ===\n";
    {
        BooleanVector a("1011");
        std::cout << "a     = " << a << "\n";
        std::cout << "a<<1  = " << (a << 1) << "\n";
        std::cout << "a<<2  = " << (a << 2) << "\n";
        std::cout << "a>>1  = " << (a >> 1) << "\n";
        std::cout << "a>>2  = " << (a >> 2) << "\n";

        BooleanVector b = a;
        b <<= 1;
        std::cout << "b<<=1 = " << b << "\n";
        b = a;
        b >>= 1;
        std::cout << "b>>=1 = " << b << "\n";
    }

    std::cout << "\n=== 21. BitReference full ===\n";
    {
        BooleanVector v(8, false);
        v[0] = true;
        v[1] = v[0];
        v[2] |= true;
        v[3] = true;
        v[3] &= false;
        v[4] = false;
        v[4] ^= true;
        v[5] = true;
        v[5].flip();
        std::cout << "v = " << v << "\n";
        std::cout << "v[0]==true: " << (v[0] == true) << "\n";
        std::cout << "v[0]!=v[1]: " << (v[0] != v[1]) << "\n";
    }

    std::cout << "\n=== 22. operator<< / operator>> ===\n";
    {
        BooleanVector v("10110");
        std::cout << "out: " << v << "\n";

        BooleanVector w;
        std::cout << "in (>): ";
        std::cin >> w;
        std::cout << "read: " << w << " bits=" << w.getBitsCount() << "\n";
    }

    return 0;
}
