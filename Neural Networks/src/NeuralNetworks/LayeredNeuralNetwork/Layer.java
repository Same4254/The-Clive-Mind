package NeuralNetworks.LayeredNeuralNetwork;

import Utilities.Matrix;

public abstract class Layer {
	protected LayeredNeuralNetwork network;
	protected int index;
	
	protected int inputMatrixCount, inputNRows, inputNCols;
	protected int outputMatrixCount, outputNRows, outputNCols;
	
	public Layer(LayeredNeuralNetwork network, int index) {
		this.network = network;
		this.index = index;
	}

	protected abstract void initialize();
	public void init() {
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
		
		initialize();
	}
	
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
	
	protected abstract Matrix[] calculate(Matrix[] input);
	public Matrix[] calculateForwards(Matrix[] input) {
		if(index == network.getLayerCount() - 1)
			return calculate(input);
		return network.getLayers()[index + 1].calculateForwards(calculate(input));
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
