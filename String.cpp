#include "String.h"
#include <windows.h>
using namespace std;

uint32_t wstrlen(const wchar_t *sour){
    int count=0;
    while (*(sour++))
        count++;
    return count;
}
bool wstrncmp(const wchar_t *str1,const wchar_t* str2,uint32_t len){
    while(len--)
        if(str1[len]!=str2[len])return false;
    return true;
}
String String::operator+(const wchar_t* other)const{
    String str;
    uint32_t len1=mLength,len2=wstrlen(other);
    str.mLength=len1+len2;
    str.mCharsquence=new wchar_t[str.mLength];
    memcpy(str.mCharsquence,mCharsquence,len1*sizeof(wchar_t));
    memcpy(str.mCharsquence+len1,other,len2*sizeof(wchar_t));
    return str;
}
String String::operator+(const String& other)const{
    String str;
    uint32_t len1=mLength,len2=other.mLength;
    str.mLength=len1+len2;
    str.mCharsquence=new wchar_t[str.mLength];
    memcpy(str.mCharsquence,mCharsquence,len1*sizeof(wchar_t));
    memcpy(str.mCharsquence+len1,other.mCharsquence,len2*sizeof(wchar_t));
    return str;
}
void String::operator+=(const wchar_t* other){
    uint32_t len2=wstrlen(other),len1=mLength;
    wchar_t* buffer=new wchar_t[len1+len2];
    memcpy(buffer,mCharsquence,len1*sizeof(wchar_t));
    memcpy(buffer+len1,other,len2*sizeof(wchar_t));
    delete[] mCharsquence;
    mCharsquence=buffer;
    mLength=len1+len2;
    return;
}
void String::operator+=(const String& other){
    uint32_t len2=other.mLength,len1=mLength;
    wchar_t* buffer=new wchar_t[len1+len2];
    memcpy(buffer,mCharsquence,len1*sizeof(wchar_t));
    memcpy(buffer+len1,other.mCharsquence,len2*sizeof(wchar_t));
    delete[] mCharsquence;
    mCharsquence=buffer;
    mLength=len1+len2;
    return;
}
const string String::Encode(CodePage cp)const{
    UINT codepage;
    switch (cp)
    {
    case CodePage::UTF8:
        codepage=CP_UTF8;
        break;
    case CodePage::GBK:
        codepage=936;
        break;
    case CodePage::SHIFT_JIS:
        codepage=932;
        break;
    case CodePage::ANSI:
        codepage=CP_ACP;
    default:
        break;
    }
    uint32_t len= WideCharToMultiByte(codepage,0,mCharsquence,mLength,NULL,0,NULL,NULL);
    char* buffer=new char[len+1];
    buffer[WideCharToMultiByte(GetACP(),0,mCharsquence,mLength,buffer,len,NULL,NULL)]=0;
    return string(buffer);
}
String::String(const wchar_t* str,uint32_t len){
    if(!len)
        len=wstrlen(str);
    mLength=len;
    mCharsquence=new wchar_t[len];
    memcpy(mCharsquence,str,len*sizeof(wchar_t));
}
int String::IndexOf(const wchar_t ch,int startIndex)const{
    while (startIndex<mLength)
        if(mCharsquence[startIndex++]==ch)
            return startIndex-1;
    return -1;
    
}
int String::IndexOf(const wchar_t* str,int startIndex)const {
    uint32_t cmp_len=wstrlen(str);
    if(mLength<cmp_len)return -1;
    for(startIndex=IndexOf(str[0],startIndex);(startIndex!=-1)&&((mLength-startIndex)>=cmp_len);startIndex=IndexOf(str[0],startIndex+1)){
        if(wstrncmp(mCharsquence+startIndex,str,cmp_len))
            return startIndex;
    }
    return -1;
}
String::~String(){
    delete[] mCharsquence;
}
String::String(){
    mCharsquence=nullptr;
    mLength=0;
}
int String::LastIndexOf(const wchar_t* str) const{
    return LastIndexOf(str,mLength-1);
}
int String::LastIndexOf(const wchar_t ch,int startIndex)const {
    while (startIndex>=0)
        if(mCharsquence[startIndex--]==ch)
            return startIndex+1;
    return -1;
}
String& String::Replace(const wchar_t* search,const wchar_t* replacement,int startIndex){
    int index=IndexOf(search,startIndex);
    uint32_t len1=wstrlen(search),len2=wstrlen(replacement);
    if(len1==len2)
        memmove(mCharsquence+index,replacement,len2*sizeof(wchar_t));
    else if(len1<len2){
        auto new_len=mLength-len1+len2;
        wchar_t* buffer=new wchar_t[new_len];
        memmove(buffer,mCharsquence,index*sizeof(wchar_t));
        memmove(buffer+index,replacement,len2*sizeof(wchar_t));
        if(mLength-index-len1>0)
            memmove(buffer+index+len2,mCharsquence+index+len1,sizeof(wchar_t)*(mLength-index-len1));
        delete[] mCharsquence;
        mCharsquence=buffer;
        mLength=new_len;
    }else{
        memmove(mCharsquence+index,replacement,len2*sizeof(wchar_t));
        memmove(mCharsquence+index+len2,mCharsquence+index+len1,sizeof(wchar_t)*(mLength-index-len1));
    }
    return *this;
}
String& String::ReplaceAll(const wchar_t* search,const wchar_t* replacement){
    uint32_t count=Count(search);
    if(count>0){
        uint32_t len1=wstrlen(search),len2=wstrlen(replacement);
        if(len1==len2){
            for(int i=IndexOf(search,0);i!=-1;i=IndexOf(search,i+len1))
                memmove(mCharsquence+i,replacement,len1*sizeof(wchar_t));
        }else{
            uint32_t new_len=mLength+count*(len2-len1);
            auto buffer=new wchar_t[new_len];    
            uint32_t lastIndex=0,buf_offset=0;
            int i=IndexOf(search,lastIndex);
            bool is_start=true;
            for(;true;lastIndex=i,i=IndexOf(search,i+len1)){
                if(i==-1){
                    if(lastIndex+len1!=mLength)
                        memmove(buffer+buf_offset,mCharsquence+lastIndex+len1,sizeof(wchar_t)*(mLength-lastIndex-len1)),buf_offset+=mLength-lastIndex-len1;
                    break;
                }
                if(is_start){
                    if(i!=0)
                        memmove(buffer+buf_offset,mCharsquence,i*sizeof(wchar_t)),buf_offset+=i;
                    memmove(buffer+buf_offset,replacement,len2*sizeof(wchar_t)),buf_offset+=len2;
                    is_start=false;
                }else{
                    if(i-lastIndex>len1)
                        memmove(buffer+buf_offset,mCharsquence+lastIndex+len1,sizeof(wchar_t)*(i-lastIndex-len1)),buf_offset+=i-lastIndex-len1;
                    memmove(buffer+buf_offset,replacement,sizeof(wchar_t)*len2),buf_offset+=len2;
                }
            }
            delete[] mCharsquence;
            mCharsquence=buffer;
            mLength=new_len;
        }
    }
    return *this;
    
}
String& String::Remove(uint32_t length,uint32_t start){
    if(start<0||(length>mLength))throw L"invalid start or length out of range";
    auto buffer=new wchar_t[mLength-length];
    if(start==0)//如果删除开头
        memmove(buffer,mCharsquence+length,(mLength-length)*sizeof(wchar_t));
    else if(start+length==mLength)//如果删除结尾
        memmove(buffer,mCharsquence,(mLength-length)*sizeof(wchar_t));
    else//如果删除中间
    {
        uint32_t offset1=0,len1=start,offset2=start+length,len2=mLength-len1-length;
        memmove(buffer,mCharsquence+offset1,len1*sizeof(wchar_t));
        memmove(buffer+len1,mCharsquence+offset2,len2*sizeof(wchar_t));
    }
    delete[] mCharsquence;
    mCharsquence=buffer;
    mLength=mLength-length;
    return *this;
}
String& String::SubString(uint32_t start){
    return SubString(start,mLength-start);
}
String& String::SubString(uint32_t start,uint32_t length){
    if(start>=mLength||(mLength-start<length))throw L"invalid argument";
    auto buffer=new wchar_t[length];
    memmove(buffer,mCharsquence+start,length*sizeof(wchar_t));
    delete[] mCharsquence;
    mCharsquence=buffer;
    mLength=length;
    return *this;
}
String::String(const String& str){
    mLength=str.mLength;
    mCharsquence=new wchar_t[str.mLength];
    memmove(mCharsquence,str.mCharsquence,mLength*sizeof(wchar_t));
}
uint32_t String::Count(const wchar_t* str)const{
    auto search_len=wstrlen(str);
    uint32_t count=0;
    for (int offset=IndexOf(str,0);(offset!=-1)&&(mLength-offset>=search_len);offset=IndexOf(str,offset+search_len))
        count++;
    return count;
}
int String::LastIndexOf(const wchar_t* str,int startIndex)const{
    uint32_t cmp_len=wstrlen(str);
    if(mLength<cmp_len)return -1;
    for(startIndex=LastIndexOf(str[0],startIndex);startIndex!=-1;startIndex=LastIndexOf(str[0],startIndex-1)){
        if((mLength-startIndex)<cmp_len)continue;
        if(wstrncmp(mCharsquence+startIndex,str,cmp_len))
            return startIndex;
    }
    return -1;
}
String::String(const char* charsquence,CodePage cp,uint32_t len){
    if(!len)len=strlen(charsquence);
    UINT codepage;
    switch (cp)
    {
    case CodePage::UTF8:
        codepage=CP_UTF8;
        break;
    case CodePage::GBK:
        codepage=936;
        break;
    case CodePage::SHIFT_JIS:
        codepage=932;
        break;
    case CodePage::ANSI:
        codepage=CP_ACP;
    default:
        break;
    }
    mLength=MultiByteToWideChar(codepage,0,charsquence,len,NULL,0);
    mCharsquence=new wchar_t[mLength];
    MultiByteToWideChar(codepage,0,charsquence,len,mCharsquence,mLength);
}
String& String::Insert(const wchar_t* str,uint32_t length){
    if(length==0)
        length=wstrlen(str);
    auto new_len=length+mLength;
    auto buffer=new wchar_t[new_len];
    memmove(buffer,str,sizeof(wchar_t)*length);
    memmove(buffer+length,mCharsquence,sizeof(wchar_t)*mLength);
    mLength=new_len;
    delete[] mCharsquence;
    mCharsquence=buffer;
    return *this;
}
String& String::Insert(const String& str){
    return Insert(str.mCharsquence,str.mLength);
}
