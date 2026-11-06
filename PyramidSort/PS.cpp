#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <algorithm>
using namespace std;
using namespace std::chrono;

static const char* KNOWN_FILES[] = {
    "original_n10000_range_-10_10.txt",
    "original_n10000_range_-1000_1000.txt",
    "original_n10000_range_-100000_100000.txt",
    "original_n100000_range_-10_10.txt",
    "original_n100000_range_-1000_1000.txt",
    "original_n100000_range_-100000_100000.txt",
    "original_n1000000_range_-10_10.txt",
    "original_n1000000_range_-1000_1000.txt",
    "original_n1000000_range_-100000_100000.txt"
};
static const int KNOWN_FILES_COUNT = sizeof(KNOWN_FILES) / sizeof(KNOWN_FILES[0]);


bool isSorted(int* a, int n) {
    for (int i = 1; i < n; i++)
        if (a[i] < a[i - 1]) return false;
    return true;
}

void siftDown(int* a, int n, int i) {
    int x = a[i];
    int j = 2 * i + 1;
    bool go = true;
    while (j < n && go) {
        if (j + 1 < n && a[j + 1] > a[j]) j++;
        if (a[j] > x) { a[i] = a[j]; i = j; j = 2 * i + 1; }
        else go = false;
    }
    a[i] = x;
}

void buildHeap(int* a, int n) {
    if (n < 2) return;
    for (int i = n / 2 - 1; i >= 0; i--) siftDown(a, n, i);
}

void heapSort(int* a, int n) {
    if (n < 2) return;
    buildHeap(a, n);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        siftDown(a, i, 0);
    }
}

void generateArray(int* a, int n, int lo, int hi, unsigned seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> d(lo, hi);
    for (int i = 0; i < n; i++) a[i] = d(rng);
}

void saveText(const string& path, const int* a, int n) {
    ofstream f(path);
    if (!f) return;
    for (int i = 0; i < n; i++) f << a[i] << "\n";
}

void saveBinary(const string& path, const int* a, int n) {
    ofstream f(path, ios::binary);
    if (!f) return;
    f.write((char*)a, n * sizeof(int));
}

bool loadBinary(const string& path, int* a, int n) {
    ifstream f(path, ios::binary);
    if (!f) return false;
    f.read((char*)a, n * sizeof(int));
    return (bool)f;
}

vector<int> readAnyFile(const string& path) {
    if (ifstream fin{ path }) {
        vector<int> v;
        int x;
        while (fin >> x) v.push_back(x);
        if (!v.empty()) return v;
    }

    if (ifstream fin{ path, ios::binary | ios::ate }) {
        streamsize sz = fin.tellg();
        if (sz > 0 && sz % (streamsize)sizeof(int) == 0) {
            int n = (int)(sz / sizeof(int));
            vector<int> v(n);
            fin.seekg(0);
            fin.read((char*)v.data(), sz);
            if (fin) return v;
        }
    }
    return {};
}

int main() {
    namespace fs = std::filesystem;
    const string outDir = "results";
    fs::create_directories(outDir);

    int runs = 3;

    ofstream csv(outDir + "/results.csv");
    if (!csv) { cerr << "Cannot open " << outDir << "/results.csv\n"; return 1; }
    csv << "case;file;n;range;source;algorithm;avg_sec\n";

    cout << "Predefined files (" << KNOWN_FILES_COUNT << "):\n";
    for (int i = 0; i < KNOWN_FILES_COUNT; i++) {
        cout << "  [" << i << "] exists=" << fs::exists(KNOWN_FILES[i])
            << "  " << KNOWN_FILES[i] << "\n";
    }

    int mode;
    cout << "\nChoose data source:\n"
        << "  1 = read " << KNOWN_FILES_COUNT << " predefined files\n"
        << "  2 = enter n, lo, hi from keyboard (3 cases)\n"
        << "  0 = reuse LAST generated array (3 cases)\n"
        << "  > ";
    cin >> mode;

    int casesCount;
    if (mode == 1)      casesCount = KNOWN_FILES_COUNT;  
    else if (mode == 2 || mode == 0) casesCount = 3;      
    else { cout << "Unknown mode, exit.\n"; return 1; }

    cout << "Will process " << casesCount << " case(s).\n";

    for (int ci = 0; ci < casesCount; ci++) {
        cout << "\n=== Case " << (ci + 1) << " of " << casesCount << " ===\n";

        int n = 0, lo = 0, hi = 0;
        int* original = nullptr;
        string source;
        string fileTag = "-";
        bool loaded = false;

        if (mode == 1) {
            string fname = KNOWN_FILES[ci];
            fileTag = fname;
            cout << "  Using predefined file: " << fname << "\n";
            vector<int> v = readAnyFile(fname);
            if (!v.empty()) {
                n = (int)v.size();
                lo = *min_element(v.begin(), v.end());
                hi = *max_element(v.begin(), v.end());
                original = new int[n];
                copy(v.begin(), v.end(), original);
                source = "file:" + fname;
                loaded = true;
            }
            else {
                cout << "  Cannot read '" << fname << "', skip.\n";
                continue;
            }
        }
        
        else if (mode == 2) {
            cout << "Enter number of elements n (n > 0): ";
            cin >> n;
            if (n <= 0) { cout << "Invalid n, skip.\n"; continue; }
            cout << "Enter range: lo hi (lo <= hi): ";
            cin >> lo >> hi;
            if (lo > hi) { cout << "Invalid range, skip.\n"; continue; }
            original = new int[n];
        }
        
        else if (mode == 0) {
            ifstream mf(outDir + "/last_meta.txt");
            if (!mf) {
                cout << "  No previous array (" << outDir << "/last_meta.txt not found), skip.\n";
                continue;
            }
            mf >> n >> lo >> hi;
            if (n <= 0) { cout << "  Bad last_meta.txt, skip.\n"; continue; }

            original = new int[n];
            if (!loadBinary(outDir + "/last_array.bin", original, n)) {
                cout << "  Cannot load " << outDir << "/last_array.bin, skip.\n";
                delete[] original;
                continue;
            }
            source = "last:" + outDir + "/last_array.bin";
            loaded = true;
            cout << "  Loaded previous array: n = " << n
                << ", range [" << lo << ".." << hi << "]\n";
        }

        if (!loaded) {
            generateArray(original, n, lo, hi, 42u + (unsigned)ci);
            source = "generated";

            saveBinary(outDir + "/array_n" + to_string(n) + "_" +
                to_string(lo) + "_" + to_string(hi) + ".bin", original, n);
            saveText(outDir + "/original_n" + to_string(n) +
                "_range_" + to_string(lo) + "_" + to_string(hi) + ".txt",
                original, n);

            saveBinary(outDir + "/last_array.bin", original, n);
            {
                ofstream mf(outDir + "/last_meta.txt");
                mf << n << " " << lo << " " << hi << "\n";
            }
        }

        cout << "  n = " << n << ", range [" << lo << ".." << hi
            << "], source = " << source << "\n";

        double totalTime = 0;
        for (int r = 0; r < runs; r++) {
            int* arr = new int[n];
            for (int i = 0; i < n; i++) arr[i] = original[i];

            auto t1 = high_resolution_clock::now();
            heapSort(arr, n);
            auto t2 = high_resolution_clock::now();

            totalTime += duration<double>(t2 - t1).count();

            if (!isSorted(arr, n))
                cout << "    ERROR: array not sorted after HeapSort!\n";
            delete[] arr;
        }

        double avg = totalTime / runs;
        cout << "    [" << source << "] HeapSort: "
            << fixed << setprecision(9) << avg << " sec\n";

        csv << (ci + 1) << ";" << fileTag << ";" << n << ";" << lo << ".." << hi
            << ";" << source << ";HeapSort;"
            << fixed << setprecision(9) << avg << "\n";

        delete[] original;
    }

    csv.close();
    cout << "\nResults saved to " << outDir << "/results.csv\n";
    return 0;
}
