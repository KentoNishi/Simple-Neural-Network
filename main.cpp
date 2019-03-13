#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
using namespace std;

float sigmoid(float val){
    return 1/(1+exp(-val));
}

float sigma(float val){
    return val*(1-val);
}

struct Edge{
    float weight;
    Edge(float w){
        weight=w;
    }
};

struct Vertex{
    float value=0;
    float delta=0;
    vector<Edge> edges;
    Vertex(float v){
        value=v;
    }
};

struct Data{
    vector<float> input;
    vector<float> output;
    Data(vector<float> i,vector<float> o){
        input=i;
        output=o;
    }
};

void display(vector<vector<Vertex>> network){
    for(int x=0;x<network.size();x++){
        for(int y=0;y<network[x].size();y++){
            cout << "(V: "<<network[x][y].value <<", D: "<<network[x][y].delta<<")"<< " ";
        }
        cout << endl;
    }
}

vector<vector<Vertex>> forwardPropagate(vector<vector<Vertex>> network,vector<float> input){
    for(int i=0;i<network[0].size();i++){
        network[0][i].value=sigmoid(input[i]);
    }
    for(int x=1;x<network.size();x++){
        for(int y=0;y<network[x].size();y++){
            for(int k=0;k<network[x-1].size();k++){
                network[x][y].value+=network[x-1][k].value;
            }
            network[x][y].value=sigmoid(network[x][y].value);
        }
    }
    return network;
}

void backPropagate(vector<vector<Vertex>>* network,vector<float> input){
    return;
}

void cost(vector<vector<Vertex>>* network,vector<float> output){
    for(int i=0;i<(*network)[(*network).size()-1].size();i++){
        Vertex* node=&(*network)[(*network).size()-1][i];
        (*node).delta=(output[i]-(*node).value)*sigma((*node).value);
    }
}

int main(){
    vector<vector<Vertex>> network;
    vector<int> config={3,3,1};
    for(int x=0;x<config.size();x++){
        network.push_back({});
        for(int y=0;y<config[x];y++){
            network[x].push_back(Vertex(0));
            if(x!=0){
                for(int i=0;i<network[x-1].size();i++){
                    float randomWeight=float(random()%1000)/500-1;
                    network[x][y].edges.push_back(Edge(randomWeight));
                }
            }
        }
    }
    vector<Data> dataSet;
    for(int i=0;i<10;i++){
        vector<float> test;
        float solution=0;
        for(int k=0;k<3;k++){
            float randomNumber=float(random()%3)+1;
            test.push_back(randomNumber);
            solution=max(randomNumber,solution);
        }
        dataSet.push_back(Data(test,{solution}));
    }
    for(int i=0;i<dataSet.size();i++){
        for(int k=0;k<dataSet[i].input.size();k++){
            cout << dataSet[i].input[k] << " ";
        }
        cout << "= "<<dataSet[i].output[0] << endl;
    }
    cout << endl;
    for(int i=0;i<dataSet.size();i++){
        vector<vector<Vertex>> result=forwardPropagate(network,dataSet[i].input);
        cost(&result,dataSet[i].output);
        display(result);
        backPropagate(&result,dataSet[i].output);
        cout << endl;
    }
    return 0;
}
