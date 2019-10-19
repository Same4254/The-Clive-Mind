package NeuralNetworks.LayeredNeuralNetwork;

import NeuralNetworks.FullyConnected;
import Utilities.Matrix;

public class LayeredNeuralNetwork {
	public Layer[] layers;

	private int inputMatrixCount, inputNRows, inputNCols;
	private int outputMatrixCount, outputNRows, outputNCols;
	
	private Matrix[] output;
	
	/**
	 * 					TODO
	 * Structure Changes:
	 * 	    Fully Connected needs to be a standalone class
	 * 	    Hyper Parameters need to be more configurable -> they are just initialized in declaration of field
	 *      Learning method needs to be more interchangeable and selectable at a higher level ie by the call to the network
	 *      Creation of layers needs to be easier -> Builder? Currently is a struggle of recalling to change 3 paramaters
	 *      Fully Connected layer should only take in the hidden layer sizes, for we know the input size to in and the output size
	 * 		Saving to a folder
	 * 
	 * Visualization (Low priority)
	 * 		Fully Connected Renderer needs to be fixed
	 * 		Layered Network visual? See the intermediate pictures of what the kernals digest
	 * 	
	 * Optimizations:
	 * 		Matrix fake padding -> There's no need to fill up memory with 0 values.... make the get method of 
	 * 			matrix calculate if the request is in the padding zone, if so return 0 
	 * 	    Convolutional Layer needs heavy amounts of clean up
	 * 	    	Possible better algorithm that does the same calculation faster?
	 * 	    Matricies need an operation version where the output is dumped into a 3rd matrix (taken as a parameter)
	 *      Fully Connected needs optimization: intermediated matricies can be preallocated and reused
	 * 	    Activation Layer needs to take advantage of dumping the ouput of a matrix operation into an already existing 3rd matrix
	 * 	    	a, b, c = new Matricies
	 * 	    	a.add(b, c) where the values of c correspond to a + b
	 * 	    	the benefit really comes into play for matrix multiplication, where mutability just isn't possible. 
	 * 	    		This is the next best thing 
	 */
	public LayeredNeuralNetwork(int inputMatrixCount, int inputNRows, int inputNCols, int outputNRows, int outputNCols) {
		this.inputMatrixCount = inputMatrixCount;
		this.inputNRows = inputNRows;
		this.inputNCols = inputNCols;
		this.outputMatrixCount = 1;
		this.outputNRows = outputNRows;
		this.outputNCols = outputNCols;
	}

	public Matrix[] feedForward(Matrix[] input) {
		this.output = layers[0].feedFoward(input);
		
		return output;
	}
	
//	public void backpropogate(Matrix[] labels) {
//		
//	}
	
	public void backpropogate(Matrix[] labels) {
		Matrix[] error = new Matrix[labels.length];
		for(int i = 0; i < error.length; i++) 
			error[i] = output[i].subtract(labels[i]);
		
		layers[layers.length - 1].backpropogate(error);
	}
	
	public int getLayerCount() { return layers.length; }
	public Layer[] getLayers() { return layers; }

	public int getInputMatrixCount() { return inputMatrixCount; }
	public int getInputNRows() { return inputNRows; }
	public int getInputNCols() { return inputNCols; }
	public int getOutputMatrixCount() { return outputMatrixCount; }
	public int getOutputNRows() { return outputNRows;}
	public int getOutputNCols() { return outputNCols; }
	
//	public static void main(String[] args) {
//		LayeredNeuralNetwork network = new LayeredNeuralNetwork(1, 4, 4, 2, 1);
//		
//		ConvolutionalLayer conv = new ConvolutionalLayer(network, 0, 0, 2, 1, 2, 1);
//		FullyConnected full = new FullyConnected(network, 1, new int[] { 4, 2 });
//		
//		network.layers = new Layer[2];
//		network.layers[0] = conv;
//		network.layers[1] = full;
//		
//		conv.initialize();
//		full.initialize();
//		
//		Matrix[] output = network.feedForward(new Matrix[] {
//			new Matrix(new double[][] {
//				{.1, .2, .3, .4},
//				{.5, .6, .7, .8},
//				{.9, .01, .02, .03},
//				{.04, .05, .06, .07}
//			})
//		});
//		
////		System.out.println(output[0]);
//		
//		network.backpropogate(new Matrix(new double[][] {
//			{.9},
//			{.1}
//		}));
//	}
}
