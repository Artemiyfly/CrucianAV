#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int n = 2000, ///Number of signatures
    nf = 200, /// Number of files
    m = 500, ///Size of signature in bytes
    mf = 10000; ///Size of file

string inttostr(int in) {
    string res;
    while(in > 0) {
        res = char(in % 10 + '0') + res;
        in /= 10;
    }
    return res;
}

void gen_sigs() {
    FILE *f;
    string fName;
    char buf[m];
    for (int i = 1; i <= n; i++) {
        fName = "viruses1/" + inttostr(i);
        //cout << fName << endl;
        f = fopen(fName.c_str(), "wb");
        for (int j = 0; j < m; j++)
            buf[j] = rand() & 255;
        fwrite(buf, 1, m, f);
        fclose(f);
    }
}

void gen_files() {
    FILE *f;
    string fName;
    char buf[max(m, mf)];
    /*cout << 1 << endl;
    for (int i = 10001; i <= nf + 10000; i++) {
        fName = "ForScan/" + inttostr(i);
        //cout << fName << endl;
        f = fopen(fName.c_str(), "wb");
        for (int j = 0; j < mf; j++)
            buf[j] = rand() & 255;
        fwrite(buf, 1, mf, f);
        fclose(f);
    }*/
    cout << 2 << endl;
    for (int i = 1; i <= nf; i++) {
        fName = "ForScan/" + inttostr(i);
        //cout << fName << endl;
        f = fopen(fName.c_str(), "ab");
        for (int j = 0; j < mf; j++)
            buf[j] = rand() & 255;
        fwrite(buf, 1, mf, f);
        fclose(f);
    }
}
int main() {
    srand(13123);
    gen_files();
    return 0;
}
