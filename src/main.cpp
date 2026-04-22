#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

// Bucketed append-only log across <=16 files to meet file count limit.
// Each bucket file stores lines: "I index value" or "D index value".
// On find(index): scan its bucket file, apply ops affecting that index, dedup, sort, print.
// No global in-memory state; only per-operation memory.

static const string DATA_DIR = "data";
static const int BUCKETS = 16; // <= 20 file limit

static bool ensure_data_dir() {
    struct stat st{};
    if(::stat(DATA_DIR.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return ::mkdir(DATA_DIR.c_str(), 0777) == 0;
}

static inline unsigned bucket_of(const string &idx){
    // Fowler–Noll–Vo hash variant for stability
    uint64_t h = 1469598103934665603ull;
    for(unsigned char c: idx){ h ^= c; h *= 1099511628211ull; }
    return (unsigned)(h & (BUCKETS-1));
}

static string bucket_path(unsigned b){
    ostringstream oss; oss << DATA_DIR << "/bucket_" << setw(2) << setfill('0') << b << ".log";
    return oss.str();
}

static void append_op(const string &idx, int v, char op){
    unsigned b = bucket_of(idx);
    string path = bucket_path(b);
    ofstream fout(path, ios::app);
    fout << op << ' ' << idx << ' ' << v << '\n';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ensure_data_dir();

    int n; if(!(cin>>n)) return 0;
    string cmd, idx; int v;
    for(int i=0;i<n;++i){
        cin >> cmd;
        if(cmd=="insert"){
            cin >> idx >> v;
            append_op(idx, v, 'I');
        } else if(cmd=="delete"){
            cin >> idx >> v;
            append_op(idx, v, 'D');
        } else if(cmd=="find"){
            cin >> idx;
            unsigned b = bucket_of(idx);
            string path = bucket_path(b);
            ifstream fin(path);
            if(!fin.good()){ cout << "null\n"; continue; }
            // Using unordered_map<bool> equivalently via unordered_set for present values
            unordered_set<int> present; present.reserve(256);
            char op; string key; int val;
            while(fin >> op >> key >> val){
                if(key==idx){
                    if(op=='I') present.insert(val);
                    else if(op=='D') present.erase(val);
                }
            }
            if(present.empty()){ cout << "null\n"; }
            else {
                vector<int> out; out.reserve(present.size());
                for(int x: present) out.push_back(x);
                sort(out.begin(), out.end());
                for(size_t j=0;j<out.size();++j){ if(j) cout << ' '; cout << out[j]; }
                cout << '\n';
            }
        } else {
            string rest; getline(cin, rest);
        }
    }
    return 0;
}
