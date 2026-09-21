#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
using namespace std;

int main() {

    int n, lo, hi, StartCity;

    cout << "\nenter the number of cities: ";
    cin >> n;
    cout << "\nmin coast: ";
    cin >> lo;
    cout << "\nmax coast: ";
    cin >> hi;
    cout << "\nStartCity (1.." << n << "): ";
    cin >> StartCity;

    StartCity = StartCity - 1;

    int** cost = new int* [n];
    for (int i = 0; i < n; i++) {
        cost[i] = new int[n];
    }

    srand(time(0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) cost[i][j] = INT_MAX;
            else cost[i][j] = lo + rand() % (hi - lo + 1);
        }
    }

    cout << "\nMatrix: \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (cost[i][j] == INT_MAX) cout << "   --";
            else cout << "  " << cost[i][j];
        }
        cout << "\n";
    }

    int* perm = new int[n];
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (i != StartCity) {
            perm[k] = i;
            k = k + 1;
        }
    }

    int best = 0;
    int worst = 0;
    int* bestWay = new int[n + 1];
    int* worstWay = new int[n + 1];
    int found = 0;

    clock_t t1 = clock();

    int done = 0;

    while (done == 0) {
        int total = 0;
        int cur = StartCity;
        int ok = 1;

        for (int i = 0; i < k; i++) {
            if (cost[cur][perm[i]] == INT_MAX) { ok = 0; break; }
            total = total + cost[cur][perm[i]];
            cur = perm[i];
        }

        if (ok == 1 && cost[cur][StartCity] != INT_MAX) {
            total = total + cost[cur][StartCity];

            if (found == 0 || total < best) {
                best = total;
                bestWay[0] = StartCity;

                for (int i = 0; i < k; i++) bestWay[i + 1] = perm[i];
                bestWay[k + 1] = StartCity;
            }

            if (found == 0 || total > worst) {
                worst = total;
                worstWay[0] = StartCity;

                for (int i = 0; i < k; i++) worstWay[i + 1] = perm[i];
                worstWay[k + 1] = StartCity;
            }

            found = 1;
        }

        int i = k - 2;
        while (i >= 0 && perm[i] >= perm[i + 1]) i = i - 1;

        if (i < 0) {
            done = 1;
        }

        else {
            int j = k - 1;

            while (perm[j] <= perm[i]) j = j - 1;
            int t = perm[i];
            perm[i] = perm[j];
            perm[j] = t;
            int left = i + 1;
            int right = k - 1;

            while (left < right) {
                t = perm[left];
                perm[left] = perm[right];
                perm[right] = t;
                left = left + 1;
                right = right - 1;
            }
        }
    }

    clock_t t2 = clock();
    double tExact = (double)(t2 - t1) / CLOCKS_PER_SEC;

    cout << "\nThe exact solution:\n";

    if (found == 0) {
        cout << "Tours is not finded\n";

        for (int i = 0; i < n; i++) delete[] cost[i];
        delete[] cost;
        delete[] perm;
        delete[] bestWay;
        delete[] worstWay;
        return 0;
    }

    cout << "Best way:  ";
    for (int i = 0; i <= k + 1; i++) {
        cout << bestWay[i] + 1;
        if (i < k + 1) cout << " -> ";
    }
    cout << "  = " << best << "\n";

    cout << "Worst Way:  ";
    for (int i = 0; i <= k + 1; i++) {
        cout << worstWay[i] + 1;
        if (i < k + 1) cout << " -> ";
    }
    cout << "  = " << worst << "\n";

    cout << "Time: " << tExact << " s\n";

    int hFound = 0;
    int hBest = 0;

    int* hPath = new int[n + 1];
    int hLen = 0;

    clock_t th1 = clock();

    for (int s = 0; s < n; s++) {

        
        int* vis = new int[n]();
       
        int* p = new int[n + 1];
        int len = 0;
        p[len] = s;
        len = len + 1;
        vis[s] = 1;

        int cur = s;
        int total = 0;
        int ok = 1;

        for (int step = 0; step < n - 1; step++) {
            int nxt = -1;
            int bd = INT_MAX;

            for (int v = 0; v < n; v++) {
                if (vis[v] == 0 && cost[cur][v] < bd) {
                    bd = cost[cur][v];
                    nxt = v;
                }
            }

            if (nxt == -1) { ok = 0; break; }

            total = total + bd;
            p[len] = nxt;
            len = len + 1;
            vis[nxt] = 1;
            cur = nxt;
        }

        if (ok == 0) {
            delete[] vis;
            delete[] p;
            continue;
        }

        total = total + cost[cur][s];
        p[len] = s;
        len = len + 1;

        if (hFound == 0 || total < hBest) {
            hFound = 1;
            hBest = total;
            for (int i = 0; i < len; i++) hPath[i] = p[i];
            hLen = len;
        }

        delete[] vis;
        delete[] p;
    }

    clock_t th2 = clock();
    double tHeur = (double)(th2 - th1) / CLOCKS_PER_SEC;

    cout << "\nHeuristics (The nearest neighbor):\n";

    if (hFound == 0) {
        cout << "Heuristic didn't find a tour.\n";

        for (int i = 0; i < n; i++) delete[] cost[i];
        delete[] cost;
        delete[] perm;
        delete[] bestWay;
        delete[] worstWay;
        delete[] hPath;
        return 0;
    }

    cout << "tour:     ";
    for (int i = 0; i < hLen; i++) {
        cout << hPath[i] + 1;
        if (i + 1 < hLen) cout << " -> ";
    }
    cout << "  = " << hBest << "\n";
    cout << "time: " << tHeur << " s\n";

    int q;
    if (worst == best) q = 100;
    else q = 100 * (worst - hBest) / (worst - best);

    cout << "quality: " << q << " %\n";

    for (int i = 0; i < n; i++) delete[] cost[i];
    delete[] cost;
    delete[] perm;
    delete[] bestWay;
    delete[] worstWay;
    delete[] hPath;

    return 0;
}