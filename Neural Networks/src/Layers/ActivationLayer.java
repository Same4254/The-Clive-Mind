package Layers;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ActivationFunction;

public class ActivationLayer extends Layer {
	private ActivationFunction activationFunction;
	private Matrix[] gradients;
	
	public ActivationLayer(NeuralNetwork network, int layer, ActivationFunction activationFunction) {
		super(network, layer);
		
		this.activationFunction = activationFunction;
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
		
		outputMatrixCount = inputMatrixCount;
		outputNRows = inputNRows;
		outputNCols = inputNCols;
		
		gradients = new Matrix[inputMatrixCount];
	}

	@Override
	protected Matrix[] forward(Matrix[] input) {
		Matrix[] toRet = new Matrix[input.length];
		
		for(int i = 0; i < toRet.length; i++) {
			toRet[i] = input[i].forEach(activationFunction.getFunction());
			
			gradients[i] = input[i].forEach(activationFunction.getDerivativeFunction());
		}
		
		return toRet;
	}

	@Override
	protected Matrix[] back(Matrix[] error) {
		Matrix[] toRet = new Matrix[error.length];
		
		for(int i  = 0; i < toRet.length; i++)
			toRet[i] = error[i].hadamardProduct(gradients[i]);
		
		return toRet;
	}
}
