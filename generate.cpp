#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
#include <thread>
using namespace std;

vector<string> split(string str, string character){
    vector<string> result;
    long long s=0;
    long long i=0;
    while(i<str.length()){
        if(str[i]==character[0]||i==str.length()-1){
            long long x=i-s;
            if(i==str.length()-1){
                x++;
            }
            result.push_back(str.substr(s,x));
            s=i+1;
        }
        i++;
    }
    return result;
}

int main(){
    ofstream data ("dataset.in");
    ofstream tests ("tests.in");
    int size=10;
    int maximum=2;
    for(int i=0;i<size;i++){
        float a=rand()%(maximum);
        float b=rand()%(maximum);
        float c=int(a)^int(b);
        data << a << " " << b << " " << c;
        if(i!=size-1){
            data << endl;
        }
    }
    for(int i=0;i<size;i++){
        float a=rand()%(maximum);
        float b=rand()%(maximum);
        float c=int(a)^int(b);
        tests << a << " " << b << " " << c;
        if(i!=size-1){
            tests << endl;
        }
    }
}