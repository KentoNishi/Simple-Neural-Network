# Simple Neural Network

Simple neural network implementation in C++.

## Download
Clone the repository with git.
```
git clone https://github.com/KentoNishi/Simple-Neural-Network.git
```
Switch to the v1 branch.
```
git checkout v1
```
## Setup

Navigate to the root directory of the repository.

The directory contains the following configuration files:

| Filename      | Type          | Description                                               |
| ------------- |:-------------:|:----------------------------------------------------------|
| "config.in"   | Configuration | Contains information about the network configuration.     |
| "dataset.in"  | Input         | Contains the training dataset for the network.            |
| "tests.in"    | Input         | Contains test cases for the network to try after training.|

### config.in:
```
1. Maximum epoch
2. Minimum average error
3. Learning rate
4. Hidden layer configuration (space separated integer list)
```
Example:
```
10000
0.001
0.01
10 10
```

### dataset.in:

Each line in the file represents a data sample in the following format:

```
[space separated int/float list of input values] [expected int/float result]
```
Example:
``
2.7810836 2.550537003 0
``

### tests.in:

Each line in the file represents a test case in the following format:

```
[space separated int/float list of input values] [expected int/float result]
```
Example:
``
7.673756466 3.508563011 1
``

## Customizations

Navigate to the root directory of the repository, then execute the following command.
```
./generate
```
This command will compile and run `generate.cpp` to create a custom dataset.
Editing `generate.cpp` will enable you to create your own datasets to train and test the neural network.


## Execution

Navigate to the root directory of the repository, then execute the following command.

```
./run
```

The network will automatically train on the dataset until it reaches the maximum epoch or reaches the minimum error.

## Issues
* Performance issues. Highly unoptimized and slow, with no parallel processing.
* No save or backup features. Data can be easily lost.
* The code uses stochastic gradient descent, causing speed issues.
* The code uses sigmoid activation functions, causing speed issues.
