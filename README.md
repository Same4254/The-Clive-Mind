# The Clive Mind

Let me start off by saying this is an educational project. I want to learn and construct neural networks. If this project were more about the results than the adventure, I would use a lirbary.

This is a cluster of 20 Dell Optiplex 755's...

![](/Cluster.jpg)

I saw a surpluss posting for these computers from a local high school and instantly jumped in the auction. My idea for these computers is to train neural networks for different projects I want to do. However, I want to take the educational approach to this and construct the neural network library myself, and use MPI to communicate between computers. 

## Current Progress

So far I have built the vanilla fully-connected neural network and a convolutional model. These have both seen success on the MNIST data set, with both scoring roughly 90% after one epoch of training.

## What I want to achieve

I would like to make a builder class for the nerural network because right now the process to set up the model is tedious and prone to errorr. 

On top of that, I would like to use MPI to communicate grradient and network paramters between computers in order to maximize the amount of work done. Once this basic step is complete, I will be able to experiment with different models of parallelization of network training. This cluster is a bit unconventional because each worker node is not very good on its own, so it is key that the work be divided efficiently to get the most out of this setup. 

I would also like to make a webserver using NodeJS to enable live "footage" of the training process. In addition, this would allow me to analyze different learning methods and experiment.
