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

struct Node{
    vector<float> weights={};
    float activation=0;
    float value=0;
    float delta=0;
};

vector<int> classes={};

struct Sample{
    vector<float> input;
    int expected;
    Sample(vector<float> i, int e){
        input=i;
        expected=e;
    }
};

struct Network{
    vector<vector<Node>> model;
    vector<Sample> dataset;
    vector<int> config;
    Network(){   
    }
    float activation(float activation){
        return 1/(1+exp(-activation));
    }
    float derivative(float out){
        return out*(1-out);
    }
    void print(vector<vector<Node>>& clone){
        for(int x=0;x<clone.size();x++){
            for(int y=0;y<clone[x].size();y++){
                cout << "[A: " << clone[x][y].activation << ", D:" << clone[x][y].delta << "] ";
            }
            cout << endl;
            for(int y=0;x!=clone.size()-1&&y<clone[x].size();y++){
                cout << "[[ ";
                for(int k=0;k<clone[x][y].weights.size();k++){
                    cout << clone[x][y].weights[k] << " ";
                }
                cout << "]]" << " ";
            }
            cout << endl;
        }
    }
    void init(vector<Sample> samples,vector<int> conf){
        config=conf;
        config.insert(config.begin(),samples[0].input.size());
        config.push_back(classes.size());
        for(int x=0;x<config.size();x++){
            model.push_back({});
            for(int y=0;y<config[x];y++){
                model[x].push_back(Node());
                if(x!=config.size()-1){
                    model[x][y].weights=vector<float>(config[x+1]);
                    for(int k=0;k<model[x][y].weights.size();k++){
                        model[x][y].weights[k]=float(rand()%200)/100-1;
                    }
                }
            }
        }
        dataset=samples;
    }
    void propagate(vector<vector<Node>>& clone,Sample sample){
        for(int y=0;y<clone[0].size();y++){
            clone[0][y].value=sample.input[y];
        }
        for(int x=0;x<clone.size();x++){
            for(int y=0;y<clone[x].size();y++){
                clone[x][y].activation=activation(clone[x][y].value);
                for(int k=0;k<clone[x][y].weights.size();k++){
                    clone[x+1][k].value+=clone[x][y].weights[k]*clone[x][y].activation;
                }
            }
        }
    }
    void backprop(vector<vector<Node>>& clone){
        for(int x=clone.size()-2;x>=0;x--){
            for(int y=0;y<clone[x].size();y++){
                float total=0;
                for(int k=0;k<clone[x][y].weights.size();k++){
                    total+=clone[x][y].weights[k]*clone[x+1][k].delta;
                }
                clone[x][y].delta=total*derivative(clone[x][y].activation);
            }
        }
    }
    void update(vector<vector<Node>>& clone){
        for(int x=clone.size()-2;x>=0;x--){
            for(int y=0;y<clone[x].size();y++){
                for(int k=0;k<clone[x][y].weights.size();k++){
                    model[x][y].weights[k]+=0.01*clone[x+1][k].delta*clone[x][y].activation;
                }
            }
        }
    }
    float cost(vector<vector<Node>>& clone,Sample sample){
        float total=0;
        for(int i=0;i<clone[clone.size()-1].size();i++){
            total+=0.5*pow((sample.expected==i?1:0)-clone[clone.size()-1][i].activation,2);
            clone[clone.size()-1][i].delta=(sample.expected==i?1:0)-clone[clone.size()-1][i].activation;
        }
        return total;
    }
    void train(float rate, float minError){
        float totalError=INFINITY;
        int i;
        for(i=0;minError<totalError;i++){
            totalError=0;
            for(int k=0;k<dataset.size();k++){
                vector<vector<Node>> clone=model;
                propagate(clone,dataset[k]);
                float caseCost=cost(clone,dataset[k]);
                backprop(clone);
                update(clone);
                totalError+=caseCost;
            }
            cout << "Epoch: " << i+1 << ", Total Error: " << totalError << endl;
        }
        cout << "Finished training in " << i << " epochs" << endl;
    }
};

int main(){
    Network network=Network();
    vector<Sample> samples;
    set<int> classSet;
    for(int i=0;i<10;i++){
        int a=rand()%2;
        int b=rand()%2;
        int c=(a&b);
        Sample sample=Sample({float(a),float(b)},c);
        samples.push_back(sample);
    }
    for(int i=0;i<samples.size();i++){
        classSet.insert(samples[i].expected);
    }
    classes=vector<int>(classSet.size());
    copy(classSet.begin(),classSet.end(),classes.begin());
    for(int i=0;i<samples.size();i++){
        samples[i].expected=distance(classes.begin(),find(classes.begin(),classes.end(),samples[i].expected));
    }
    network.init(samples,{2,2});
    network.train(0.1,0.001);
    return 0;
}
