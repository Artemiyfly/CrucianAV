#include <iostream>
#include <windows.h>
#include <cstdio>
#include <vector>
#include <set>

using namespace std;

#define BUF_SZ 3900
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

struct node {
    int go[4], terminal;
    node(){};
};

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
        if (ptr + 2 >= (BUF_SZ << 3)){
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

node nodes[2000000];
reader Reader;
int64_t partSize, currentSize, lastOut;
struct walker{
    char mode;
    string blacklist;
    set <string> exec_formats;
    void inspect(const string &path) {
        if (path == blacklist)
            return;
        WIN32_FIND_DATA f;
        HANDLE h = FindFirstFile((path + "*").c_str(), &f);
        do{
            if (strcmp(f.cFileName, ".") == 0 || strcmp(f.cFileName, "..") == 0)
                continue;
            if (f.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
                inspect(path + f.cFileName +"/");
            } else {
                onFound(path + f.cFileName, (1ll<<32)*f.nFileSizeHigh + f.nFileSizeLow);
            }
        }while(FindNextFile(h, &f) != 0);
    }
    walker(const string &myAdress) {
        blacklist = myAdress.substr(0, myAdress.size() - 7) + "viruses/";
        exec_formats.insert({".exe"});
    }
    void onFound(const string &path, int64_t n) {
        if (mode&ATTR_EXEC_ONLY) {
            if (exec_formats.find(path.substr(path.size() - 4, 4)) == exec_formats.end())
                return;
        }
        currentSize += n;
        if (mode&ATTR_SIZE_ONLY)
            return;
        //cout << "N: " << n << endl;
        //cout << '"'<< path <<'"' << n << endl;
        if (currentSize - lastOut >= partSize) {
            lastOut = currentSize;
            cout << currentSize/partSize << endl;
        }
        if(!Reader.init(path, n))
            return;
        int cur = 0;
        for (int i = 0; i < n * 4; i++) {
            cur = nodes[cur].go[Reader.get_int2()];
            if (nodes[cur].terminal != -1)
                treats.push_back(make_pair(path, nodes[cur].terminal));
        }
    }
    void scan(const string &path, int mode_) {
        mode = mode_ | 1;
        inspect(path);
        partSize = max(currentSize / 200, 1ll);
        currentSize = lastOut = 0;
        mode = mode_;
        cout << 0 << endl;
        inspect(path);
        cout << -(int)treats.size() << endl;
        for (int i = 0; i < treats.size(); i++)
            cout << treats[i].first << '|' << treats[i].second << endl;
    }
};


int main(int argc, char * argv[]) {
    reader a;
    int n, buf[5];
    FILE *stamms = fopen("stamms", "rb");
    fread(buf, 4, 1, stamms);
    n = buf[0];
    for (int i = 0; i < n; i++) {
        fread(buf, 4, 5, stamms);
        for (int j = 0; j < 4; j++)
            nodes[i].go[j] = buf[j];
        nodes[i].terminal = buf[4];
    }

    /*cout << n << " nodes:\n";
    for (int i = 0; i < n; i++) {
        cout << i << ": ";
        for (int j = 0; j < 4; j++)
            cout << nodes[i].go[j] << ' ';
        cout << endl;
    }*/
    walker b(argv[0]);
    if (argc < 3) {
        b.scan("C:\\Users\\Artemiy\\Desktop\\", 0);
        cout << "NEA";
        return 0;
    }
    b.scan(argv[2], argv[1][0] - '0');
    return 0;
}
