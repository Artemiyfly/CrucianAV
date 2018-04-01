#include <iostream>
#include <cstdio>
#include <windows.h>
using namespace std;

struct node {
    int go[4] = {-1, -1, -1, -1};
    int link = -1, terminate = -1, p;
    char last;
    node() {}
    void setBuf(int buf[]) {
        for (int i = 0; i < 4; i++)
            buf[i] = go[i];
        buf[4] = terminate;
    }
};

#define BUF_SZ 3900

struct reader {
private:
    FILE *cFile = NULL;
    char buf[BUF_SZ];
    int ptr = 1e9;
public:
    int64_t n;
    reader(){}
    void init(const string &path, int64_t n_) {
        if(cFile)
            fclose(cFile);
        cFile = fopen(path.c_str(), "rb");
        n = n_;
        ptr = 1e9;
    }
    ~reader(){
        if(cFile)
            fclose(cFile);
    }
    char get_int2() {
        if (ptr + 2 >= (BUF_SZ << 3)) {
            fread(buf, 1, BUF_SZ, cFile);
            ptr = 0;
        }
        ptr += 2;
        //cout << "#" << ((ptr - 2) >> 3) << ' ';
        //outChar(buf[(ptr - 2) >> 3]);
        //return 0;
        return (buf[(ptr - 2) >> 3] >> (6 - ((ptr - 2)&7))) & 3;
    }

};


node nodes[6000000];
int nodesCnt = 1;
int link(int from);

int go(int from, char to) {
    if (nodes[from].go[to] == -1)
        nodes[from].go[to] = go(link(nodes[from].p), to);
    return nodes[from].go[to];
}

int link(int from) {
    if (nodes[from].p == 0)
        return 0;
    if (nodes[from].link == -1)
        nodes[from].link = go(link(nodes[from].p), nodes[from].last);
    return nodes[from].link;
}

int StrToInt(const string &in) {
    int res = 0;
    for (int i = 0; i < in.size(); i++)
        res = res * 10 + (in[i] - '0');
    return res;
}

int main(int argc, char* argv[]) {
    reader a;
    WIN32_FIND_DATA f;
    int cur, curChar;
    string curDir = string(argv[0]).substr(0, string(argv[0]).size() - 10);
    HANDLE h = FindFirstFile((curDir + "viruses\\*").c_str(), &f);
    do {
        if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
            continue;
        a.init(curDir + "viruses\\" + (string) f.cFileName, (1ll<<32)*f.nFileSizeHigh + f.nFileSizeLow);
        cur = 0;
        for (int i = 0; i < a.n * 4; i++){
            curChar = a.get_int2();
            //cout << "red " << curChar;
            if (nodes[cur].go[curChar] == -1){
            //    cout << " NN" << nodesCnt - 1 << ' ';
                nodes[nodesCnt].p = cur;
                nodes[nodesCnt].last = curChar;
                nodes[cur].go[curChar] = nodesCnt++;
            }
            cur = nodes[cur].go[curChar];
            //cout << "\twent to " << cur << endl;
        }
        nodes[cur].terminate = StrToInt(f.cFileName);
    } while(FindNextFile(h, &f) != 0);
    for (int i = 0; i < 4; i++)
        if (nodes[0].go[i] == -1)
            nodes[0].go[i] = 0;
    nodes[0].p = 0;
    int buf[5];
    buf[0] = nodesCnt;
    FILE *stamms = fopen((curDir + "stamms").c_str(), "wb");
    fwrite(buf, 4, 1, stamms);

    for(int i = 0; i < nodesCnt; i++) {
        for (int j = 0; j < 4; j++)
            buf[j] = go(i, j);
        buf[4] = nodes[i].terminate;
        fwrite(buf, 4, 5, stamms);
    }
    fclose(stamms);

    cout << nodesCnt << " nodes:\n";

    if (nodesCnt > 100)
        return 0;
    for (int i = 0; i < nodesCnt; i++) {
        cout << i << ": ";
        for (int j = 0; j < 4; j++)
            cout << nodes[i].go[j] << ' ';
        cout << nodes[i].terminate;
        cout << endl;
    }
    return 0;
}
