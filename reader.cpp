

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
        cout << "#" << ptr << ' ' << ((ptr - 2) >> 3) << ' ';
        //outChar(buf[(ptr - 2) >> 3]);
        //return 0;
        //cout << ((buf[(ptr - 2) >> 3] >> (6 - ((ptr - 2)&7))) & 3) << endl;
        return (buf[(ptr - 2) >> 3] >> (6 - ((ptr - 2)&7))) & 3;
    }
};

char hash_5bits(const string &a) {
    char res;
    for (int i = 0; i < a.size(); i++)
        res ^= a[i];
    return (res ^ (res >> 3)) & 31;
}

struct walker{
    char mode;
    int64_t size_by_hash[32];
    std::set <string> exec_formats;
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
    walker() {
        for (int i = 0; i < 32; i++)
            size_by_hash[i] = 0;
        exec_formats.insert({".exe"});
    }
    void onFound(const string &path, int64_t n) {
        if (mode&ATTR_EXEC_ONLY) {
            if (exec_formats.find(path.substr(path.size() - 4, 4)) == exec_formats.end())
                return;
        }
        currentSize += n;
        size_by_hash[hash_5bits(path)] += n;
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
        int64_t tread_task[8] = {0,0,0,0,0,0,0,0};
        unsigned int tread_mask[8] = {0,0,0,0,0,0,0,0};
        vector <int> sorted_taks(32);
        for (int i = 0; i < 32; i++)
            sorted_taks[i] = [i];
        sort(sorted_taks.begin(), sorted_taks.end(), [](int a, int b) {return size_by_hash[a] > size_by_hash[b];})
        for (int i = 0; i < 32; i++)
            cout << size_by_hash[sorted_taks[i]] << ' ';
    }
};

