package Layers;

import Utilities.Matrix;
import Utilities.ActivationFunctions.ReluFunction;

public class ConvolutionalLayer extends Layer {
	private Matrix[] input, output;
	
	private Matrix[][] kernals;
	private double[] biases;
	
	private int padding, stride;
	private int kernalCount, kernalSize;
	
	private Matrix[][] weightVelocities;
	private double[] biasVelocities;
			
	private double velocityCoeficcient = 0.9;
	private double learningRate = 0.001;
	
	/**
	 * Sources:
	 * 	https://www.jefkine.com/general/2016/09/05/backpropagation-in-convolutional-neural-networks/
	 *  http://cs231n.github.io/convolutional-networks/
	 *  http://yann.lecun.com/exdb/publis/pdf/lecun-98b.pdf
	 *  https://becominghuman.ai/only-numpy-implementing-convolutional-neural-network-using-numpy-deriving-forward-feed-and-back-458a5250d6e4
	 *  
	 *  https://stats.stackexchange.com/questions/361817/back-propagation-in-convolution-layer
	 */
	public ConvolutionalLayer(NeuralNetwork network, int layer, int padding, int stride, int kernalCount, int kernalSize, int kernalDimensionalExtent) {
		super(network, layer);
		
		this.padding = padding;
		this.stride = stride;
		this.kernalCount = kernalCount;
		this.kernalSize = kernalSize;
		
		kernals = new Matrix[kernalCount][kernalDimensionalExtent];
		weightVelocities = new Matrix[kernalCount][kernalDimensionalExtent];
		
		for(int i = 0; i < kernals.length; i++) {
		for(int j = 0; j < kernals[0].length; j++) {
			kernals[i][j] = Matrix.random(kernalSize, kernalSize, -1, 1);
			weightVelocities[i][j] = new Matrix(kernalSize, kernalSize);
		}}
		
		biases = new double[kernalCount];
		biasVelocities = new double[biases.length];
		
		for(int i = 0; i < biases.length; i++) 
			biases[i] = (Math.random() * 2.0) - 1.0;
	}

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
		
		outputMatrixCount = kernalCount;
		
//		System.out.println(inputNCols);
		
		double outputWidth = ((inputNCols - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
		double outputHeight = ((inputNRows - kernalSize + (2.0 * padding)) / ((double) stride)) + 1.0;
		
		if((int) outputWidth != outputWidth) {
			System.err.println("Invalid Conv Width!");
//			System.exit(-1);
		}
		
		if((int) outputHeight != outputHeight) {
			System.err.println("Invalid Conv Height!");
//			System.exit(-1);
		}
		
		outputNCols = (int) outputWidth;
		outputNRows = (int) outputHeight;
		
		output = new Matrix[kernalCount];
		for(int i = 0; i < output.length; i++) 
			output[i] = new Matrix(outputNRows, outputNCols);
	}
	
	protected Matrix[] forward(Matrix[] input) {
		if(padding != 0) {
			for(int i = 0; i < input.length; i++) {
				input[i] = input[i].pad(padding);
			}
		}

		this.input = input;

//		System.out.println("-------");
//		System.out.println(index);
//		System.out.println(output[0]);
		
		for(int y = 0; y <= input[0].getNRows() - kernalSize; y += stride) {
		for(int x = 0; x <= input[0].getNCols() - kernalSize; x += stride) {
			for(int kernalSet = 0; kernalSet < kernals.length; kernalSet++) {
				double sum = 0.0;
				for(int kernal = 0; kernal < kernals[kernalSet].length; kernal++) 
					sum += input[kernal].elementWiseProduct(kernals[kernalSet][kernal], x, y);
				
				sum += biases[kernalSet];
				
				output[kernalSet].set(y / stride, x / stride, sum);
			}
		}}
		
		return output;
	}
	
	/*
	 * Calculate error on new kernals or old ones?
	 * What gradient does the bias use? The one that is passed in, or the one for this layer?
	 */
	protected Matrix[] back(Matrix[] error) {
		Matrix[] toRet = new Matrix[error.length]; 
		
		for(int i = 0; i < error.length; i++) {
			Matrix sum = null; //= new Matrix(inputNRows, inputNCols);
			
			for(int j = 0; j < kernalCount; j++) {
				Matrix m = fullConvolute(error[i], kernals[j][i].flip());
				
				if(sum == null)
					sum = new Matrix(m.getNRows(), m.getNCols());
				
//				System.out.println(kernals[j][i]);
				
				sum.mAdd(m);
			}
			
			toRet[i] = sum;
		}
		
		for(int i = 0; i < error.length; i++) {
			Matrix weightGradient = convolute(input[i], error[i]);
			
			biasVelocities[i] *= velocityCoeficcient;
			
			biases[i] -= biasVelocities[i];
			
			biasVelocities[i] -= weightGradient.sum() * learningRate;
			
			biases[i] += biasVelocities[i] * (1.0 + velocityCoeficcient);
			
//				biases[weightLayer].mSubtract(biasVelocities[weightLayer])
//					.mAdd(biasVelocities[weightLayer].mSubtract(error.scale(learningRate)).scale(1.0 + velocityCoeficcient));
			
			for(int kernal = 0; kernal < kernals[i].length; kernal++) {
				weightVelocities[i][kernal].mScale(velocityCoeficcient);
				   
				kernals[i][kernal].mSubtract(weightVelocities[i][kernal])
					.mAdd(weightVelocities[i][kernal].mSubtract(weightGradient.mScale(learningRate)).scale(1.0 + velocityCoeficcient));
				
//				kernals[kernalSet][kernal].mSubtract(weightGradient.mScale(0.001));
			}
		}
		
//		System.out.println("Input " + index);
//		System.out.println(input[0]);
//		
//		System.out.println("Output " + index);
//		System.out.println(output[0]);
//		
//		System.out.println("Error " + index);
//		System.out.println(error[0]);
//		
//		System.out.println("Kernal " + index);
//		System.out.println(kernals[0][0]);
//		
//		System.out.println("Ret " + index);
		
//		System.out.println(toRet);
		
		return toRet;
	}
	
	public static Matrix fullConvolute(Matrix data, Matrix filter) {
		Matrix d = data.pad(filter.getNRows() - 1);
		
//		System.out.println(d);
		
		return convolute(d, filter, filter.getNRows());
	}
	
	public static Matrix fullConvolute(Matrix data, Matrix filter, int stride) {
		Matrix d = data.pad(filter.getNRows() - 1);
		
//		System.out.println(d);
		
		return convolute(d, filter, stride);
	}
	
	public static Matrix convolute(Matrix data, Matrix filter) {
		return convolute(data, filter, filter.getNRows());
	}
	
	public static Matrix convolute(Matrix data, Matrix filter, int stride) {
		double outputWidth = ((data.getNCols() - filter.getNCols()) / ((double) stride)) + 1.0;
		double outputHeight = ((data.getNRows() - filter.getNRows()) / ((double) stride)) + 1.0;
		
		if((int) outputWidth != outputWidth) {
			System.err.println("Invalid Conv Width!");
			return null;
		}
		
		if((int) outputHeight != outputHeight) {
			System.err.println("Invalid Conv Height!");
			return null;
		}
		
		Matrix output = new Matrix((int) outputHeight, (int) outputWidth);
		
		for(int y = 0; y <= data.getNRows() - filter.getNRows(); y += stride) {
		for(int x = 0; x <= data.getNCols() - filter.getNCols(); x += stride) {
			output.set(y / stride, x / stride, data.elementWiseProduct(filter, x, y));
		}}
		
		return output;
	}
	
	public Matrix[][] getKarnals() { return kernals; }
	
	public static void main(String[] args) {
		NeuralNetwork network = new NeuralNetwork(1, 8, 8, 3, 1);
		
		ConvolutionalLayer conv1 = new ConvolutionalLayer(network, 0, 0, 2, 1, 2, 2);
		ActivationLayer activation = new ActivationLayer(network, 1, new ReluFunction());
		ConvolutionalLayer conv2 = new ConvolutionalLayer(network, 2, 0, 2, 2, 2, 1);
		FullyConnected full = new FullyConnected(network, 3, new int[] {8, 3});
		
		network.layers = new Layer[4];
		network.layers[0] = conv1;
		network.layers[1] = activation;
		network.layers[2] = conv2;
		network.layers[3] = full;
		
		conv1.initialize();
		activation.initialize();
		conv2.initialize();
		full.initialize();
		
		for(int i = 0; i < 2000; i++) {
			Matrix[] out = network.feedForward(new Matrix[] {
				new Matrix(new double[][] {
					{0.5792389362467796, 0.9211249458082603, 0.5333749112829619, 0.9388725662073563, 0.005098582026965448, 0.4290278331324482, 0.22153076187049558, 0.8303867778242918},
					{0.7619360869189011, 0.5263370840546454, 0.26881182458224206, 0.378941560286265, 0.43460684177431574, 0.03623056022874438, 0.558991982493623, 0.47759396409576915},
					{0.11044642101644542, 0.28018810891028656, 0.7610696334001295, 0.7501802694001208, 0.6826697621010581, 0.5744528128136138, 0.8361376419102844, 0.29721124112233244},
					{0.09883598069859989, 0.32604541953559174, 0.35328668272923036, 0.5157620894652506, 0.80501939352295, 0.6862480988843276, 0.46137420267732376, 0.8013709017929852},
					{0.7427731712020266, 0.3962316568367592, 0.923061787255414, 0.1948425309305073, 0.5186671047593546, 0.5425081257102878, 0.8333434343858379, 0.19798702311243332},
					{0.7851261054210508, 0.4291482961742722, 0.5168584826101902, 0.0497055220795406, 0.4464916709108353, 0.6692117794878232, 0.6266021819364491, 0.7274683049634093},
					{0.03567386728627897, 0.8995950513891406, 0.6512182305183563, 0.8191131990281166, 0.4944563639002051, 0.23151027475074581, 0.8788769465009179, 0.1960251456232045},
					{0.6344626739163071, 0.6630662996925635, 0.36281403558405556, 0.11114044504754883, 0.75598702539214, 0.20093019158447434, 0.5869242768398852, 0.9438521589234725}
				}),
				
				new Matrix(new double[][] {
					{0.5792389362467796, 0.9211249458082603, 0.5333749112829619, 0.9388725662073563, 0.005098582026965448, 0.4290278331324482, 0.22153076187049558, 0.8303867778242918},
					{0.7619360869189011, 0.5263370840546454, 0.26881182458224206, 0.378941560286265, 0.43460684177431574, 0.03623056022874438, 0.558991982493623, 0.47759396409576915},
					{0.11044642101644542, 0.28018810891028656, 0.7610696334001295, 0.7501802694001208, 0.6826697621010581, 0.5744528128136138, 0.8361376419102844, 0.29721124112233244},
					{0.09883598069859989, 0.32604541953559174, 0.35328668272923036, 0.5157620894652506, 0.80501939352295, 0.6862480988843276, 0.46137420267732376, 0.8013709017929852},
					{0.7427731712020266, 0.3962316568367592, 0.923061787255414, 0.1948425309305073, 0.5186671047593546, 0.5425081257102878, 0.8333434343858379, 0.19798702311243332},
					{0.7851261054210508, 0.4291482961742722, 0.5168584826101902, 0.0497055220795406, 0.4464916709108353, 0.6692117794878232, 0.6266021819364491, 0.7274683049634093},
					{0.03567386728627897, 0.8995950513891406, 0.6512182305183563, 0.8191131990281166, 0.4944563639002051, 0.23151027475074581, 0.8788769465009179, 0.1960251456232045},
					{0.6344626739163071, 0.6630662996925635, 0.36281403558405556, 0.11114044504754883, 0.75598702539214, 0.20093019158447434, 0.5869242768398852, 0.9438521589234725}
				})
			});
			
//			Matrix[] out = network.feedForward(new Matrix[] {
//				new Matrix(new double[][] {
//					{.4, 0, 0, .8},
//					{0, 0, 0, 0},
//					{0, 0, 0, 0},
//					{0, .9, .7, 0}
//				})	
//			});
			
			System.out.println("------------------------------");
			System.out.println(out[0]);
			System.out.println("-------------------------");
			
			network.backPropogate(new Matrix(new double[][] {
//				{2 * (out[0].get(0, 0) - .1) }
				{.5},
				{.7},
				{.1}
				
//				{2 * (out[0].get(0, 0) - .1), 2 * (out[0].get(0, 1) - .6)},//, Math.pow(-out[0].get(0, 1) + .7, 2)}
//				{2 * (out[0].get(1, 0) - .8), 2 * (out[0].get(1, 1) - .4)} 
//				{out[0].get(0, 0) - .4, out[0].get(0, 1) - .1},
//				{out[0].get(1, 0) - .9, out[0].get(1, 1) - .6}
			}));
		}
		
//		Matrix[] out = network.feedForward(new Matrix[] {
//			new Matrix(new double[][] {
//				{0, 0, 0, 0, 0, 0, 0},
//				{0, 0, 2, 2, 0, 0, 0},
//				{0, 2, 2, 1, 0, 2, 0},
//				{0, 1, 0, 1, 1, 0, 0},
//				{0, 0, 1, 0, 2, 1, 0},
//				{0, 2, 1, 0, 1, 0, 0},
//				{0, 0, 0, 0, 0, 0, 0}
//			}),
//			
//			new Matrix(new double[][] {
//				{0, 0, 0, 0, 0, 0, 0},
//				{0, 2, 2, 2, 2, 1, 0},
//				{0, 2, 1, 0, 0, 0, 0},
//				{0, 2, 0, 2, 1, 0, 0},
//				{0, 0, 2, 0, 1, 2, 0},
//				{0, 2, 1, 1, 1, 1, 0},
//				{0, 0, 0, 0, 0, 0, 0}
//			}),
//			
//			new Matrix(new double[][] {
//				{0, 0, 0, 0, 0, 0, 0},
//				{0, 2, 0, 2, 2, 2, 0},
//				{0, 0, 0, 2, 0, 1, 0},
//				{0, 2, 0, 0, 1, 1, 0},
//				{0, 1, 2, 1, 2, 1, 0},
//				{0, 2, 2, 2, 2, 0, 0},
//				{0, 0, 0, 0, 0, 0, 0}
//			})
//		});
	}
}
