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

float activate(vector<float> weights, vector<float> inputs){
    float activation=weights[weights.size()-1];
    for(int i=0;i<weights.size()-1;i++){
        activation+=weights[i] * inputs[i];
    }
    return activation;
}

float transfer(float activation){
    return 1/(1+exp(-activation));
}

float derivative(float out){
    return out*(1-out);
}

vector<float> forwardPropagate(vector<float> inputs){/*
    for(int i=0;i<inputs.size();i++){
        cout << inputs[i] << " ";
    }
    cout << endl;*/
    for(int x=0;x<network.size();x++){
        vector<float> newInputs;
        for(int y=0;y<network[x].size();y++){
            float activation=activate(network[x][y].weights,inputs);
            network[x][y].output=transfer(activation);
            newInputs.push_back(network[x][y].output);
        }
        inputs=newInputs;
    }
    return inputs;
}

void backwardPropagate(vector<float> expected){
    for(int x=network.size()-1;x>=0;x--){
        vector<float> errors;
        if(x!=network.size()-1){
            for(int y=0;y<network[x].size();y++){
                float error=0;
                for(int k=0;k<network[x+1].size();k++){
                    error+=network[x+1][k].weights[y]*network[x+1][k].delta;
                }
                errors.push_back(error);
            }
        }else{
            for(int y=0;y<network[x].size();y++){
                errors.push_back(expected[y]-network[x][y].output);
            }
        }
        for(int y=0;y<network[x].size();y++){
            network[x][y].delta=errors[y]*derivative(network[x][y].output);
        }
    }
}

float randomFloat(){
    float randomNum=float(rand()%10000000)/5000000-1;
    return randomNum;
}

void initNet(int inputSize, int hiddenCount, int hiddenSize, int outputSize){
    vector<vector<Node>> hiddenLayers;
    for(int layer=0;layer<hiddenCount;layer++){
        hiddenLayers.push_back({});
        for(int i=0;i<hiddenSize;i++){
            hiddenLayers[layer].push_back(Node());
            for(int k=0;k<=inputSize;k++){
                hiddenLayers[layer][i].weights.push_back(randomFloat());
            }
        }
    }
    for(int i=0;i<hiddenLayers.size();i++){
        network.push_back(hiddenLayers[i]);
    }
    vector<Node> outputLayer;
    for(int i=0;i<outputSize;i++){
        outputLayer.push_back(Node());
        for(int k=0;k<=hiddenSize;k++){
            outputLayer[i].weights.push_back(randomFloat());
        }
    }
    network.push_back(outputLayer);
}

float cost(vector<float> expected,vector<float> outputs){
    float total=0;
    for(int i=0;i<expected.size();i++){
        total+=pow(expected[i]-outputs[i],2);
    }
    return total;
}

void updateWeights(vector<float> data, float rate){
    vector<float> inputs=data;
    inputs.erase(inputs.end()-1);
    for(int x=0;x<network.size();x++){
        if(x!=0){
            inputs={};
            for(int k=0;k<network[x-1].size();k++){
                inputs.push_back(network[x-1][k].output);
            }
        }
        for(int y=0;y<network[x].size();y++){
            for(int k=0;k<inputs.size();k++){
                network[x][y].weights[k]+=rate*network[x][y].delta*inputs[k];
            }
            network[x][y].weights[network[x][y].weights.size()-1]+=rate*network[x][y].delta;
        }
    }
}

void trainNet(vector<vector<float>> dataset, float rate, int epoch, float minError,int outputSize){
    float errorSum=-1;
    for(int iter=0;(iter<epoch||epoch==0)&&(errorSum==-1||errorSum>minError||minError==0);iter++){
        errorSum=0;
        for(int set=0;set<dataset.size();set++){
            vector<float> outputs=forwardPropagate(dataset[set]);
            vector<float> expected=vector<float>(outputSize);
            expected[dataset[set][dataset[set].size()-1]]=1;
            errorSum+=cost(expected,outputs);
            backwardPropagate(expected);
            updateWeights(dataset[set],rate);
        }
        cout <<">Epoch: "<<iter<<", Rate: "<<rate<<", Error: "<<errorSum<< endl;
    }
}

void display(){
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
    set<float> outputTypes;
    for(int i=0;i<dataset.size();i++){
        outputTypes.insert(dataset[i][dataset[i].size()-1]);
    }
    int inputSize=dataset[0].size()-1;
    int outputSize=outputTypes.size();
    int hiddenCount=2;
    int hiddenSize=2;
    vector<float> outputVector=vector<float>(outputTypes.size());
    copy(outputTypes.begin(), outputTypes.end(), outputVector.begin());
    initNet(inputSize,hiddenCount,hiddenSize,outputSize);
    trainNet(dataset,0.1,0,0.001,outputSize);
    return 0;
}