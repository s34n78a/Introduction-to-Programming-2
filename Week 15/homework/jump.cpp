#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main(){
    int n;
    if(!(cin >> n)) return 0;

    auto sievePrimes = [](int N){
        vector<int> primes;
        if(N < 2) return primes;
        vector<bool> isPrime(N+1, true);
        isPrime[0] = isPrime[1] = false;
        for(int i = 2; i <= N; ++i){
            if(isPrime[i]){
                primes.push_back(i);
                if ((long long)i * i <= N) {
                    for(long long j = (long long)i*i; j <= N; j += i) isPrime[j] = false;
                }
            }
        }
        return primes;
    };

    auto getDivisorsSingle = [](int x, const vector<int>& primes){
        vector<int> div;
        div.push_back(1);
        int temp = x;
        for(int p : primes){
            if((long long)p * p > temp) break;
            if(temp % p == 0){
                int sz = div.size();
                int pw = 1;
                while(temp % p == 0){
                    pw *= p;
                    temp /= p;
                }
                for(int i = 0; i < sz; ++i){
                    int d = div[i];
                    for(int m = p; m <= pw; m *= p) div.push_back(d * m);
                }
            }
        }
        if(temp > 1){
            int sz = div.size();
            for(int i = 0; i < sz; ++i) div.push_back(div[i] * temp);
        }
        return div;
    };

    const int INF = 1e9;
    vector<int> dist(n+1, INF);
    queue<int> q;
    dist[n] = 0;
    q.push(n);

    vector<int> primes = sievePrimes(n);
    
    while(!q.empty()){
        int v = q.front(); q.pop();
        vector<int> currentDivs = getDivisorsSingle(v, primes);

        for(int t : currentDivs){
            if(t == v) continue;
            int u1 = v - t;
            if(u1 >= 1 && dist[u1] == INF){
                dist[u1] = dist[v] + 1;
                q.push(u1);
            }
            int u2 = v + t;
            if(u2 <= n && dist[u2] == INF){
                dist[u2] = dist[v] + 1;
                q.push(u2);
            }
        }
    }

    for(int i = 1; i <= n; ++i) cout << dist[i] << ' ';
    return 0;
}