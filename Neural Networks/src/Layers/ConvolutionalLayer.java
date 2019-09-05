package Layers;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ReluFunction;

public class ConvolutionalLayer extends Layer {
	private static ReluFunction relu = new ReluFunction();

	private Matrix[] input, output;
	
	private Matrix[][] kernals;
	private double[] biases;
	
	private int padding, stride;
	private int kernalCount, kernalSize;
	
	/**
	 * Sources:
	 * 	https://www.jefkine.com/general/2016/09/05/backpropagation-in-convolutional-neural-networks/
	 *  http://cs231n.github.io/convolutional-networks/
	 *  http://yann.lecun.com/exdb/publis/pdf/lecun-98b.pdf
	 *  https://becominghuman.ai/only-numpy-implementing-convolutional-neural-network-using-numpy-deriving-forward-feed-and-back-458a5250d6e4
	 */
	public ConvolutionalLayer(NeuralNetwork network, int layer, int padding, int stride, int kernalCount, int kernalSize, int kernalDimensionalExtent) {
		super(network, layer);
		
		this.padding = padding;
		this.stride = stride;
		this.kernalCount = kernalCount;
		this.kernalSize = kernalSize;
		
		kernals = new Matrix[kernalCount][kernalDimensionalExtent];
		
		for(int i = 0; i < kernals.length; i++) {
		for(int j = 0; j < kernals[0].length; j++) {
			kernals[i][j] = Matrix.random(kernalSize, kernalSize, -1, 1);
		}}
		
		biases = new double[kernalCount];
		
		for(int i = 0; i < biases.length; i++) 
			biases[i] = 0;//(Math.random() * 2.0) - 1.0;
	}

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
		
		outputMatrixCount = kernalCount;
		
//		System.out.println(inputNCols);
		
		double outputWidth = ((inputNCols - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
		double outputHeight = ((inputNRows - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
		
		if((int) outputWidth != outputWidth) {
			System.err.println("Invalid Conv Width!");
			System.exit(-1);
		}
		
		if((int) outputHeight != outputHeight) {
			System.err.println("Invalid Conv Height!");
			System.exit(-1);
		}
		
		outputNCols = (int) outputWidth;
		outputNRows = (int) outputHeight;
		
		output = new Matrix[kernalCount];
		for(int i = 0; i < output.length; i++) 
			output[i] = new Matrix(outputNRows, outputNCols);
	}
	
	protected Matrix[] forward(Matrix[] input) {
		if(padding != 0) {
			for(int i = 0; i < input.length; i++) {
				input[i] = input[i].pad(padding);
			}
		}

		this.input = input;
		
		for(int y = 0; y <= input[0].getNRows() - kernalSize; y += stride) {
		for(int x = 0; x <= input[0].getNCols() - kernalSize; x += stride) {
			for(int kernalSet = 0; kernalSet < kernals.length; kernalSet++) {
				double sum = 0.0;
				for(int kernal = 0; kernal < kernals[kernalSet].length; kernal++) 
					sum += input[kernal].elementWiseProduct(kernals[kernalSet][kernal], x, y);
				
				sum += biases[kernalSet];
				
				output[kernalSet].set(y / stride, x / stride, relu.getFunction().apply(sum));
			}
		}}
		
		return output;
	}
	
	protected Matrix[] back(Matrix[] error) {
		for(int i = 0; i < error.length; i++) {
			Matrix weightGradient = convolute(input[i], error[i]);
			
			for(int kernalSet = 0; kernalSet < kernals.length; kernalSet++) {
			for(int kernal = 0; kernal < kernals[kernalSet].length; kernal++) {
				kernals[kernalSet][kernal].mAdd(weightGradient.mScale(0.001));
			}}
		}
		
		return null;
	}
	
	private Matrix convolute(Matrix data, Matrix filter) {
		if(padding != 0)
			data = data.pad(padding);
		
		double outputWidth = ((data.getNCols() - filter.getNCols() + (2.0 * padding)) / ((double) stride)) + 1.0;
		double outputHeight = ((data.getNRows() - filter.getNRows() + (2.0 * padding)) / ((double) stride)) + 1.0;
		
		if((int) outputWidth != outputWidth) {
			System.err.println("Invalid Conv Width!");
			return null;
		}
		
		if((int) outputHeight != outputHeight) {
			System.err.println("Invalid Conv Height!");
			return null;
		}
		
		Matrix output = new Matrix((int) outputHeight, (int) outputWidth);
		
		for(int y = 0; y <= data.getNRows() - filter.getNRows(); y += stride) {
		for(int x = 0; x <= data.getNCols() - filter.getNCols(); x += stride) {
			output.set(y / stride, x / stride, data.elementWiseProduct(filter, x, y));
		}}
		
		return output;
	}
	
	public Matrix[][] getKarnals() { return kernals; }
	
//	public static void main(String[] args) {
//		NeuralNetwork network = new NeuralNetwork();
//		
//		ConvolutionalLayer convolutional = new ConvolutionalLayer(network, 0, 0, 2, 1, 3, 1);
//		
//		network.layers = new Layer[1];
//		network.layers[0] = convolutional;
//		
//		convolutional.kernals = new Matrix[][] {
//			{ new Matrix(new double[][] {
//				{-1, 1, 1},
//				{-1, 0, -1},
//				{1, 1, 1}
//				}),
//			 
////			  new Matrix(new double[][] {
////				  {0, 0, -1},
////				  {1, 1, 1},
////				  {1, -1, 1}
////			  }),
////			  
////			  new Matrix(new double[][] {
////				  {0,-1,1},
////				  {0,1,-1},
////				  {0,-1,-1}
////			  })
//			}
//		};
//		
//		Matrix[] out = network.feedForward(new Matrix[] {
//			new Matrix(new double[][] {
//				{0, 0, 0, 0, 0, 0, 0},
//				{0, 0, 2, 2, 0, 0, 0},
//				{0, 2, 2, 1, 0, 2, 0},
//				{0, 1, 0, 1, 1, 0, 0},
//				{0, 0, 1, 0, 2, 1, 0},
//				{0, 2, 1, 0, 1, 0, 0},
//				{0, 0, 0, 0, 0, 0, 0}
//			}),
//			
////			new Matrix(new double[][] {
////				{0, 0, 0, 0, 0, 0, 0},
////				{0, 2, 2, 2, 2, 1, 0},
////				{0, 2, 1, 0, 0, 0, 0},
////				{0, 2, 0, 2, 1, 0, 0},
////				{0, 0, 2, 0, 1, 2, 0},
////				{0, 2, 1, 1, 1, 1, 0},
////				{0, 0, 0, 0, 0, 0, 0}
////			}),
////			
////			new Matrix(new double[][] {
////				{0, 0, 0, 0, 0, 0, 0},
////				{0, 2, 0, 2, 2, 2, 0},
////				{0, 0, 0, 2, 0, 1, 0},
////				{0, 2, 0, 0, 1, 1, 0},
////				{0, 1, 2, 1, 2, 1, 0},
////				{0, 2, 2, 2, 2, 0, 0},
////				{0, 0, 0, 0, 0, 0, 0}
////			})
//		});
//	}
}
