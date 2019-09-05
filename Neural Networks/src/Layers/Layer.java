package Layers;

import Utilities.Matrix;

public abstract class Layer {
	protected NeuralNetwork network;
	protected int index;
	
	protected int inputMatrixCount, inputNRows, inputNCols;
	protected int outputMatrixCount, outputNRows, outputNCols;
	
	public Layer(NeuralNetwork network, int index) {
		this.network = network;
		this.index = index;
	}
	
	public abstract void initialize();
	
	public Layer getPreviousLayer() { 
		if(index == 0)
			return null;
		return network.getLayers()[index - 1];
	}
	
	protected abstract Matrix[] forward(Matrix[] input);
	public Matrix[] feedFoward(Matrix[] input) {
		if(index == network.getLayerCount() - 1)
			return forward(input);
		return network.getLayers()[index + 1].feedFoward(forward(input));
	}

	protected abstract Matrix[] back(Matrix[] error);
	public Matrix[] backpropogate(Matrix[] error) {
		if(index == 0)
			return back(error);
		return network.getLayers()[index - 1].backpropogate(back(error));
	}
	
	public int getInputMatrixCount() { return inputMatrixCount; }
	public int getInputNRows() { return inputNRows; }
	public int getInputNCols() { return inputNCols; }
	public int getOutputMatrixCount() { return outputMatrixCount; }
	public int getOutputNRows() { return outputNRows;}
	public int getOutputNCols() { return outputNCols; }
}
