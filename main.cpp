#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
using namespace std;

struct Node{
    vector<float> weights;
    float output;
    float delta;
};

vector<vector<Node>> network;

void initNet(int inputSize, int hiddenSize, int outputSize){
    vector<Node> hiddenLayer;
    for(int i=0;i<hiddenSize;i++){
        hiddenLayer.push_back(Node());
        for(int k=0;k<=inputSize;k++){
            hiddenLayer[i].weights.push_back(2*((rand()%100)/50-0.5));
        }
    }
    network.push_back(hiddenLayer);
    vector<Node> outputLayer;
    for(int i=0;i<outputSize;i++){
        outputLayer.push_back(Node());
        for(int k=0;k<=hiddenSize;k++){
            outputLayer[i].weights.push_back(2*((rand()%100)/50-0.5));
        }
    }
    network.push_back(outputLayer);
}

void trainNet(vector<vector<float>> dataset, float rate, int epoch, int outputSize){
    for(int iter=0;iter<epoch;iter++){
        float errorSum=0;
        for(int set=0;set<dataset.size();set++){
            
            for(int x=0;x<network.size();x++){
                for(int y=0;y<network[x].size();y++){
                    cout << network[x][y].output << " ";
                }
                cout << endl;
                for(int y=0;y<network[x].size();y++){
                    cout << "[ ";
                    for(int i=0;i<network[x][y].weights.size();i++){
                        int maximum=network[x][y].weights.size()-1;
                        cout << (i==maximum?"{":"(") <<network[x][y].weights[i] << (i==maximum?"} ":") ");
                    }
                    cout << "] ";
                }
                cout << endl;
            }
        }
    }
}

int main(){
    vector<vector<float>> dataset = {
        {2.7810836,2.550537003,0},
        {1.465489372,2.362125076,0},
        {3.396561688,4.400293529,0},
        {1.38807019,1.850220317,0},
        {3.06407232,3.005305973,0},
        {7.627531214,2.759262235,1},
        {5.332441248,2.088626775,1},
        {6.922596716,1.77106367,1},
        {8.675418651,-0.242068655,1},
        {7.673756466,3.508563011,1}
    };
    int inputSize=dataset[0].size()-1;
    set<float> outputTypes;
    for(int i=0;i<dataset.size();i++){
        outputTypes.insert(dataset[i][dataset[i].size()-1]);
    }
    int outputSize=outputTypes.size();
    int hiddenSize=2;
    initNet(inputSize,hiddenSize,outputSize);
    trainNet(dataset,0.5,20,outputSize);
    return 0;
}