#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include <cmath>
using namespace std;

struct Parameters{
    float rate;
    int epoch;
    float error;
};

struct Node{
    vector<float> weights;
    float output;
    float delta;
};

class NeuralNet{
    public:
        vector<vector<Node>> model;
        vector<float> outputSet;
        NeuralNet(vector<vector<float>> dataset,vector<int> layout,Parameters params){
            float rate=params.rate;
            int epoch=params.epoch;
            float error=params.error;
            set<float> outputTypes;
            for(int i=0;i<dataset.size();i++){
                outputTypes.insert(dataset[i][dataset[i].size()-1]);
            }
            int inputSize=dataset[0].size()-1;
            int outputSize=outputTypes.size();
            int hiddenCount=layout.size();
            vector<int> hiddenSize=layout;
            outputSet=vector<float>(outputTypes.size());
            copy(outputTypes.begin(), outputTypes.end(), outputSet.begin());
            initNet(inputSize,hiddenCount,hiddenSize,outputSize);
            trainNet(dataset,rate,epoch,error,outputSize);
        }
        NeuralNet(){

        }
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
            for(int x=0;x<model.size();x++){
                vector<float> newInputs;
                for(int y=0;y<model[x].size();y++){
                    float activation=activate(model[x][y].weights,inputs);
                    model[x][y].output=transfer(activation);
                    newInputs.push_back(model[x][y].output);
                }
                inputs=newInputs;
            }
            return inputs;
        }

        void backwardPropagate(vector<float> expected){
            for(int x=model.size()-1;x>=0;x--){
                vector<float> errors;
                if(x!=model.size()-1){
                    for(int y=0;y<model[x].size();y++){
                        float error=0;
                        for(int k=0;k<model[x+1].size();k++){
                            error+=model[x+1][k].weights[y]*model[x+1][k].delta;
                        }
                        errors.push_back(error);
                    }
                }else{
                    for(int y=0;y<model[x].size();y++){
                        errors.push_back(expected[y]-model[x][y].output);
                    }
                }
                for(int y=0;y<model[x].size();y++){
                    model[x][y].delta=errors[y]*derivative(model[x][y].output);
                }
            }
        }

        float randomFloat(){
            float randomNum=float(rand()%10000000)/5000000-1;
            return randomNum;
        }

        void initNet(int inputSize, int hiddenCount, vector<int> hiddenSize, int outputSize){
            vector<vector<Node>> hiddenLayers;
            for(int layer=0;layer<hiddenCount;layer++){
                hiddenLayers.push_back({});
                for(int i=0;i<hiddenSize[layer];i++){
                    hiddenLayers[layer].push_back(Node());
                    for(int k=0;k<=inputSize;k++){
                        hiddenLayers[layer][i].weights.push_back(randomFloat());
                    }
                }
                inputSize=hiddenSize[layer];
            }
            for(int i=0;i<hiddenLayers.size();i++){
                model.push_back(hiddenLayers[i]);
            }
            vector<Node> outputLayer;
            for(int i=0;i<outputSize;i++){
                outputLayer.push_back(Node());
                for(int k=0;k<=hiddenSize[hiddenSize.size()-1];k++){
                    outputLayer[i].weights.push_back(randomFloat());
                }
            }
            model.push_back(outputLayer);
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
            for(int x=0;x<model.size();x++){
                if(x!=0){
                    inputs={};
                    for(int k=0;k<model[x-1].size();k++){
                        inputs.push_back(model[x-1][k].output);
                    }
                }
                for(int y=0;y<model[x].size();y++){
                    for(int k=0;k<inputs.size();k++){
                        model[x][y].weights[k]+=rate*model[x][y].delta*inputs[k];
                    }
                    model[x][y].weights[model[x][y].weights.size()-1]+=rate*model[x][y].delta;
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
                cout <<"Epoch: "<<iter<<", Rate: "<<rate <<", Minimum Error: " << minError <<", Error: "<<errorSum<< endl;
            }
        }

        void display(){
            for(int i=0;i<model[0][0].weights.size()-1;i++){
                cout << "(-) ";
            }
            cout << endl;
            for(int x=0;x<model.size();x++){
                for(int y=0;y<model[x].size();y++){
                    cout << "[ ";
                    for(int i=0;i<model[x][y].weights.size();i++){
                        int maximum=model[x][y].weights.size()-1;
                        cout << (i==maximum?"{":"(") <<model[x][y].weights[i] << (i==maximum?"} ":") ");
                    }
                    cout << "] ";
                }
                cout << endl;
                for(int y=0;y<model[x].size();y++){
                    cout << "(-)"<<" ";
                }
                cout << endl;
            }
        }
        pair<vector<float>,pair<int,float>> test(vector<float> testCase){
            vector<float> result=forwardPropagate(testCase);
            int id=0;
            for(int i=1;i<result.size();i++){
                if(result[i]>result[id]){
                    id=i;
                }
            }
            return make_pair(result,make_pair(id,testCase[testCase.size()-1]));
        }
};

int main(){
    vector<vector<float>> dataset;
    /* sample case = {
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
    };*/
    for(int i=0;i<100;i++){
        float num=rand()%10-5;
        dataset.push_back({num, int(num)%2==0?2*num:(rand()%10-5),float(int(num)%2==0?1:0)});
    }
    Parameters params=Parameters();
    params.epoch=100000;
    params.error=0.001;
    params.rate=0.1;
    NeuralNet network=NeuralNet(dataset,{10,10},params);
    for(int k=0;k<10000;k++){
        float num=rand()%10-5;
        pair<vector<float>,pair<int,float>> test=network.test({num, int(num)%2==0?2*num:(rand()%10-5),float(int(num)%2==0?1:0)});
        cout << (test.second.second==network.outputSet[test.second.first]?"[OK]":"[NG]") << " ";
        for(int i=0;i<test.first.size();i++){
            cout << network.outputSet[i] << ": "<< test.first[i]*100 << "%   ";
        }
        cout << " Expected: " << test.second.second << ", returned: "<<network.outputSet[test.second.first] << endl;
    }
    return 0;
}