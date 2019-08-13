package Layers;

import Utilities.Matrix;

public class ConvolutionalLayer {
	private Matrix[][] kernals;
	private double[] biases;
	
	private int padding, stride;
	private int kernalCount, kernalSize;
	
	public ConvolutionalLayer(int padding, int stride, int kernalCount, int kernalSize, int kernalDimensionalExtent) {
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
			biases[i] = (Math.random() * 2.0) - 1.0;
	}

	public Matrix[] calculate(Matrix[] input) {
		if(padding != 0) {
			for(int i = 0; i < input.length; i++) {
				input[i] = input[i].pad(padding);
			}
		}
		
		double outputWidth = ((input[0].getNCols() - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
		double outputHeight = ((input[0].getNRows() - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
		
		if((int) outputWidth != outputWidth) {
			System.err.println("Invalid Conv Width!");
			return null;
		}
		
		if((int) outputHeight != outputHeight) {
			System.err.println("Invalid Conv Height!");
			return null;
		}
		
		Matrix[] output = new Matrix[kernalCount];
		for(int i = 0; i < output.length; i++) {
			output[i] = new Matrix((int) outputHeight, (int) outputWidth);
		}

		for(int y = 0; y <= input[0].getNRows() - kernalSize; y += stride) {
		for(int x = 0; x <= input[0].getNCols() - kernalSize; x += stride) {
			for(int kernalSet = 0; kernalSet < kernals.length; kernalSet++) {
				double sum = 0.0;
				for(int kernal = 0; kernal < kernals[kernalSet].length; kernal++) 
					sum += input[kernal].elementWiseProduct(kernals[kernalSet][kernal], x, y);
				
				sum += biases[kernalSet];
				
				output[kernalSet].set(y / stride, x / stride, sum);
			}
		}}
		
		return output;
	}
	
	public Matrix[][] getKarnals() { return kernals; }
	
	public static void main(String[] args) {
		ConvolutionalLayer convolutional = new ConvolutionalLayer(0, 2, 1, 3, 3);
		convolutional.kernals = new Matrix[][] {
			{ new Matrix(new double[][] {
				{-1, 1, 1},
				{-1, 0, -1},
				{1, 1, 1}
				}),
			 
			  new Matrix(new double[][] {
				  {0, 0, -1},
				  {1, 1, 1},
				  {1, -1, 1}
			  }),
			  
			  new Matrix(new double[][] {
				  {0,-1,1},
				  {0,1,-1},
				  {0,-1,-1}
			  })
			}
		};
		
		Matrix[] out = convolutional.calculate(new Matrix[] {
			new Matrix(new double[][] {
				{0, 0, 0, 0, 0, 0, 0},
				{0, 0, 2, 2, 0, 0, 0},
				{0, 2, 2, 1, 0, 2, 0},
				{0, 1, 0, 1, 1, 0, 0},
				{0, 0, 1, 0, 2, 1, 0},
				{0, 2, 1, 0, 1, 0, 0},
				{0, 0, 0, 0, 0, 0, 0}
			}),
			
			new Matrix(new double[][] {
				{0, 0, 0, 0, 0, 0, 0},
				{0, 2, 2, 2, 2, 1, 0},
				{0, 2, 1, 0, 0, 0, 0},
				{0, 2, 0, 2, 1, 0, 0},
				{0, 0, 2, 0, 1, 2, 0},
				{0, 2, 1, 1, 1, 1, 0},
				{0, 0, 0, 0, 0, 0, 0}
			}),
			
			new Matrix(new double[][] {
				{0, 0, 0, 0, 0, 0, 0},
				{0, 2, 0, 2, 2, 2, 0},
				{0, 0, 0, 2, 0, 1, 0},
				{0, 2, 0, 0, 1, 1, 0},
				{0, 1, 2, 1, 2, 1, 0},
				{0, 2, 2, 2, 2, 0, 0},
				{0, 0, 0, 0, 0, 0, 0}
			})
		});
		
		for(Matrix m : out) {
			System.out.println(m);
			System.out.println("----------");
		}
	}
}
