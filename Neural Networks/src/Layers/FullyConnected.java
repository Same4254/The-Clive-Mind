package Layers;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ActivationFunction;
import Utilities.ActivationFunctions.SigmoidFunction;

public class FullyConnected {
	private Matrix[] weights, biases, activations, nets;
	private Matrix[] weightVelocities, biasVelocities;
	
	private Matrix[] runningAverageWeightGradients, runningAverageBiasGradients;
	
	private int batchCount, batchSize = 10;
	
	private int[] layerSizes;
	
	private ActivationFunction activationFunction;
	private double learningRate = 0.001;
	private double velocityCoeficcient = 0.9;
	
	public FullyConnected(int[] layerSizes) {
		this.layerSizes = layerSizes;
		
		weights = new Matrix[layerSizes.length - 1];
		biases = new Matrix[layerSizes.length - 1];
		
		weightVelocities = new Matrix[layerSizes.length - 1];
		biasVelocities = new Matrix[layerSizes.length - 1];
		
		runningAverageWeightGradients = new Matrix[layerSizes.length - 1];
		runningAverageBiasGradients = new Matrix[layerSizes.length - 1];
		
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
		}
	}
	
	public Matrix calculate(double[] in) {
		activations[0] = new Matrix(in.length, 1, in);
		nets[0] = new Matrix(in.length, 1, in);
		
		for(int i = 1; i < layerSizes.length; i++) {
			Matrix net = weights[i - 1].multiply(activations[i - 1]).add(biases[i - 1]);
			nets[i] = new Matrix(net);
			activations[i] = net.forEach(activationFunction.getFunction());
		}
		
		return activations[activations.length - 1];
	}
	
	public void vanillaBackpropogate(double[] labels) { vanillaBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void vanillaBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].immutableForEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].immutableForEach(activationFunction.getDerivativeFunction()));
			}
			
			weights[weightLayer] = weights[weightLayer].subtract(weights[weightLayer].immutableScale(learningRate)
									.hadamardProduct(error.multiply(activations[weightLayer].transpose())));
			
			biases[weightLayer] = biases[weightLayer].subtract(biases[weightLayer].immutableScale(learningRate)
									.hadamardProduct(error));
		}
	}
	
	public void momentumBackpropogate(double[] labels) { momentumBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void momentumBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].immutableForEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].immutableForEach(activationFunction.getDerivativeFunction()));
			}
			
			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
			weightVelocities[weightLayer] = weightVelocities[weightLayer].immutableScale(velocityCoeficcient).subtract(weightGradient.immutableScale(learningRate));
			weights[weightLayer] = weights[weightLayer].add(weightVelocities[weightLayer]);

			biasVelocities[weightLayer] = biasVelocities[weightLayer].immutableScale(velocityCoeficcient).subtract(error.immutableScale(learningRate));
			biases[weightLayer] = biases[weightLayer].add(biasVelocities[weightLayer]);
		}
	}
	
	public void nesterovBackpropogate(double[] labels) { nesterovBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void nesterovBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].immutableForEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].immutableForEach(activationFunction.getDerivativeFunction()));
			}

			Matrix weightGradient = error.multiply(activations[weightLayer].transpose());
			
			Matrix previousWeightVelocity = new Matrix(weightVelocities[weightLayer]);
			Matrix previousBiasVelocity = new Matrix(biasVelocities[weightLayer]);
			
			weightVelocities[weightLayer] = weightVelocities[weightLayer].immutableScale(velocityCoeficcient)
												.subtract(weightGradient.immutableScale(learningRate));
			biasVelocities[weightLayer] = biasVelocities[weightLayer].immutableScale(velocityCoeficcient)
												.subtract(error.immutableScale(learningRate));
			
			weights[weightLayer] = weights[weightLayer].subtract(previousWeightVelocity.immutableScale(velocityCoeficcient))
										.add(weightVelocities[weightLayer].immutableScale(1.0 + velocityCoeficcient));
			biases[weightLayer] = biases[weightLayer].subtract(previousBiasVelocity.immutableScale(velocityCoeficcient))
										.add(biasVelocities[weightLayer].immutableScale(1.0 + velocityCoeficcient));
		}
	}
	
	public void batchedNesterovBackpropogate(double[] labels) { batchedNesterovBackpropogate(new Matrix(labels.length, 1, labels)); }
	public void batchedNesterovBackpropogate(Matrix labels) {
		Matrix error = null;
		for(int weightLayer = weights.length - 1; weightLayer >= 0; weightLayer--) {
			if(weightLayer == weights.length - 1) {
				error = activations[activations.length - 1].subtract(labels)
							.hadamardProduct(nets[weightLayer].immutableForEach(activationFunction.getDerivativeFunction()));
			} else {
				error = weights[weightLayer + 1].transpose().multiply(error)
							.hadamardProduct(nets[weightLayer + 1].immutableForEach(activationFunction.getDerivativeFunction()));
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
				
				weightVelocities[i] = weightVelocities[i].immutableScale(velocityCoeficcient)
						.subtract(runningAverageWeightGradients[i].immutableScale(learningRate / batchCount));
				biasVelocities[i] = biasVelocities[i].immutableScale(velocityCoeficcient)
						.subtract(runningAverageBiasGradients[i].immutableScale(learningRate / batchCount));
				
				weights[i] = weights[i].subtract(previousWeightVelocity.immutableScale(velocityCoeficcient))
						.add(weightVelocities[i].immutableScale(1.0 + velocityCoeficcient));
				biases[i] = biases[i].subtract(previousBiasVelocity.immutableScale(velocityCoeficcient))
						.add(biasVelocities[i].immutableScale(1.0 + velocityCoeficcient));
				
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
	
	public static void main(String[] args) {
		FullyConnected network = new FullyConnected(new int[] {2, 50, 2});

		double[][][] dataset = {
			{ {0, .5}, {.4, .3} },
			{ {.1, .7}, {.9, .1} },
			{ {.6, .8}, {.3, .2} },
			{ {.5, .3}, {.108, .904} },
			{ {.8, .6}, {.87, .46} },
			{ {.9, .05}, {.12, .79} },
			{ {.705, .806}, {.34, .68} },
		};
		
		System.out.println(network.calculate(dataset[0][0]));
		System.out.println(network.calculate(dataset[1][0]));
		
		for(int i = 0; i < 100000; i++) {
//			System.out.println("-----");
			
			network.calculate(dataset[0][0]);
			network.nesterovBackpropogate(dataset[0][1]);
			
			network.calculate(dataset[1][0]);
			network.nesterovBackpropogate(dataset[1][1]);
			
//			System.out.println(network.calculate(dataset[2][0]));
//			network.backpropogate(dataset[2][1]);
		}
		
		System.out.println(network.calculate(dataset[0][0]));
		System.out.println(network.calculate(dataset[1][0]));
	}
}
