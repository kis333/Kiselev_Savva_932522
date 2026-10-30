#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
using namespace std;
using namespace std::chrono;

bool isSorted(int* a, int n) {
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i - 1]) return false;
    }
    return true;
}

void shellSort(int* a, int n, int* gaps, int gapCount) {
    for (int g = 0; g < gapCount; g++) {
        int gap = gaps[g];
        if (gap >= n) continue;
        for (int i = gap; i < n; i++) {
            int key = a[i];
            int j = i - gap;
            while (j >= 0 && a[j] > key) {
                a[j + gap] = a[j];
                j = j - gap;
            }
            a[j + gap] = key;
        }
    }
}

int shellGaps(int n, int* gaps) {
    int count = 0;
    for (int g = n / 2; g > 0; g = g / 2) {
        gaps[count++] = g;
    }
    return count;
}

int knuthGaps(int n, int* gaps) {
    int count = 0;
    int h = 1;
    while (h < n) {
        gaps[count++] = h;
        h = 3 * h + 1;
    }
    for (int i = 0; i < count / 2; i++) {
        int t = gaps[i];
        gaps[i] = gaps[count - 1 - i];
        gaps[count - 1 - i] = t;
    }
    return count;
}

int hibbardGaps(int n, int* gaps) {
    int count = 0;
    int h = 1;
    while (h < n) {
        gaps[count++] = h;
        h = 2 * h + 1;
    }
    for (int i = 0; i < count / 2; i++) {
        int t = gaps[i];
        gaps[i] = gaps[count - 1 - i];
        gaps[count - 1 - i] = t;
    }
    return count;
}

void generateArray(int* a, int n, int lo, int hi, unsigned seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> d(lo, hi);
    for (int i = 0; i < n; i++) {
        a[i] = d(rng);
    }
}

void saveArrayToFile(const string& filename, const int* a, int n) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Cannot open file for writing: " << filename << "\n";
        return;
    }
    for (int i = 0; i < n; i++) {
        fout << a[i] << "\n";
    }
}

int main() {
    namespace fs = std::filesystem;
    const string outDir = "results";
    fs::create_directories(outDir);

    int sizes[3] = { 10000, 100000, 1000000 };
    int ranges[3][2] = { {-10, 10}, {-1000, 1000}, {-100000, 100000} };
    int runs = 3;

    ofstream csv(outDir + "/results.csv");
    if (!csv) {
        cerr << "Cannot open " << outDir << "/results.csv\n";
        return 1;
    }
    csv << "size;range;algorithm;avg_sec\n";

    for (int si = 0; si < 3; si++) {
        int n = sizes[si];
        cout << "\n=== size = " << n << " ===\n";

        for (int ri = 0; ri < 3; ri++) {
            int lo = ranges[ri][0];
            int hi = ranges[ri][1];

            int* original = new int[n];
            generateArray(original, n, lo, hi, 42 + si * 10 + ri);

            cout << "  range [" << lo << ".." << hi << "]\n";

            {
                ostringstream name;
                name << outDir << "/original_n" << n
                    << "_range_" << lo << "_" << hi << ".txt";
                saveArrayToFile(name.str(), original, n);
            }

            for (int alg = 0; alg < 3; alg++) {
                string algName;
                if (alg == 0) algName = "Shell(n/2)";
                else if (alg == 1) algName = "Knuth";
                else algName = "Hibbard";

                double totalTime = 0;

                for (int r = 0; r < runs; r++) {
                    int* arr = new int[n];
                    for (int i = 0; i < n; i++) arr[i] = original[i];

                    int* g = new int[100];
                    int gc;
                    if (alg == 0) gc = shellGaps(n, g);
                    else if (alg == 1) gc = knuthGaps(n, g);
                    else gc = hibbardGaps(n, g);

                    auto t1 = high_resolution_clock::now();
                    shellSort(arr, n, g, gc);
                    auto t2 = high_resolution_clock::now();

                    double t = duration<double>(t2 - t1).count();
                    totalTime += t;

                    if (!isSorted(arr, n)) {
                        cout << "    ERROR: array not sorted after " << algName << "!\n";
                    }

                    delete[] arr;
                    delete[] g;
                }

                double avg = totalTime / runs;
                cout << "    " << algName << ": " << avg << " sec\n";
                cout << n << ";" << lo << ".." << hi << ";" << algName << ";" << avg << "\n";

                csv << n << ";" << lo << ".." << hi << ";" << algName << ";"
                    << fixed << setprecision(6) << avg << "\n";
            }

            delete[] original;
        }
    }

    csv.close();
    cout << "\nResults saved to " << outDir << "/results.csv\n";
    return 0;
}
