# Simple-Neural-Network-v3
Inclusion of biases, on top of the [Simple Neural Network v2](https://github.com/KentoNishi/Simple-Neural-Network/tree/v2) project.

## Download
Clone the repository with git.
```
git clone https://github.com/KentoNishi/Simple-Neural-Network.git
```
Switch to the v3 branch.
```
git checkout v3
```

## Files
``main.cpp`` in the root directory contains the basic code for the neural network.

``autosaving`` contains the network with autosaving modifications.

## Breakdown
### Basic Network
The ``main`` function serves the following purposes.
```cpp
int main(){ // function definition
    Network network=Network(); // create network
    vector<Sample> samples; // list of samples
    set<int> classSet; // a set (ordered list with no duplicates) of output types
    // custom datasets can be created in the following loop.
    for(int i=0;i<10;i++){ // generate 10 samples
        int a=rand()%2; // random number a
        int b=rand()%2; // random number b
        int c=(a&b); // binary and operation
        Sample sample=Sample({float(a),float(b)},c); // list of float input values, one output value
        samples.push_back(sample); // push back sample to list
    }
    ... // data formatting for processing
    // layer configurations can be specified by a vector of integers.
    // layer configurations do not include the input and output layers.
    network.init(samples,{2,2}); // initialize the network with the vector of samples and a layer configuration
    network.train(0.1,0.001); // Specify a learning rate and minimum error value.
    return 0; // exit the program
} // end function
```
### Autosave Network
The autosave network has only one difference in the ``main`` function from the basic network.
Replace the following code:
```
network.init(samples,{2,2});
network.train(0.1,0.001);
```
with this line:
```
network.run(samples,{2,2},0.1,0.001);
```
The new line will attempt to find a backup file, but will fall back to the specified parameters.

## Execution
Compile and run the desired program.
```
g++ main.cpp -o result;./result;rm result;
```

## Other Information
* To learn more about neural networks, check out [this article](https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/) about backpropagation.