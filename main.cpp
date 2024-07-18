#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

const int N = 2e5 + 5;
const int LOG = 20;
const ll INF = 2e18;

int sparse[N][LOG];
int binup[N][LOG];
ll sum[N][LOG];

int get_max(int l, int r) {
    int lg = __lg(r - l + 1);
    
    return max(sparse[l][lg], sparse[r - (1 << lg) + 1][lg]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    cin.tie(0);
    
    int n;
    cin >> n;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < LOG; j++) {
            binup[i][j] = -1;
        }
    }
    
    
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    
    vector<int> b(n);
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }
    
    for (int i = 0; i < n; i++) {
        sparse[i][0] = a[i];
    }
    
    for (int i = 1; i < LOG; i++) {
        for (int j = 0; j < n - (1 << (i - 1)); j++) {
            sparse[j][i] = max(sparse[j][i - 1], sparse[j + (1 << (i - 1))][i - 1]);
        }
    }
    
    vector<int> next_arr(n, -1);
    
    for (int i = 0; i < n; i++) {
        if (i == n - 1) continue;
        if (get_max(i + 1, n - 1) <= a[i]) continue;
        
        int l = 0;
        int r = n - 1 - i;
        
        while (r - l > 1) {
            int mid = (l + r) / 2;
            
            int res = get_max(i + 1, i + mid);
            
            if (res >  a[i]) {
                r = mid;
            } else {
                l = mid;
            }
        }
        
        next_arr[i] = i + r;
    }

    // for (int i = 0; i < n; i++) {
    //     cout << next_arr[i] << ' ';
    // }
    
    for (int i = 0; i < n; i++) {
        if (next_arr[i] == -1) continue;
        
        binup[i][0] = next_arr[i];
        sum[i][0] = b[next_arr[i]];
    }
    
    for (int i = 1; i < LOG; i++) {
        for (int j = 0; j < n; j++) {
            if (binup[j][i - 1] != -1) {
                binup[j][i] = binup[binup[j][i - 1]][i - 1];
            } else {
                binup[j][i] = -1;
            }

            if (binup[j][i - 1] != -1) {
                sum[j][i] = sum[j][i - 1] + sum[binup[j][i - 1]][i - 1];
            }
        }
    }

    // cout << binup[0][LOG - 1] << '\n';
    
    int q;
    cin >> q;
    
    while (q--) {
        ll s, m;
        cin >> s >> m;
        
        if (b[s - 1] > m) {
            cout << 0 << '\n';
            continue;
        }
        
        m -= b[s - 1];
        s--;
        
        int ans = 1;
        
        for (int i = LOG - 1; i >= 0; i--) {
            if (binup[s][i] != -1 && sum[s][i] <= m) {
                m -= sum[s][i];
                ans += (1 << (i));
                
                s = binup[s][i];
            }
        }
        
        cout << ans << '\n';
    }
}
