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
    float delta;
};
struct Edge{
    float weight;
    Edge(float w){
        weight=w;
    }
};

float activation(float x){
//    return x;
    return exp(x)/(exp(x)+1);
}

float derivative(float x){
//    return 1;
    return x * (1.0 - x);
}

vector<vector<Node>> propagate(vector<int> inputs,vector<vector<Node>> network){
    for(int i=0;i<inputs.size();i++){
        network[0][i].value+=inputs[i];
    }
    for(int x=0;x<network.size()-1;x++){
        for(int y=0;y<network[x].size();y++){
            network[x][y].value=activation(network[x][y].value);
            for(int i=0;i<network[x][y].edges.size();i++){
                network[x+1][i].value+=(network[x][y].edges)[i].weight*network[x][y].value;
            }
        }
    }
    
    for(int i=0;i<network[network.size()-1].size();i++){
        network[network.size()-1][i].value=activation(network[network.size()-1][i].value);
    }
    return network;
}

vector<vector<Node>> backpropagate(vector<float> expected,vector<vector<Node>> network){
    for(int x=network.size()-1;x>=0;x--){
        vector<float> errors;
        if(x==network.size()-1){
            for(int y=0;y<network[x].size();y++){
                errors.push_back(pow(expected[y]-network[x][y].value,2));
            }
        }else{
            for(int y=0;y<network[x].size();y++){
                float err=0;
                for(int i=0;i<network[x+1].size();i++){
                    err+=(network[x+1][i].);
                }
            }
        }
    }
}
/*
vector<float> error(vector<vector<Node>> network, vector<int> outputs){
    vector<float> cost;
    for(int i=0;i<network[network.size()-1].size();i++){
        cost.push_back(pow(network[network.size()-1][i].value-outputs[i],2));
    }
    return cost;
}*/

void print(vector<vector<Node>> result){
    for(int x=result.size()-1;x>=0;x--){
        for(int y=0;y<result[x].size();y++){
            cout << result[x][y].value << " ";
        }
        cout << endl;
    }
}

int main(){
    vector<vector<Node>> network;
    vector<int> model={3,3,1};
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
    vector<vector<Node>> result=propagate({1,2,3},network);/*
    vector<float> cost=error(result,{3});
    for(int i=0;i<cost.size();i++){
        result[result.size()-1][i].delta=cost[i];
    }*/
    print(result);
    result=backpropagate({3},result);
    print(result);
    return 0;
}