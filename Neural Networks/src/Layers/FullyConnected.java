package Layers;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Scanner;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ActivationFunction;
import Utilities.ActivationFunctions.SigmoidFunction;

public class FullyConnected {
	private Matrix[] weights, biases, activations, nets;
	private Matrix[] weightVelocities, biasVelocities;
	
	private Matrix[] runningAverageWeightGradients, runningAverageBiasGradients;
	
	private int batchCount, batchSize = 20;
	
	private int[] layerSizes;
	
	private ActivationFunction activationFunction;
	private double learningRate = 0.001;
	private double velocityCoeficcient = 0.9;
	
	private Matrix[] previousWeightGradients;
	private Matrix[] previousDeltas;
	private Matrix[] previousWeightDeltas;
	
	private double etaPlus = 1.2, etaMinus = 0.5, deltaMin = 1E-6, deltaMax = 50.0;
	
	public FullyConnected(int[] layerSizes) {
		this.layerSizes = layerSizes;
		
		weights = new Matrix[layerSizes.length - 1];
		biases = new Matrix[layerSizes.length - 1];
		
		//Momentum
		weightVelocities = new Matrix[layerSizes.length - 1];
		biasVelocities = new Matrix[layerSizes.length - 1];
		
		runningAverageWeightGradients = new Matrix[layerSizes.length - 1];
		runningAverageBiasGradients = new Matrix[layerSizes.length - 1];
		
		previousWeightGradients = new Matrix[layerSizes.length - 1];
		previousDeltas = new Matrix[layerSizes.length - 1];
		previousWeightDeltas = new Matrix[layerSizes.length - 1];
		
		nets = new Matrix[layerSizes.length];
		activations = new Matrix[layerSizes.length];
		
		activationFunction = new SigmoidFunction();
		
		for(int layer = 1; layer < layerSizes.length; layer++) {
			weights[layer - 1] = Matrix.random(layerSizes[layer], layerSizes[layer - 1], -1, 1);
			weightVelocities[layer - 1] = new Matrix(layerSizes[layer], layerSizes[layer - 1]);
			runningAverageWeightGradients[layer - 1] = new Matrix(layerSizes[layer], layerSizes[layer - 1]);
			
			biases[layer - 1] = Matrix.random(layerSizes[layer], 1, -1, 1);
			biasVelocities[layer - 1] = new Matrix(layerSizes[layer], 1);
			runningAverageBiasGradients[layer - 1] = new Matrix(layerSizes[layer], 1);
			
			previousWeightGradients[layer - 1] = new Matrix(layerSizes[layer], layerSizes[layer - 1]);
			previousDeltas[layer - 1] = new Matrix(layerSizes[layer], layerSizes[layer - 1], 0.1);
			previousWeightDeltas[layer - 1] = new Matrix(layerSizes[layer], layerSizes[layer - 1]);
		}
	}
	
	public FullyConnected(File file) {
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
		
		for(int layer = 1; layer < layerSizes.length; layer++) {
			runningAverageWeightGradients[layer - 1] = new Matrix(layerSizes[layer], layerSizes[layer - 1]);
			runningAverageBiasGradients[layer - 1] = new Matrix(layerSizes[layer], 1);
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
	
	public Matrix feedForward(double[] in) {
		activations[0] = new Matrix(in.length, 1, in);
		nets[0] = new Matrix(in.length, 1, in);
		
		for(int i = 1; i < layerSizes.length; i++) {
//			Matrix net = weights[i - 1].multiply(activations[i - 1]).mAdd(biases[i - 1]);
//			nets[i] = new Matrix(net);
//			activations[i] = net.mForEach(activationFunction.getFunction());
			
			nets[i] = weights[i - 1].multiply(activations[i - 1]).mAdd(biases[i - 1]);
			activations[i] = nets[i].forEach(activationFunction.getFunction());
		}
		
		return activations[activations.length - 1];
	}
	
	public Matrix calculate(double[] in) {
		Matrix last = new Matrix(in.length, 1, in);
		
		for(int i = 1; i < layerSizes.length; i++)
			last = weights[i - 1].multiply(last).add(biases[i - 1]).mForEach(activationFunction.getFunction());
		
		return last;
	}
	
	public void vanillaBackpropogate(double[] labels) { vanillaBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void vanillaBackpropogate(Matrix labels) {
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
	}
	
	public void nesterovBackpropogate(double[] labels) { nesterovBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void nesterovBackpropogate(Matrix labels) {
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
			
			weightVelocities[weightLayer].mScale(velocityCoeficcient);
			biasVelocities[weightLayer].mScale(velocityCoeficcient);
			
			weights[weightLayer].mSubtract(weightVelocities[weightLayer])
										.mAdd(weightVelocities[weightLayer].mSubtract(weightGradient.mScale(learningRate)).scale(1.0 + velocityCoeficcient));
			biases[weightLayer].mSubtract(biasVelocities[weightLayer])
										.mAdd(biasVelocities[weightLayer].mSubtract(error.scale(learningRate)).scale(1.0 + velocityCoeficcient));
		}
	}
	
	//72 Seconds
//	Matrix error = null;
//	for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
//		if(weightLayer == weights.length - 1) {
//			error = activations[activations.length - 1].subtract(labels)
//						.mHadamardProduct(nets[weightLayer].mForEach(activationFunction.getDerivativeFunction()));
//		} else {
//			error = weights[weightLayer + 1].transpose().multiply(error)
//						.mHadamardProduct(nets[weightLayer + 1].mForEach(activationFunction.getDerivativeFunction()));
//		}
//
//		Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
//		
//		weightVelocities[weightLayer].mScale(velocityCoeficcient);
//		biasVelocities[weightLayer].mScale(velocityCoeficcient);
//		
//		weights[weightLayer].mSubtract(weightVelocities[weightLayer])
//									.mAdd(weightVelocities[weightLayer].mSubtract(weightGradient.mScale(learningRate)).scale(1.0 + velocityCoeficcient));
//		biases[weightLayer].mSubtract(biasVelocities[weightLayer])
//									.mAdd(biasVelocities[weightLayer].mSubtract(error.scale(learningRate)).scale(1.0 + velocityCoeficcient));
//	}
	
	//86 Seconds
//	Matrix error = null;
//	for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
//		if(weightLayer == weights.length - 1) {
//			error = activations[activations.length - 1].subtract(labels)
//						.mHadamardProduct(nets[weightLayer].forEach(activationFunction.getDerivativeFunction()));
//		} else {
//			error = weights[weightLayer + 1].transpose().multiply(error)
//						.mHadamardProduct(nets[weightLayer + 1].forEach(activationFunction.getDerivativeFunction()));
//		}
//
//		Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
//		
//		weightVelocities[weightLayer].mScale(velocityCoeficcient);
//		biasVelocities[weightLayer].mScale(velocityCoeficcient);
//		
//		weights[weightLayer].mSubtract(weightVelocities[weightLayer])
//									.mAdd(weightVelocities[weightLayer].mSubtract(weightGradient.mScale(learningRate)).scale(1.0 + velocityCoeficcient));
//		biases[weightLayer].mSubtract(biasVelocities[weightLayer])
//									.mAdd(biasVelocities[weightLayer].mSubtract(error.scale(learningRate)).scale(1.0 + velocityCoeficcient));
//	}	
	
//	public void nesterovBackpropogate(Matrix labels) {
//		Matrix error = null;
//		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
//			if(weightLayer == weights.length - 1) {
//				error = activations[activations.length - 1].subtract(labels)
//							.hadamardProduct(nets[weightLayer].forEach(activationFunction.getDerivativeFunction()));
//			} else {
//				error = weights[weightLayer + 1].transpose().multiply(error)
//							.hadamardProduct(nets[weightLayer + 1].forEach(activationFunction.getDerivativeFunction()));
//			}
//
//			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
//			
//			Matrix previousWeightVelocity = new Matrix(weightVelocities[weightLayer]);
//			Matrix previousBiasVelocity = new Matrix(biasVelocities[weightLayer]);
//			
//			weightVelocities[weightLayer] = weightVelocities[weightLayer].scale(velocityCoeficcient)
//												.subtract(weightGradient.scale(learningRate));
//			biasVelocities[weightLayer] = biasVelocities[weightLayer].scale(velocityCoeficcient)
//												.subtract(error.scale(learningRate));
//			
//			weights[weightLayer] = weights[weightLayer].subtract(previousWeightVelocity.scale(velocityCoeficcient))
//										.add(weightVelocities[weightLayer].scale(1.0 + velocityCoeficcient));
//			biases[weightLayer] = biases[weightLayer].subtract(previousBiasVelocity.scale(velocityCoeficcient))
//										.add(biasVelocities[weightLayer].scale(1.0 + velocityCoeficcient));
//		}
//	}
	
	public void batchedNesterovBackpropogate(double[] labels) { batchedNesterovBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void batchedNesterovBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].forEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].forEach(activationFunction.getDerivativeFunction()));
			}

			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
			runningAverageWeightGradients[weightLayer] = runningAverageWeightGradients[weightLayer].add(weightGradient);
			runningAverageBiasGradients[weightLayer] = runningAverageBiasGradients[weightLayer].add(error); 
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
				
				runningAverageWeightGradients[i].clear();
				runningAverageBiasGradients[i].clear();
			}
			
			batchCount = 0;
		}
	}
	
	public void resilientBackpropogation(double[] labels) { resilientBackpropogation(new Matrix(labels.length, 1, labels)); }
	public void resilientBackpropogation(Matrix labels) {
//		System.out.println("---------------------------------------------");
		
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].forEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].forEach(activationFunction.getDerivativeFunction()));
			}

			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
			
			final int index = weightLayer;
			
			weights[index] = weights[index].forEach((row, col, value) -> {
				if(previousWeightGradients[index].get(row, col) * weightGradient.get(row, col) > 0) {
					double delta = Math.min(previousDeltas[index].get(row, col) * etaPlus, deltaMax);
					double deltaWeight = -Math.copySign(1, weightGradient.get(row, col)) * delta;
					
					previousWeightGradients[index].set(row, col, weightGradient.get(row, col));
					previousDeltas[index].set(row, col, delta);
					
//					System.out.println(deltaWeight);
					
					previousWeightDeltas[index].set(row, col, deltaWeight);
					
//					return value - deltaWeight;
					return value;
				} else if(previousWeightGradients[index].get(row, col) * weightGradient.get(row, col) < 0) {
					previousDeltas[index].set(row, col, Math.max(previousDeltas[index].get(row, col) * etaMinus, deltaMin));
					previousWeightGradients[index].set(row, col, 0.0);
					
//					if(weightGradient.get(row, col) > previousWeightGradients[index].get(row, col)) 
//						return value + previousWeightDeltas[index].get(row, col);
					return value;
				} else {
					double deltaWeight = -Math.copySign(1, weightGradient.get(row, col)) * previousDeltas[index].get(row, col);
					previousWeightGradients[index].set(row, col, weightGradient.get(row, col));
					
					previousWeightDeltas[index].set(row, col, deltaWeight);
					
					return value - deltaWeight;
//					return value;
				}
			});
		}
	}
	
	public Matrix[] getActivations() { return activations; }
	public Matrix getInput() { return activations[0]; }
	public Matrix getOutput() { return activations[activations.length - 1]; } 
	
	public Matrix[] getWeights() { return weights; }
	public Matrix[] getBiases() { return biases; }
	
	public int[] getLayerSizes() { return layerSizes; }
	
//	public static void main(String[] args) {
//		FullyConnected network = new FullyConnected(new int[] {2, 50, 2});
//		File file = new File(System.getProperty("user.dir") + "/model.txt");
//		
//		if(!file.exists())
//			try {
//				file.createNewFile();
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//		
//		System.out.println(network.calculate(new double[] {.1, .2} ));
//		
//		network.save(file);
//		
//		FullyConnected net2 = new FullyConnected(file);
//		System.out.println(net2.calculate(new double[] {.1, .2} ));
//		
//		double[][][] dataset = {
//			{ {0, .5}, {.4, .3} },
//			{ {.1, .7}, {.9, .1} },
//			{ {.6, .8}, {.3, .2} },
//			{ {.5, .3}, {.108, .904} },
//			{ {.8, .6}, {.87, .46} },
//			{ {.9, .05}, {.12, .79} },
//			{ {.705, .806}, {.34, .68} },
//		};
//		
//		System.out.println(network.feedForward(dataset[0][0]));
//		System.out.println(network.feedForward(dataset[1][0]));
//		
//		for(int i = 0; i < 100000; i++) {
////			System.out.println("-----");
//			
//			network.feedForward(dataset[0][0]);
//			network.nesterovBackpropogate(dataset[0][1]);
//			
//			network.feedForward(dataset[1][0]);
//			network.nesterovBackpropogate(dataset[1][1]);
//			
////			System.out.println(network.calculate(dataset[2][0]));
////			network.backpropogate(dataset[2][1]);
//		}
//		
//		System.out.println(network.feedForward(dataset[0][0]));
//		System.out.println(network.feedForward(dataset[1][0]));
//	}
}
