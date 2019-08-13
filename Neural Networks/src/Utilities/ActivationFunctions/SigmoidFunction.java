package Utilities.ActivationFunctions;

import java.util.function.Function;

public class SigmoidFunction extends ActivationFunction {
	@Override
	protected Function<Double, Double> createFunction() {
		return x -> 1.0 / (1.0 + Math.pow(Math.E, -x)); 
	}

	@Override
	protected Function<Double, Double> createDerivativeFunction() {
		return x -> function.apply(x) * (1.0 - function.apply(x));
	}
}
