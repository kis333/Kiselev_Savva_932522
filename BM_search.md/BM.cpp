#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> buildTable(const string& p) {
    int m = (int)p.size();
    vector<int> T(256, m);          

    for (int i = 0; i < m - 1; i++) {
        T[(unsigned char)p[i]] = m - 1 - i;
    }
    return T;
}


int BMfindFirst(const string& s, const string& p) {
    int n = (int)s.size();
    int m = (int)p.size();
    if (m == 0) return 0;
    if (m > n)  return -1;

    vector<int> T = buildTable(p);

    int i = m - 1;                  
    while (i < n) {
        int k = i;
        int j = m - 1;

        while (j >= 0 && s[k] == p[j]) {
            k--;
            j--;
        }

        if (j < 0) {
            return i - m + 1;    
        }
        else {
            i = i + T[(unsigned char)s[i]];   
        }
    }
    return -1;
}

vector<int> BMfindAll(const string& s, const string& p) {
    vector<int> res;
    int n = (int)s.size();
    int m = (int)p.size();
    if (m == 0 || m > n) return res;

    vector<int> T = buildTable(p);

    int i = m - 1;
    while (i < n) {
        int k = i;
        int j = m - 1;

        while (j >= 0 && s[k] == p[j]) {
            k--;
            j--;
        }

        if (j < 0) {
            res.push_back(i - m + 1);
            i = i + 1;             
        }
        else {
            i = i + T[(unsigned char)s[i]];
        }
    }
    return res;
}

vector<int> BMfindAll(const string& s, const string& p, int begin, int end) {
    vector<int> res;
    int n = (int)s.size();
    int m = (int)p.size();
    if (m == 0 || m > n) return res;

    if (begin < 0) begin = 0;
    if (end >= n)  end = n - 1;
    if (begin > end) return res;

    vector<int> T = buildTable(p);

    int i = m - 1;
    if (begin + m - 1 > i) i = begin + m - 1;

    while (i <= end) {
        int k = i;
        int j = m - 1;

        while (j >= 0 && k >= begin && s[k] == p[j]) {
            k--;
            j--;
        }

        if (j < 0) {
            int start = i - m + 1;
            if (start >= begin && i <= end) {
                res.push_back(start);
            }
            i = i + 1;
        }
        else {
            int shift = T[(unsigned char)s[i]];
            if (i + shift < begin + m - 1) {
                i = begin + m - 1;
            }
            else {
                i = i + shift;
            }
        }
    }
    return res;
}

void print(const vector<int>& v) {
    cout << "[";
    for (int i = 0; i < (int)v.size(); i++) {
        cout << v[i];
        if (i + 1 < (int)v.size()) cout << ", ";
    }
    cout << "]";
}


int main() {
    string s, p;
    int begin, end;

    cout << "enter text: ";
    getline(cin, s);

    cout << "enter sample: ";
    getline(cin, p);

    cout << "enter begin: ";
    cin >> begin;
    cout << "enter end:   ";
    cin >> end;

    int first = BMfindFirst(s, p);
    cout << "BMfindFirst: " << first << "\n";

    cout << "BMfindAll: ";
    print(BMfindAll(s, p));
    cout << "\n";

    cout << "BMfindAll(" << begin << ", " << end << "): ";
    print(BMfindAll(s, p, begin, end));
    cout << "\n";

    return 0;
}
