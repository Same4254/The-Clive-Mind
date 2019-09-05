package Utilities.ActivationFunctions;

import java.util.function.Function;

public class ReluFunction extends ActivationFunction {
	@Override
	protected Function<Double, Double> createFunction() {
		return x -> Math.max(0, x);
	}

	@Override
	protected Function<Double, Double> createDerivativeFunction() {
		return x -> x > 0 ? 1.0 : 0.0;
	}
}
