#include <iostream>
#include <utility>
#include "DynamicInt.h"

int main() {
    DynamicArray empty;
    std::cout << "Empty: " << empty << " length=" << empty.getLength()
        << " capacity=" << empty.getCapacity() << "\n";

    int values[] = { 5,2,8,2,1 };
    DynamicArray a(values, 5), copy(a);
    std::cout << "a=" << a << " copy=" << copy << "\n";

    DynamicArray source; source += 40; source += 50;
    DynamicArray moved(std::move(source));
    std::cout << "moved=" << moved << " source length=" << source.getLength() << "\n";

    a += 10; a += 20;
    DynamicArray extra;
    extra += 70;
    extra += 80;
    a += extra;
    std::cout << "a after += array: " << a << "\n";

    DynamicArray self; self += 1;
    self += 2;
    self += self;
    std::cout << "self append: " << self << "\n";

    std::cout << "a+99=" << (a + 99) << " a+extra=" << (a + extra) << "\n";
    std::cout << "a[0]=" << a[0] << " find(2)=" << a.find(2) << " find(999)=" << a.find(999) << "\n";

    a[0] = 123;
    a.insert(1, 77);
    std::cout << "after insert/index: " << a << "\n";
    a.removeAt(1);
    a.remove(2);

    DynamicArray dup;
    dup += 4;
    dup += 4;
    dup += 7;
    dup += 4;
    std::cout << "removeAll count=" << dup.removeAll(4) << " result=" << dup << "\n";

    DynamicArray sorted;
    sorted += 9;
    sorted += 3;
    sorted += 6;
    sorted += -2;
    sorted.sort();
    std::cout << "sorted=" << sorted << " min=" << sorted.getMin() << " max=" << sorted.getMax() << "\n";

    DynamicArray assigned; assigned = sorted;
    std::cout << "copy assignment equality=" << (assigned == sorted) << "\n";
    assigned += 1000;
    std::cout << "not equal=" << (assigned != sorted) << "\n";

    DynamicArray left; left += 11;
    DynamicArray right; right += 22;
    right += 33;
    left.swap(right);
    std::cout << "swap left=" << left << " right=" << right << "\n";

    DynamicArray moveAssigned; moveAssigned = std::move(moved);
    std::cout << "move assigned=" << moveAssigned << "\n";

    DynamicArray iter;
    iter += 10; iter += 20; iter += 30;

    for (DynamicArray::iterator it = iter.begin(); it != iter.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    DynamicArray::iterator it = iter.begin();
    DynamicArray::iterator old = it++;
    std::cout << "postfix old=" << *old << " new=" << *it << "\n";

    it = iter.begin();
    ++it; iter.insert(it, 15);
    it = iter.begin();
    ++it; iter.erase(it);

    if (iter.getLength() >= 3) {
        DynamicArray::iterator f = iter.begin(), l = iter.begin();
        ++f;
        ++l;
        ++l;
        iter.erase(f, l);
    }

    const DynamicArray& cref = iter;
    for (DynamicArray::const_iterator ci = cref.cbegin(); ci != cref.cend(); ++ci)
        std::cout << *ci << " ";
    std::cout << " const first (if present): ";
    if (cref.getLength()) std::cout << cref[0];
    std::cout << "\n";

    DynamicArray::const_iterator converted(iter.begin());
    (void)converted;

    DynamicArray::const_iterator ci = cref.cbegin();
    if (ci != cref.cend()) {
        DynamicArray::const_iterator oldci = ci++;
        std::cout << "const postfix old=" << *oldci << "\n";
    }

    DynamicArray input;
    int count = 0;
    std::cout << "How many integers? ";
    if (std::cin >> count && count > 0) {
        for (int i = 0; i < count; ++i) {
            std::cout << "Integer " << i + 1 << ": ";
            if (!(std::cin >> input)) break;
        }
    }
    std::cout << "Input array: " << input << "\n";
    std::cout << "All tests finished.\n";
    return 0;
}
