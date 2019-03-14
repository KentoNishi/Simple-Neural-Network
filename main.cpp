#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
using namespace std;

float activation(float val){
    return 1/(1+exp(-val));
}

float derivative(float val){
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
        if(x!=0){
            for(int y=0;y<network[x].size();y++){
                cout << " [ ";
                for(int i=0;i<network[x][y].edges.size();i++){
                    cout << network[x][y].edges[i].weight<<" ";
                }
                cout << " ] ";
            }
            cout << endl;
        }
        for(int y=0;y<network[x].size();y++){
            cout << "(V: "<<network[x][y].value <<", D: "<<network[x][y].delta<<")"<< " ";
        }
        cout << endl;
    }
}

vector<vector<Vertex>> forwardPropagate(vector<vector<Vertex>> network,vector<float> input){
    for(int i=0;i<network[0].size();i++){
        network[0][i]=activation(input[i]);
    }
    for(int x=1;x<network.size();x++){
        for(int y=0;y<network[x].size();y++){
            for(int k=0;k<network[x-1].size();k++){
                network[x][y].value+=network[x-1][k].value*network[x][y].edges[k].weight;
            }
            network[x][y].value=activation(network[x][y].value);
//            cout << x << "," << y << " now becomes "<<network[x][y].value << endl;
        }
    }
    return network;
}

void backPropagate(vector<vector<Vertex>>* network,vector<float> input){
    for(int x=(*network).size()-2;x>=0;x--){
        vector<float> errors;
        for(int y=0;y<(*network)[x].size();y++){
            float error=0;
            for(int k=0;k<(*network)[x+1].size();k++){
                Vertex neuron=(*network)[x+1][k];
                error+=neuron.edges[y].weight*neuron.delta;
            }
            (*network)[x][y].delta=error*derivative((*network)[x][y].value);
        }
    }
    return;
}

void updateWeights(vector<vector<Vertex>>* network, float rate){
    for(int x=1;x<(*network).size();x++){
        for(int y=0;y<(*network)[x].size();y++){
            Vertex* neuron=&(*network)[x][y];
            for(int i=0;i<(*network)[x-1].size();i++){
                (neuron->edges)[i].weight+=rate*neuron->delta*(*network)[x-1][i].value;
            }
        }
    }
}

float cost(vector<vector<Vertex>>* network,vector<float> output){
    float ans=0;
    for(int i=0;i<(*network)[(*network).size()-1].size();i++){
        Vertex* node=&(*network)[(*network).size()-1][i];
        (*node).delta=pow(output[i]-(*node).value,2);
        ans+=(*node).delta;
    }
    return ans;
}

void updateNetwork(vector<vector<Vertex>>* network,vector<vector<Vertex>>* result){
    for(int x=0;x<(*network).size();x++){
        for(int y=0;y<(*network)[x].size();y++){
            for(int i=0;i<(*network)[x][y].edges.size();i++){
                (*network)[x][y].edges[i].weight=(*result)[x][y].edges[i].weight;
            }
        }
    }
}

int main(){
    vector<vector<Vertex>> network;
    vector<int> config={1,1,1};
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
    vector<Data> dataSet; //3 inputs, is the sum even?
    for(int i=0;i<10000;i++){
        vector<float> test;
        float solution=0;
        for(int k=0;k<config[0];k++){
            float randomNumber=float(random()%2);
            test.push_back(randomNumber);
        }
        solution=test[0];
        dataSet.push_back(Data(test,{solution}));
    }
    cout << endl;
    float err=0;
    for(int i=0;i<dataSet.size();i++){
        cout << endl << endl;
        for(int k=0;k<dataSet[i].input.size();k++){
            cout << dataSet[i].input[k] << " ";
        }
        cout << "= "<<dataSet[i].output[0] << endl;
        vector<vector<Vertex>> result=forwardPropagate(network,dataSet[i].input);
        display(network);
        cout << endl;
        err+=cost(&result,dataSet[i].output);
        backPropagate(&result,dataSet[i].output);
        updateWeights(&result,1);
        display(result);
        updateNetwork(&network,&result);
        cout << "ERROR: "<<float(err)/(i+1)<<endl;
        cout << endl;
    }
    return 0;
}
