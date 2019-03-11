#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
using namespace std;

struct Edge;
struct Node;
struct Node{
    vector<Edge> edges;
    float value=0;
};
struct Edge{
    float weight;
    Edge(float w){
        weight=w;
    }
};

float sigmoid(float x){
    return exp(x)/(exp(x)+1);
}

vector<vector<Node>> propagate(vector<int> inputs,vector<vector<Node>> network){
    for(int i=0;i<inputs.size();i++){
        network[0][i].value+=inputs[i];
    }
    for(int x=0;x<network.size()-1;x++){
        for(int y=0;y<network[x].size();y++){
            network[x][y].value=sigmoid(network[x][y].value);
            for(int i=0;i<network[x][y].edges.size();i++){
                network[x+1][i].value+=(network[x][y].edges)[i].weight*network[x][y].value;
            }
        }
    }
    
    for(int i=0;i<network[network.size()-1].size();i++){
        network[network.size()-1][i].value=sigmoid(network[network.size()-1][i].value);
    }
    return network;
}

float error(vector<vector<Node>> network, vector<int> outputs){
    float cost=0;
    for(int i=0;i<network[network.size()-1].size();i++){
        cost+=0.5*pow(network[network.size()-1][i].value-outputs[i],2);
    }
    return cost;
}

int main(){
    vector<vector<Node>> network;
    vector<int> model={4,4,4};
    for(int x=0;x<model.size();x++){
        network.push_back({});
        for(int y=0;y<model[x];y++){
            network[x].push_back(Node());
        }
    }
    for(int x=0;x<network.size()-1;x++){
        for(int y=0;y<network[x].size();y++){
            for(int i=0;i<network[x+1].size();i++){
                (network[x][y].edges).push_back(Edge(1));
            }
        }
    }
    vector<vector<Node>> result=propagate({1,2,3,4},network);
    float cost=error(result,{4,3,2,1});
    for(int x=0;x<result.size();x++){
        for(int y=0;y<result[x].size();y++){
            cout << result[x][y].value << " ";
        }
        cout << endl;
    }
    backpropagate({4,3,2,1},result);
    return 0;
}