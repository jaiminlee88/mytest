namespace std {

    class string
    {
        unsigned len;
        char* pbuf;
        
    public:
        explicit string(const char* str);
        string(const string&);
        ~string();
        string& operator=(const string&);
        string& operator=(const char* s);
        const char& operator[](unsigned idx) const;
        char& operator[](unsigned idx);
        const char* c_str() const;
        unsigned length() const;
        unsigned size() const;
    };

    string::string(const char* str) :
        len (0), pbuf(0)
    {
        *this = str;
    }

    string::string(const string& s) : 
        len(0), pbuf(0)
    {
        *this = s;
    }
    string::~string()
    {
        if(pbuf != 0) {
            delete[] pbuf;
            pbuf = 0;
        }
    }

    string& string::operator=(const string& s)
    {
        if (&s == this)
            return *this;
        this->~string();
        len = s.len;
        pbuf = strcpy(new char[len + 1], s.pbuf);
        return *this;
    }


    string& string::operator=(const char* s)
    {
        this->~string();
        len = strlen(s);
        pbuf = strcpy(new char[len + 1], s);
        return *this;
           }

    const char& string::operator[](unsigned idx) const
    {
        return pbuf[idx];
    }
    char& string::operator[](unsigned idx)
    {
        return pbuf[idx];
    }
    const char* string::c_str() const
    {
        return pbuf;
    }
    unsigned string::length() const
    {
        return len;
    }
    unsigned string::size() const
    {
        return len;
    }
    ofstream& operator<<(ofstream& o, const string& s)
    {
        return o << s.c_str();
    }
}
清单13-14 iostream

// iostream
#include "minicrt.h"

namespace std {

class ofstream
{
    protected:
        FILE* fp;
        ofstream(const ofstream&);
    public:
        enum openmode{in = 1, out = 2, binary = 4, trunc = 8};

        ofstream();
        explicit ofstream(const char *filename, ofstream::openmode md = ofstream::out);
        ~ofstream();
        ofstream& operator<<(char c);
        ofstream& operator<<(int n);
        ofstream& operator<<(const char* str);
        ofstream& operator<<(ofstream& (*)(ofstream&));
        void open(const char *filename, ofstream::openmode md = ofstream::out); 
        void close();
        ofstream& write(const char *buf, unsigned size);
};

inline ofstream& endl(ofstream& o)
{
    return o << '\n';
}

class stdout_stream : public ofstream {
public:
    stdout_stream();
};

extern stdout_stream cout;
}
清单13-15 iostream.cpp

// iostream.cpp
#include "minicrt.h"
#include "iostream"

#ifdef WIN32
#include <Windows.h>
#endif

namespace std {

stdout_stream::stdout_stream() : ofstream() 
{
        fp = stdout;
}

stdout_stream cout;

ofstream::ofstream() : fp(0)
{
}

ofstream::ofstream(const char *filename, ofstream::openmode md) : fp(0)
{
    open(filename, md);
    
}
ofstream::~ofstream()
{
    close();
}
ofstream& ofstream::operator<<(char c)
{
    fputc(c, fp);
    return *this;
}
ofstream& ofstream::operator<<(int n)
{
    fprintf(fp, "%d", n);
    return *this;
}
ofstream& ofstream::operator<<(const char* str)
{
    fprintf(fp, "%s", str);
    return *this;
}

ofstream& ofstream::operator<<(ofstream& (*manip)(ofstream&))
{
    return manip(*this);
}

void ofstream::open(const char *filename, ofstream::openmode md)
{
    char mode[4];
    close();
    switch (md)
    {
            case out | trunc:
        strcpy(mode, "w");
        break;
    case out | in | trunc:
        strcpy(mode, "w+");
    case out | trunc | binary:
        strcpy(mode, "wb");
        break;
    case out | in | trunc | binary:
        strcpy(mode, "wb+");
    }
    fp = fopen(filename, mode);
}
void ofstream::close()
{
    if (fp)
    {
        fclose(fp);
        fp = 0;
    }
}

ofstream& ofstream::write(const char *buf, unsigned size)
{
    fwrite(buf, 1, size, fp);
    return *this;
}

}