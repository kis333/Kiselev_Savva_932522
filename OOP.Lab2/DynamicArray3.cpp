#include <iostream>
#include <string>
#include <utility>
#include "DynamicArray3.h"

int main()
{
    std::cout << " Constructors, length, capacity, output \n";
    DynamicArray<int> empty;
    std::cout << "Empty: " << empty << ", length=" << empty.getLength()
        << ", capacity=" << empty.getCapacity() << "\n";

    int values[] = { 5, 2, 8, 2, 1 };
    DynamicArray<int> a(values, 5);
    std::cout << "Constructed from C-array: " << a << "\n";

    DynamicArray<int> b(a); 
    std::cout << "Copy constructor: " << b << "\n";

    DynamicArray<int> moveSource;
    moveSource += 40;
    moveSource += 50;
    DynamicArray<int> moved(std::move(moveSource)); 
    std::cout << "Move constructor: " << moved
        << ", source length=" << moveSource.getLength() << "\n";

    std::cout << "\nAdd, growth, + and += \n";
    a += 10;
    a += 20;
    std::cout << "a += item: " << a << ", length=" << a.getLength()
        << ", capacity=" << a.getCapacity() << "\n";

    DynamicArray<int> extra;
    extra += 70;
    extra += 80;
    a += extra; 
    std::cout << "a += extra: " << a << "\n";

    DynamicArray<int> selfAppend;
    selfAppend += 1;
    selfAppend += 2;
    selfAppend += selfAppend; 
    std::cout << "self append: " << selfAppend << "\n";

    DynamicArray<int> plusItem = a + 99; 
    std::cout << "a + 99: " << plusItem << "; original a: " << a << "\n";
    DynamicArray<int> plusArray = a + extra; 
    std::cout << "a + extra: " << plusArray << "\n";

    std::cout << "\n Indexing, find, insert, remove \n";
    std::cout << "a[0] = " << a[0] << "\n";
    a[0] = 123; 
    std::cout << "after a[0]=123: " << a << "\n";
    std::cout << "find(2)=" << a.find(2) << ", find(999)=" << a.find(999) << "\n";
    std::cout << "insert(1,77)=" << a.insert(1, 77) << "\n";
    std::cout << "after insert: " << a << "\n";
    std::cout << "insert(invalid)=" << a.insert(999, 77) << "\n";
    std::cout << "removeAt(1)=" << a.removeAt(1) << "\n";
    std::cout << "removeAt(invalid)=" << a.removeAt(999) << "\n";
    std::cout << "remove(2)=" << a.remove(2) << "\n";
    std::cout << "remove(999)=" << a.remove(999) << "\n";

    DynamicArray<int> duplicates;
    duplicates += 4;
    duplicates += 4;
    duplicates += 7;
    duplicates += 4;
    std::cout << "before removeAll: " << duplicates << "\n";
    std::cout << "removeAll(4) count=" << duplicates.removeAll(4)
        << ", after: " << duplicates << "\n";

    std::cout << "\n Sort, minimum, maximum \n";
    DynamicArray<int> sortable;
    sortable += 9;
    sortable += 3;
    sortable += 6;
    sortable += -2;
    std::cout << "before sort: " << sortable << "\n";
    sortable.sort();
    std::cout << "after sort: " << sortable << "\n";
    std::cout << "min=" << sortable.getMin() << ", max=" << sortable.getMax() << "\n";

    std::cout << "\nAssignments, compare, swap\n";
    DynamicArray<int> copyAssigned;
    copyAssigned = sortable; 
    std::cout << "copy assignment: " << copyAssigned << "\n";
    std::cout << "sortable == copyAssigned: " << (sortable == copyAssigned) << "\n";
    copyAssigned += 1000;
    std::cout << "sortable != copyAssigned: " << (sortable != copyAssigned) << "\n";

    DynamicArray<int> left;
    left += 11;
    DynamicArray<int> right;
    right += 22;
    right += 33;
    left.swap(right);
    std::cout << "after swap: left=" << left << ", right=" << right << "\n";

    DynamicArray<int> moveAssigned;
    moveAssigned = std::move(moved); 
    std::cout << "move assignment: " << moveAssigned
        << ", source length=" << moved.getLength() << "\n";

    std::cout << "\n Mutable iterators\n";
    DynamicArray<int> iterArray;
    iterArray += 10;
    iterArray += 20;
    iterArray += 30;

    std::cout << "begin/end traversal: ";
    for (DynamicArray<int>::iterator it = iterArray.begin();
        it != iterArray.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    DynamicArray<int>::iterator it1 = iterArray.begin();
    DynamicArray<int>::iterator it2 = iterArray.begin();
    std::cout << "iterator ==: " << (it1 == it2) << "\n";

    DynamicArray<int>::iterator post = iterArray.begin();
    DynamicArray<int>::iterator oldPost = post++;
    std::cout << "post++ old value=" << *oldPost << ", new value=" << *post << "\n";

    DynamicArray<int>::iterator pos = iterArray.begin();
    ++pos;
    iterArray.insert(pos, 15);
    std::cout << "iterator insert: " << iterArray << "\n";

    pos = iterArray.begin();
    ++pos;
    iterArray.erase(pos);
    std::cout << "iterator erase: " << iterArray << "\n";

    if (iterArray.getLength() >= 3)
    {
        DynamicArray<int>::iterator first = iterArray.begin();
        DynamicArray<int>::iterator last = iterArray.begin();
        ++first;
        ++last;
        ++last;
        iterArray.erase(first, last);
    }
    std::cout << "range erase: " << iterArray << "\n";

    std::cout << "\n Const iterators and const []\n";
    const DynamicArray<int>& constRef = iterArray;

    std::cout << "const begin/end traversal: ";
    for (DynamicArray<int>::const_iterator it = constRef.begin();
        it != constRef.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    int sum = 0;
    for (DynamicArray<int>::const_iterator it = constRef.cbegin();
        it != constRef.cend(); ++it)
        sum += *it;
    std::cout << "sum through cbegin/cend=" << sum << "\n";

    if (constRef.getLength() > 0)
        std::cout << "const operator[]: " << constRef[0] << "\n";

    DynamicArray<int>::const_iterator converted(iterArray.begin());
    if (converted != iterArray.cend())
        std::cout << "converted const iterator: " << *converted << "\n";

    DynamicArray<int>::const_iterator cit1 = constRef.cbegin();
    DynamicArray<int>::const_iterator cit2 = constRef.cbegin();
    std::cout << "const_iterator ==: " << (cit1 == cit2) << "\n";
    if (cit1 != constRef.cend())
    {
        DynamicArray<int>::const_iterator oldCit = cit1++;
        std::cout << "const post++ old=" << *oldCit;
        if (cit1 != constRef.cend())
            std::cout << ", new=" << *cit1;
        std::cout << "\n";
    }

    std::cout << "\n Keyboard input operator >> \n";
    DynamicArray<int> inputArray;
    int count = 0;
    std::cout << "How many integers will you enter? ";
    if (std::cin >> count && count > 0)
    {
        for (int i = 0; i < count; ++i)
        {
            std::cout << "Enter integer " << i + 1 << ": ";
            if (!(std::cin >> inputArray))
            {
                std::cout << "Invalid input; stopping input test.\n";
                break;
            }
        }
        std::cout << "Input array: " << inputArray << "\n";
    }
    else
        std::cout << "No input values requested.\n";

    std::cout << "\nTemplate with std::string\n";
    DynamicArray<std::string> words;
    words += std::string("hello");
    words += std::string("world");
    std::cout << "words: " << words << "\n";

    return 0;
}
