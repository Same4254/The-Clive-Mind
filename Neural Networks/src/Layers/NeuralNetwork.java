package Layers;

import Utilities.Matrix;

public class NeuralNetwork {
	public Layer[] layers;

	private int inputMatrixCount, inputNRows, inputNCols;
	private int outputMatrixCount, outputNRows, outputNCols;
	
	public NeuralNetwork(int inputMatrixCount, int inputNRows, int inputNCols, int outputNRows, int outputNCols) {
		this.inputMatrixCount = inputMatrixCount;
		this.inputNRows = inputNRows;
		this.inputNCols = inputNCols;
		this.outputMatrixCount = 1;
		this.outputNRows = outputNRows;
		this.outputNCols = outputNCols;
	}

	public Matrix[] feedForward(Matrix[] input) {
		return layers[0].feedFoward(input);
	}
	
	public void backPropogate(Matrix labels) {
		layers[layers.length - 1].backpropogate(new Matrix[] { labels });
	}
	
	public int getLayerCount() { return layers.length; }
	public Layer[] getLayers() { return layers; }

	public int getInputMatrixCount() { return inputMatrixCount; }
	public int getInputNRows() { return inputNRows; }
	public int getInputNCols() { return inputNCols; }
	public int getOutputMatrixCount() { return outputMatrixCount; }
	public int getOutputNRows() { return outputNRows;}
	public int getOutputNCols() { return outputNCols; }
	
	public static void main(String[] args) {
		NeuralNetwork network = new NeuralNetwork(1, 4, 4, 2, 1);
		
		ConvolutionalLayer conv = new ConvolutionalLayer(network, 0, 0, 2, 1, 2, 1);
		FullyConnected full = new FullyConnected(network, 1, new int[] { 4, 2 });
		
		network.layers = new Layer[2];
		network.layers[0] = conv;
		network.layers[1] = full;
		
		conv.initialize();
		full.initialize();
		
		Matrix[] output = network.feedForward(new Matrix[] {
			new Matrix(new double[][] {
				{.1, .2, .3, .4},
				{.5, .6, .7, .8},
				{.9, .01, .02, .03},
				{.04, .05, .06, .07}
			})
		});
		
//		System.out.println(output[0]);
		
		network.backPropogate(new Matrix(new double[][] {
			{.9},
			{.1}
		}));
	}
}
