package NeuralNetworks.LayeredNeuralNetwork;

import NeuralNetworks.FullyConnected;
import Utilities.Matrix;

public class FullyConnectedLayer extends Layer {
	private FullyConnected fullyConnected;
	
	public FullyConnectedLayer(LayeredNeuralNetwork network, int index, int[] layerSizes) {
		super(network, index);

		fullyConnected = new FullyConnected(layerSizes);
	}

	@Override
	public void initialize() {
		outputMatrixCount = 1;
		outputNRows = fullyConnected.getOutputLength();
		outputNCols = 1;
	}

	@Override
	protected Matrix[] forward(Matrix[] input) {
		double[] in = new double[fullyConnected.getLayerSizes()[0]];
		
		int index = 0;
		
		for(int i = 0; i < input.length; i++) {
			for(int row = 0; row < inputNRows; row++) {
			for(int col = 0; col < inputNCols; col++) {
				in[index] = input[i].get(row, col);
				
				index++;
			}}
		}
		
		return new Matrix[] { fullyConnected.feedForward(in) };
	}

	@Override
	protected Matrix[] calculate(Matrix[] input) {
		double[] in = new double[fullyConnected.getLayerSizes()[0]];
		
		int index = 0;
		
		for(int i = 0; i < input.length; i++) {
			for(int row = 0; row < inputNRows; row++) {
			for(int col = 0; col < inputNCols; col++) {
				in[index] = input[i].get(row, col);
				
				index++;
			}}
		}
		
		return new Matrix[] { fullyConnected.calculate(in) };
	}
	
	@Override
	protected Matrix[] back(Matrix[] previousError) {
		Matrix error = fullyConnected.nesterovBackpropogate(previousError[0]);

		if(index != 0) {
			Matrix[][] kernals = new Matrix[fullyConnected.getLayerSizes()[1]][inputMatrixCount];
			for(int kernalDimension = 0; kernalDimension < inputMatrixCount; kernalDimension++) {
				for(int kernal = 0; kernal < kernals.length; kernal++) {
					kernals[kernal][kernalDimension] = new Matrix(inputNRows, inputNCols);
				
					int index = inputNRows * kernalDimension;
					for(int row = 0; row < kernals[0][0].getNRows(); row++) {
					for(int col = 0; col < kernals[0][0].getNCols(); col++) {
						kernals[kernal][kernalDimension].set(row, col, fullyConnected.getWeights()[0].get(kernal, index));
						
						index++;
					}}
				}
			}
			
			Matrix[] nextError = new Matrix[inputMatrixCount]; 
			for(int i = 0; i < nextError.length; i++) {
				Matrix sum = null;
				
				for(int j = 0; j < kernals.length; j++) {
					
					Matrix temp = ConvolutionalLayer.fullConvolute(error.subMatrix(j, 0, 1, 1), kernals[j][i].flip(), 1);
					
					if(sum == null)//Why construct a whole other matrix?
						sum = temp;//Set to the current matrix, and then start adding to it
					else
						sum.mAdd(temp);
				}
				
				nextError[i] = sum;
			}
			
			return nextError;
		}
		
		return null;
	}
}
