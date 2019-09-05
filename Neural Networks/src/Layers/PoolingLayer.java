package Layers;

import Utilities.Matrix;
import Utilities.Pair;

public class PoolingLayer extends Layer {
	private Matrix[] gradients;
	private Matrix[] output;
	
	private int size, stride;
	
	public PoolingLayer(NeuralNetwork network, int layer, int size, int stride) {
		super(network, layer);
		
		this.size = size;
		this.stride = stride;
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
		
		double outputCols = ((inputNCols - size) / ((double) stride)) + 1.0;
		double outputRows = ((inputNRows - size) / ((double) stride)) + 1.0;
		
		if((int) outputCols != outputCols) {
			System.err.println("Invalid Pooling Width!");
			System.exit(-1);
		}
		
		if((int) outputRows != outputRows) {
			System.err.println("Invalid Pooling Height!");
			System.exit(-1);
		}
		
		this.outputNCols = (int) outputCols;
		this.outputNRows = (int) outputRows;
		this.outputMatrixCount = inputMatrixCount;
		
		output = new Matrix[outputMatrixCount];
		for(int i = 0; i < output.length; i++)
			output[i] = new Matrix(outputNRows, outputNCols);
	}
	
	@Override
	public Matrix[] forward(Matrix[] input) {
		for(int i = 0; i < input.length; i++)
			output[i].clear();
		
		if(gradients == null) {
			gradients = new Matrix[input.length];
			for(int i = 0; i < input.length; i++)
				gradients[i] = new Matrix(input[0].getNRows(), input[0].getNCols());
		} else {
			for(int i = 0; i < gradients.length; i++) {
				gradients[i].clear();
			}
		}
		
		for(int i = 0; i < output.length; i++) {
			for(int row = 0; row <= input[0].getNRows() - size; row += stride) {
			for(int col = 0; col <= input[0].getNCols() - size; col += stride) {
				Pair<int[], Double> pair = input[i].mostPositive(row, col, row + size - 1, col + size - 1);
				int[] location = pair.getKey();
				double value = pair.getValue();
				
				gradients[i].set(location[0], location[1], value);
				
				output[i].set(row / stride, col / stride, value);
			}}
		}
		
		return output;
	}
	
	@Override
	protected Matrix[] back(Matrix[] error) {
		return gradients;
	}
}
