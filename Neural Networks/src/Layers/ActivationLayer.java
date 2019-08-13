package Layers;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ActivationFunction;

public class ActivationLayer {
	private ActivationFunction activationFunction;
	public ActivationLayer(ActivationFunction activationFunction) {
		this.activationFunction = activationFunction;
	}
	
	public Matrix[] calculate(Matrix[] input) {
		Matrix[] toRet = new Matrix[input.length];
		
		for(int i = 0; i < toRet.length; i++) 
			toRet[i] = input[i].forEach(activationFunction.getFunction());
		
		return toRet;
	}
}
