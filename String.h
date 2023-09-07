#pragma once
#include <cstdint>
#include <string>

class String{
public:
    enum CodePage{
        ANSI,UTF8,GBK,SHIFT_JIS
    };
    String(const wchar_t* charsquence,uint32_t len=0);
    String(const char* charsquence,CodePage cp=UTF8,uint32_t len=0);
    String(const String&);
    ~String();
    String operator+(const String& other)const;
    String operator+(const wchar_t* other)const;
    void operator+=(const wchar_t* other);
    void operator+=(const String& other);
    uint32_t GetLength()const{return mLength;}
    const std::string Encode(CodePage cp)const;
    int IndexOf(const wchar_t ch,int startIndex=0) const;
    int IndexOf(const wchar_t* str,int startIndex=0) const;
    int LastIndexOf(const wchar_t* str,int startIndex) const;
    int LastIndexOf(const wchar_t* str) const;
    int LastIndexOf(const wchar_t ch,int startIndex) const;
    uint32_t Count(const wchar_t* str)const;
    String& Insert(const wchar_t* str,uint32_t length=0);
    String& Insert(const String& str);
    String& Replace(const wchar_t* search,const wchar_t* replacement,int startIndex);
    String& ReplaceAll(const wchar_t* search,const wchar_t* replacement);
    String& Remove(uint32_t length,uint32_t start=0);
    String& SubString(uint32_t start);
    String& SubString(uint32_t start,uint32_t length);
protected:
    String();
    wchar_t* mCharsquence;
    uint32_t mLength;
};