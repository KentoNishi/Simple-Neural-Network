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
    float value=0;
    float delta=0;
};

struct Test{
    vector<float> in;
    vector<float> out;
    Test(vector<float> a, vector<float> b){
        in=a;
        out=b;
    }
};

vector<int> config={1,2,1};
vector<vector<Node>> network;

float transfer(float activation){
    return 1/(1+exp(-activation));
}

float activate(vector<float> weights, vector<float> inputs){
    float activation=weights[weights.size()-1];
    for(int i=0;i<weights.size()-1;i++){
        activation+=weights[i]*inputs[i];
    }
    return activation;
}

vector<float> forwardPropagate(vector<float> input){
    for(int y=0;y<network[0].size();y++){
        network[0][y].value=transfer(input[y]);
    }
    for(int x=1;x<network.size();x++){
        vector<float> newInputs;
        for(int y=0;y<network[x].size();y++){
//            cout << network[x][y].weights.size() << endl;
            float activation=activate(network[x][y].weights,input);
            network[x][y].value=transfer(activation);
//            cout << input[0] << " results in "<<transfer(activation)<< endl;
            newInputs.push_back(network[x][y].value);
        }
        input=newInputs;
    }
    return input;
}

vector<float> backwardPropagate(vector<float> output){
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
                errors.push_back(output[y]*transfer(network[x][y].value));
            }
        }
        for(int y=0;y<network[x].size();y++){
            network[x][y].delta=errors[y]*transfer(network[x][y].value);
        }
    }
    return {};
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

void display(){
    for(int x=0;x<network.size();x++){
        if(x!=0){
            cout << endl;
            for(int y=0;y<network[x].size();y++){
                cout << " [[ ";
                for(int i=0;i<network[x][y].weights.size();i++){
                    cout << network[x][y].weights[i]<<" ";
                }
                cout << "]] "<<"  ";
            }
            cout << endl;
        }
        for(int y=0;y<network[x].size();y++){
            cout << "(V:"<<network[x][y].value<<", D: "<<network[x][y].delta<<")"<< " ";
        }
    }
    cout << endl<<endl;
}

void updateWeights(vector<float> input, float rate){
    for(int x=0;x<network.size();x++){
        vector<Node> inputs=network[network.size()-1];
        if(x!=0){
            inputs={};
            for(int y=0;y<network[x-1].size();y++){
                inputs.push_back(network[x-1][y]);
            }
            for(int y=0;y<network[x].size();y++){
                for(int k=0;k<inputs.size();k++){
//                    cout << k << " in a list of " << inputs.size() << ", with coords "<< x << "," << y<<", size of weight vector is " << network[x][y].weights.size()<< endl;
                    network[x][y].weights[k]+=rate*network[x][y].delta*inputs[k].value;
                }
                network[x][y].weights[network[x][y].weights.size()-1]+=rate*network[x][y].delta;
            }
        }
    }
}

void train(vector<Test> dataset, float rate, int epoch){
    for(int i=0;i<epoch;i++){
        float err=0;
        for(int k=0;k<dataset.size();k++){
            for(int j=0;j<dataset[k].in.size();j++){
                cout << "Test Case: "<<dataset[k].in[j] << " ";
            }
            cout <<"= "<<dataset[k].out[0]<< endl;
            vector<float> result=forwardPropagate(dataset[k].in);
            err+=cost(result,dataset[k].out);
            backwardPropagate(dataset[k].out);
            updateWeights(dataset[k].in,0.1);
            display();
        }
    }
}

int main(){
    vector<Test> dataset;
    for(int i=0;i<100;i++){
        float randomNum=(rand()%2);
        dataset.push_back(Test({randomNum},{1-randomNum}));
    }
    initNet();
    train(dataset,0.1,dataset.size());
    return 0;
}