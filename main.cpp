#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
using namespace std;

vector<int> config={1,2,1};
vector<vector<Node>> network;
struct Test{
    vector<float> in;
    vector<float> out;
    Test(vector<float> a, vector<float> b){
        in=a;
        out=b;
    }
};

struct Node{
    vector<float> weights;
    int value=0;
    int delta=0;
};

vector<float> forwardPropagate(vector<float> input){
    
}

vector<float> backwardPropagate(vector<float> output){
    
}

void initNet(){
    for(int x=0;x<config.size();x++){
        network.push_back({});
        for(int y=0;y<config[x];y++){
            network[x].push_back(Node());
            if(x!=0){
                for(int i=0;i<config[x-1];i++){
                    network[x][y].weights.push_back(float(rand()%3000)/3000-0.5);
                }
            }
        }
    }
}

float cost(vector<float> result, vector<float> output){
    float ans=0;
    for(int i=0;i<result.size();i++){
        ans+=pow(output[i]-result[i],2);
    }
    return ans;
}

void updateWeights(vector<float> input, float rate){

}

void train(vector<Test> dataset, float rate, int epoch){
    for(int i=0;i<epoch;i++){
        float err=0;
        for(int k=0;k<dataset.size();k++){
            vector<float> result=forwardPropagate(dataset[k].in);
            err+=cost(result,dataset[k].out);
            backwardPropagate(dataset[k].out);
            updateWeights(dataset[k].in,0.1);
        }
    }
}

int main(){
    vector<Test> dataset;
    for(int i=0;i<10;i++){
        dataset.push_back(Test({float(i)},{float(-2*i)}));
    }
    initNet(network);
    train(network,dataset,0.5,100);
    return 0;
}