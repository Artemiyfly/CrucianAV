#include <iostream>
#include <windows.h>
#include <cstdio>
#include <vector>
#include <set>

using namespace std;

#define BUF_SZ 13900
#define ATTR_SIZE_ONLY 1
#define ATTR_NEW_ONLY 2
#define ATTR_EXEC_ONLY 4

vector <pair<string, int> > treats;//path, id

void outChar(char in)
{
    for(int i = 7; i >= 0; i--)
        cout << int((in & (1 << i)) != 0);
    cout << endl;
}

struct reader {
private:
    FILE *cFile = NULL;
    char buf[BUF_SZ];
    int ptr = 1e9;
public:
    int64_t n;
    reader(){}
    bool init(const string &path, int64_t n_) {
        if(cFile)
            fclose(cFile);
        cFile = fopen(path.c_str(), "rb");
        if (!cFile)
            return 0;
        n = n_;
        ptr = 1e9;
        return 1;
    }
    ~reader(){
        if(cFile)
            fclose(cFile);
    }
    char get_int2(){
        if (ptr + 2 > (BUF_SZ << 3)){
            fread(buf, 1, BUF_SZ, cFile);
            ptr = 0;
        }
        ptr += 2;
        //cout << "#" << ((ptr - 2) >> 3) << ' ';
        //outChar(buf[(ptr - 2) >> 3]);
        //return 0;
        //cout << ((buf[(ptr - 2) >> 3] >> (6 - ((ptr - 2)&7))) & 3) << endl;
        return (buf[(ptr - 2) >> 3] >> (6 - ((ptr - 2)&7))) & 3;
    }
};

string inttostr(int in) {
    string res;
    while(in > 0) {
        res = char(in % 10 + '0') + res;
        in /= 10;
    }
    return res;
}


char a[2000][500], file[10100];
int16_t z[10600];

char A(int i, int cur_v) {
    if (i >= 500)
        return file[i - 500];
    else
        return a[cur_v][i];
}

int main() {
    int z_block_start = -1, z_block_len = -1;
    for (int i = 0; i < 2000; i++) {
        FILE*f = fopen(("viruses\\" + inttostr(i + 1)).c_str(), "rb");
        fread(a[i], 1, 500, f);
        fclose(f);
    }
    for (int k = 0; k < 200; k++) {
        FILE*f = fopen((".\\ForScan\\" + inttostr(k + 1)).c_str(), "rb");
        fread(file, 1, 10000, f);
        fclose(f);
        for (int cur_v = 0; cur_v < 2000; cur_v++) {
            z_block_len = z_block_start = -1;
            z[0] = 10500;
            for (int i = 1; i < 10500; i++) {
                z[i] = 0;
                if (z_block_start + z_block_len - 1 >= i) {
                    z[i] = min((int)z[i - z_block_start], z_block_start + z_block_len - 1 - i);
                     //cout << z[i] << " as " << i - z_block_start << " %\n";
                }
                while (i + z[i] < 10500 && A(i + z[i], cur_v) == A(z[i], cur_v)) {
                    //cout << "A("<<i + z[i] << ") = A(" << z[i] << ") as " << (int)A(i + z[i], cur_v) << " = " << (int)A(z[i], cur_v) << endl;
                    z[i]++;
                    //cout << i << "+"<<z[i] << endl;
                }
                //if (z[i] >= 500 && i >= 500)
                //    cout << "vir " << cur_v << " in " << k << ':' << i - 500 << " is " << z[i] << endl;
                if (i + z[i] > z_block_start + z_block_len) {
                    z_block_start = i;
                    z_block_len = z[i];
                }
            }
        }
    }
    return 0;
}
