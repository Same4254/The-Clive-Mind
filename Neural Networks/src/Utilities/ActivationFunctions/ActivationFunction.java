package Utilities.ActivationFunctions;

import java.util.function.Function;

public abstract class ActivationFunction {
	protected Function<Double, Double> function, derivativeFunction, inverseFunction;

	protected ActivationFunction() {
		this.function = createFunction();
		this.derivativeFunction = createDerivativeFunction();
		this.inverseFunction = createInverseFunction();
	}
	
	protected abstract Function<Double, Double> createFunction();
	protected abstract Function<Double, Double> createDerivativeFunction();
	protected abstract Function<Double, Double> createInverseFunction();
	
	public Function<Double, Double> getFunction() { return function; }
	public Function<Double, Double> getDerivativeFunction() { return derivativeFunction; }
	public Function<Double, Double> getInverseFunction() { return inverseFunction; }
}
