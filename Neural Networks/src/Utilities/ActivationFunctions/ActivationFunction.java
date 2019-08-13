package Utilities.ActivationFunctions;

import java.util.function.Function;

public abstract class ActivationFunction {
	protected Function<Double, Double> function, derivativeFunction;

	protected ActivationFunction() {
		this.function = createFunction();
		this.derivativeFunction = createDerivativeFunction();
	}
	
	protected abstract Function<Double, Double> createFunction();
	protected abstract Function<Double, Double> createDerivativeFunction();
	
	public Function<Double, Double> getFunction() { return function; }
	public Function<Double, Double> getDerivativeFunction() { return derivativeFunction; }
}
