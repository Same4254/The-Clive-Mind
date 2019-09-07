package Layers;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ActivationFunction;
import Utilities.ActivationFunctions.SigmoidFunction;

public class FullyConnected extends Layer {
	private Matrix[] weights, biases, activations, nets;
	private Matrix[] weightVelocities, biasVelocities;
	
	private Matrix[] runningAverageWeightGradients, runningAverageBiasGradients;
	
	private int batchCount, batchSize = 1000;
	
	private int[] layerSizes;
	
	private ActivationFunction activationFunction;
	private double learningRate = 0.001;
	private double velocityCoeficcient = 0.9;
	
	private Matrix[] previousWeightGradients, previousBiasGradients;
	private Matrix[] previousWeightDeltas, previousBiasDeltas;
	private Matrix[] weightBackTrack;
	
	private double etaPlus = 1.2, etaMinus = 0.5, deltaMin = 1E-6, deltaMax = 50.0;
	
	public FullyConnected(NeuralNetwork network, int layer, int[] layerSizes) {
		super(network, layer);
		
		this.layerSizes = layerSizes;
		
		weights = new Matrix[layerSizes.length - 1];
		biases = new Matrix[layerSizes.length - 1];
		
		//Momentum
		weightVelocities = new Matrix[layerSizes.length - 1];
		biasVelocities = new Matrix[layerSizes.length - 1];
		
		//Batching
		runningAverageWeightGradients = new Matrix[layerSizes.length - 1];
		runningAverageBiasGradients = new Matrix[layerSizes.length - 1];
		
		//Resilient
		previousWeightGradients = new Matrix[layerSizes.length - 1];
		previousBiasGradients = new Matrix[layerSizes.length - 1];
		
		previousWeightDeltas = new Matrix[layerSizes.length - 1];
		previousBiasDeltas = new Matrix[layerSizes.length - 1];
		
		weightBackTrack = new Matrix[layerSizes.length - 1];
		
		//Feed Forward (Classic / Momentum)
		nets = new Matrix[layerSizes.length];
		activations = new Matrix[layerSizes.length];
		
		activationFunction = new SigmoidFunction();
		
		//Create Matrices
		for(int i = 1; i < layerSizes.length; i++) {
//			weights[layer - 1] = Matrix.random(layerSizes[layer], layerSizes[layer - 1], -1, 1);
			weights[i - 1] = Matrix.random(layerSizes[i], layerSizes[i - 1], 
									Math.sqrt(2.0 / (layerSizes[i] + layerSizes[i - 1])));
			
			weightVelocities[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1]);
			runningAverageWeightGradients[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1]);
			
//			biases[layer - 1] = Matrix.random(layerSizes[layer], 1, -1, 1);
			biases[i - 1] = Matrix.random(layerSizes[i], 1, Math.sqrt(2.0 / (layerSizes[i] + layerSizes[i - 1])));
			
			biasVelocities[i - 1] = new Matrix(layerSizes[i], 1);
			runningAverageBiasGradients[i - 1] = new Matrix(layerSizes[i], 1);
			
			previousWeightGradients[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1]);
			previousWeightDeltas[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1], 0.1);
			weightBackTrack[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1]);
			
			previousBiasGradients[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1]);
			previousBiasDeltas[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1], 0.1);
		}
	}
	
	@Override
	public void initialize() {
		Layer previousLayer = getPreviousLayer();
		
		if(previousLayer == null) {
			inputMatrixCount = network.getInputMatrixCount();
			inputNRows = network.getInputNRows();
			inputNCols = network.getInputNCols();
		} else {
			inputMatrixCount = previousLayer.getOutputMatrixCount();
			inputNRows = previousLayer.getOutputNRows();
			inputNCols = previousLayer.getOutputNCols();
		}
		
		outputMatrixCount = 1;
		outputNRows = layerSizes[layerSizes.length - 1];
	}
	
	/**
	 * Load the Neural Network from a file
	 * 
	 * @param file -> The File containing the neural network information
	 */
	public FullyConnected(NeuralNetwork network, int layer, File file) {
		super(network, layer);
		
		//Layer Sizes
		//activation function
		//learning rate, velocity coefficient
		//Weights
		//Biases
		//Weight Velocities
		//Bias Velocities
		
		Scanner sc = null;
		try {
			sc = new Scanner(file);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		String[] layerSizesParts = sc.nextLine().split(",");
		layerSizes = new int[layerSizesParts.length];
		
		for(int i = 0; i < layerSizesParts.length; i++)
			layerSizes[i] = Integer.parseInt(layerSizesParts[i]);
		
		weights = new Matrix[layerSizes.length - 1];
		biases = new Matrix[layerSizes.length - 1];
		
		weightVelocities = new Matrix[layerSizes.length - 1];
		biasVelocities = new Matrix[layerSizes.length - 1];
		
		runningAverageWeightGradients = new Matrix[layerSizes.length - 1];
		runningAverageBiasGradients = new Matrix[layerSizes.length - 1];
		
		nets = new Matrix[layerSizes.length];
		activations = new Matrix[layerSizes.length];
		
		activationFunction = new SigmoidFunction();
		
		//Learning Rate, Valocity Coefficient
		String[] parts = sc.nextLine().split(",");
		learningRate = Double.parseDouble(parts[0]);
		velocityCoeficcient = Double.parseDouble(parts[1]);
		
		for(int i = 1; i < layerSizes.length; i++) {
			runningAverageWeightGradients[i - 1] = new Matrix(layerSizes[i], layerSizes[i - 1]);
			runningAverageBiasGradients[i - 1] = new Matrix(layerSizes[i], 1);
		}

		for(int weight = 0; weight < weights.length; weight++) {
			weights[weight] = Matrix.parseFileString(sc.nextLine());
		}
		
		for(int bias = 0; bias < biases.length; bias++) {
			biases[bias] = Matrix.parseFileString(sc.nextLine());
		}
		
		for(int weightVelocity = 0; weightVelocity < weightVelocities.length; weightVelocity++) {
			weightVelocities[weightVelocity] = Matrix.parseFileString(sc.nextLine());
		}
		
		for(int biasVelocity = 0; biasVelocity < biasVelocities.length; biasVelocity++) {
			biasVelocities[biasVelocity] = Matrix.parseFileString(sc.nextLine());
		}
	}
	
	/**
	 * Save the neural network into a file. This will save the layer sizes, weights, biases, and learning information such as velocities
	 * 
	 * Format:
	 *   Layer Sizes
	 *   Learning Rate, Velocity Coefficient
	 *   Weights (The matrix for each layer is written as its own line)
	 *   Biases
	 *   Weight Velocities
	 *   Bias Velocities
	 * 
	 * @param file -> The file to write the information into
	 */
	public void save(File file) {
		PrintWriter writer = null;
		try {
			writer = new PrintWriter(file);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		//Layer Sizes
		for(int i = 0; i < layerSizes.length; i++) {
			if(i != layerSizes.length - 1)
				writer.print(layerSizes[i] + ",");
			else
				writer.println(layerSizes[i]);
		}
		
		//Activation Function
		
		//Learning Rate, Velocity Coefficient
		writer.println(learningRate + "," + velocityCoeficcient);
		
		//Weights
		for(int weightIndex = 0; weightIndex < weights.length; weightIndex++)
			writer.println(weights[weightIndex].fileString());
		
		//Biases
		for(int biasIndex = 0; biasIndex < biases.length; biasIndex++)
			writer.println(biases[biasIndex].fileString());
		
		//Weight Velocity
		for(int weightVelocityIndex = 0; weightVelocityIndex < weightVelocities.length; weightVelocityIndex++) {
			writer.println(weightVelocities[weightVelocityIndex].fileString());
		}
		
		//Bias Velocity
		for(int biasVelocityIndex = 0; biasVelocityIndex < biasVelocities.length; biasVelocityIndex++) {
			writer.println(biasVelocities[biasVelocityIndex].fileString());
		}
		
		writer.close();
	}

	/**
	 * This will take an input vector and feed it through the network and return the result.
	 * 
	 * In addition it will save the net value and activation of each neuron for the learning process.
	 * 
	 * Call this before calling a learning function
	 * 
	 * @param in -> Input data
	 * @return The resulting vector
	 */
	public Matrix forward(double[] in) {
		activations[0] = new Matrix(in.length, 1, in);
		nets[0] = new Matrix(in.length, 1, in);
		
		for(int i = 1; i < layerSizes.length; i++) {
			//This somehow runs faster than the below segment....
			Matrix net = weights[i - 1].multiply(activations[i - 1]).mAdd(biases[i - 1]);
			nets[i] = new Matrix(net);
			activations[i] = net.mForEach(activationFunction.getFunction());
			
//			nets[i] = weights[i - 1].multiply(activations[i - 1]).mAdd(biases[i - 1]);
//			activations[i] = nets[i].forEach(activationFunction.getFunction());
		}
		
		return activations[activations.length - 1];
	}
	
	@Override
	public Matrix[] forward(Matrix[] input) {
		double[] in = new double[layerSizes[0]];
		
		int index = 0;
		
		for(int i = 0; i < input.length; i++) {
			for(int row = 0; row < inputNRows; row++) {
			for(int col = 0; col < inputNCols; col++) {
				in[index] = input[i].get(row, col);
				
				index++;
			}}
		}
		
		return new Matrix[] { forward(in) };
	}
	
	/**
	 * This will take an input vector and feed it through the network and return the result.
	 * 
	 * It will NOT save the net value and activation of each neuron. Do not use this if learning is desired.
	 * 
	 * @param in -> Input data
	 * @return The resulting vector
	 */
	public Matrix calculate(double[] in) {
		Matrix last = new Matrix(in.length, 1, in);
		
		for(int i = 1; i < layerSizes.length; i++)
			last = weights[i - 1].multiply(last).add(biases[i - 1]).mForEach(activationFunction.getFunction());
		
		return last;
	}
	
	@Override
	public Matrix[] back(Matrix[] labels) {
		Matrix error = nesterovBackpropogate(labels[0]);
		
		Matrix[] kernals = new Matrix[layerSizes[1]];
		int index = 0;
		
		for(int i = 0; i < kernals.length; i++) {
			kernals[i] = new Matrix(inputNRows, inputNCols);
			
			for(int row = 0; row < kernals[0].getNRows(); row++) {
			for(int col = 0; col < kernals[0].getNCols(); col++) {
				kernals[i].set(row, col, weights[0].get(i, index));
				
				index++;
			}}
		}
		
		Matrix toRet = ConvolutionalLayer.fullConvolute(error, kernals[0].flip(), 1);
		
//		System.out.println(toRet);
		
		return new Matrix[] { toRet };
	}
	
	/**
	 * This is the classic backpropogation method. It doesn't do all that well....
	 * 
	 * @param labels -> Answers to the input (last feed forward call)
	 */
	public Matrix vanillaBackpropogate(double[] labels) { return vanillaBackpropogate(new Matrix(labels.length, 1, labels)); }
	public Matrix vanillaBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].forEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].forEach(activationFunction.getDerivativeFunction()));
			}
			
			weights[weightLayer] = weights[weightLayer].subtract(weights[weightLayer].scale(learningRate)
									.hadamardProduct(error.multiply(activations[weightLayer].transpose())));
			
			biases[weightLayer] = biases[weightLayer].subtract(biases[weightLayer].scale(learningRate)
									.hadamardProduct(error));
		}
		
		return error;
	}
	
	/**
	 * The Nesterov function makes use of a velocity term to give the learning process a sense of momentum. 
	 * 
	 * This works much better than the classic algorithm.
	 * 
	 * @param labels -> Answers to the input (last feed forward call)
	 */
	public Matrix nesterovBackpropogate(double[] labels) { return nesterovBackpropogate(new Matrix(labels.length, 1, labels)); }
	public Matrix nesterovBackpropogate(Matrix labels) {
		Matrix error = null;
		
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.mHadamardProduct(nets[weightLayer].mForEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.mHadamardProduct(nets[weightLayer + 1].mForEach(activationFunction.getDerivativeFunction()));
			}

//			System.out.println(error);
			
			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
			
			weightVelocities[weightLayer].mScale(velocityCoeficcient);
			biasVelocities[weightLayer].mScale(velocityCoeficcient);
			
			weights[weightLayer].mSubtract(weightVelocities[weightLayer])
										.mAdd(weightVelocities[weightLayer].mSubtract(weightGradient.mScale(learningRate)).scale(1.0 + velocityCoeficcient));
			biases[weightLayer].mSubtract(biasVelocities[weightLayer])
										.mAdd(biasVelocities[weightLayer].mSubtract(error.scale(learningRate)).scale(1.0 + velocityCoeficcient));
		}
		
		return error;
	}
	
	public void batchedNesterovBackpropogate(double[] labels) { batchedNesterovBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void batchedNesterovBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.mHadamardProduct(nets[weightLayer].mForEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.mHadamardProduct(nets[weightLayer + 1].mForEach(activationFunction.getDerivativeFunction()));
			}

			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
			runningAverageWeightGradients[weightLayer].mAdd(weightGradient);
			runningAverageBiasGradients[weightLayer].mAdd(error); 
		}
		
		batchCount++;
		
		if(batchCount == batchSize) {
			for(int i = 0; i < weights.length; i++) {
				Matrix previousWeightVelocity = new Matrix(weightVelocities[i]);
				Matrix previousBiasVelocity = new Matrix(biasVelocities[i]);
				
				weightVelocities[i] = weightVelocities[i].scale(velocityCoeficcient)
						.subtract(runningAverageWeightGradients[i].scale(learningRate / batchCount));
				biasVelocities[i] = biasVelocities[i].scale(velocityCoeficcient)
						.subtract(runningAverageBiasGradients[i].scale(learningRate / batchCount));
				
				weights[i] = weights[i].subtract(previousWeightVelocity.scale(velocityCoeficcient))
						.add(weightVelocities[i].scale(1.0 + velocityCoeficcient));
				biases[i] = biases[i].subtract(previousBiasVelocity.scale(velocityCoeficcient))
						.add(biasVelocities[i].scale(1.0 + velocityCoeficcient));
				
				weightVelocities[i].mScale(velocityCoeficcient);
				biasVelocities[i].mScale(velocityCoeficcient);
				
				weights[i].mSubtract(weightVelocities[i])
											.mAdd(weightVelocities[i].mSubtract(runningAverageWeightGradients[i].mScale(learningRate / batchCount)).scale(1.0 + velocityCoeficcient));
				biases[i].mSubtract(biasVelocities[i])
											.mAdd(biasVelocities[i].mSubtract(runningAverageBiasGradients[i].mScale(learningRate / batchCount)).scale(1.0 + velocityCoeficcient));
				
				
				runningAverageWeightGradients[i].clear();
				runningAverageBiasGradients[i].clear();
			}
			
			batchCount = 0;
		}
	}
	
	public Matrix[] getActivations() { return activations; }
	public Matrix getInput() { return activations[0]; }
	public Matrix getOutput() { return activations[activations.length - 1]; } 
	
	public Matrix[] getWeights() { return weights; }
	public Matrix[] getBiases() { return biases; }
	
	public int[] getLayerSizes() { return layerSizes; }
}
