# Simple Neural Network

Simple neural network implementation in C++ by Kento Nishi.

## Download
Clone the repository with git.
```
git clone https://github.com/KentoNishi/Simple-Neural-Network.git
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
```
[space separated int/float list of input values] [expected int/float result]
```
Example:
``
2.7810836 2.550537003 0
``

### tests.in:
```
[space separated int/float list of input values] [expected int/float result]
```
Example:
``
7.673756466 3.508563011 1
``


## Execution

Navigate to the root directory of the repository, then execute the following command.

```
./run
```