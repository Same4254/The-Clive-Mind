package MNIST;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;

import NeuralNetworks.FullyConnected;
import NeuralNetworks.LayeredNeuralNetwork.ActivationLayer;
import NeuralNetworks.LayeredNeuralNetwork.ConvolutionalLayer;
import NeuralNetworks.LayeredNeuralNetwork.FullyConnectedLayer;
import NeuralNetworks.LayeredNeuralNetwork.Layer;
import NeuralNetworks.LayeredNeuralNetwork.LayeredNeuralNetwork;
import NeuralNetworks.LayeredNeuralNetwork.PoolingLayer;
import Utilities.Matrix;
import Utilities.ActivationFunctions.ReluFunction;

public class MnistTraining {
	public static LayeredNeuralNetwork network;
	
	public static int[] trainLabels = MnistReader.getLabels(System.getProperty("user.dir") + "/train-labels.idx1-ubyte");
	public static ArrayList<int[][]> trainImages = (ArrayList<int[][]>) MnistReader.getImages(System.getProperty("user.dir") + "/train-images.idx3-ubyte");
	
	public static int[] testLabels = MnistReader.getLabels(System.getProperty("user.dir") + "/t10k-labels.idx1-ubyte");
	public static ArrayList<int[][]> testImages = (ArrayList<int[][]>) MnistReader.getImages(System.getProperty("user.dir") + "/t10k-images.idx3-ubyte");
	
	public static double[] convert(int[][] image) {
		double[] toRet = new double[image.length * image[0].length];
		int index = 0;
		
		for(int row = 0; row < image.length; row++) {
		for(int col = 0; col < image[0].length; col++) {
			toRet[index] = image[row][col] / 255.0;
			
			index++;
		}}
		
		return toRet;
	}
	
	public static Matrix normalize(int[][] image) {
		Matrix toRet = new Matrix(image.length, image[0].length);
		for(int row = 0; row < image.length; row++) {
		for(int col = 0; col < image[0].length; col++) {
			toRet.set(row, col, image[row][col] / 255.0);
		}}
		
		return toRet;
	}
	
	public static int greatest(double[] values) {
		int greatest = 0;
		
		for(int i = 1; i < values.length; i++)
			if(values[i] > values[greatest]) greatest = i;
		
		return greatest;
	}
	
	public static double evaluate() {
		int correct = 0;
		
		for(int i = 0; i < 10000; i++) {
			int label = testLabels[i];
			double result = greatest(network.feedForward(new Matrix[] { normalize(testImages.get(i))})[0].getColumn(0));
			
			if(label == result)
				correct++;
		}
		
		return correct / 10000.0;
	}
	
	public static BufferedImage getImage(String path) {
		try {
			return ImageIO.read(new File(path));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return null;
	}
	
	public static BufferedImage getImage(File file) {
		try {
			return ImageIO.read(file);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return null;
	}
	
	public static void main(String[] args) {
//		File file = new File(System.getProperty("user.dir"), "mnist93model.txt");
//		if(!file.exists()) {
//			try {
//				file.createNewFile();
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//		}
//		
//		BufferedImage image = getImage(new File(System.getProperty("user.dir"), "test.png"));
//		
//		double[] data = new double[784];
//		
//		int index = 0;
//		
//		for(int y = 0; y < image.getHeight(); y++) {
//			for(int x = 0; x < image.getWidth(); x++) {
//				Color color = new Color(image.getRGB(x, y));
//				data[index] = color.getRed() / 255.0;
//			
//				index++;
//			}
//		}
		
//		network = new FullyConnected(new int[] { 784, 32, 32, 10 });
		
		network = new LayeredNeuralNetwork(1, 28, 28, 10, 1);
		
//		network.layers = new Layer[1];
//		FullyConnected connected = new FullyConnected(network, 0, new int[] { 784, 32, 32, 10 });
//		network.layers[0] = connected;
//		connected.initialize();
		
		network.layers = new Layer[2];
		ConvolutionalLayer conv = new ConvolutionalLayer(network, 0, 1, 5, 18, 5, 1);
		FullyConnectedLayer connected = new FullyConnectedLayer(network, 1, new int[] {648, 10});
		
		network.layers[0] = conv;
		network.layers[1] = connected;
		
		conv.init();
		connected.init();
		
//		network.layers = new Layer[11];
//		ConvolutionalLayer conv1 = new ConvolutionalLayer(network, 0, 2, 1, 32, 5, 1);
//		ActivationLayer activation1 = new ActivationLayer(network, 1, new ReluFunction());
//		
//		ConvolutionalLayer conv2 = new ConvolutionalLayer(network, 2, 2, 1, 32, 5, 32);
//		ActivationLayer activation2 = new ActivationLayer(network, 3, new ReluFunction());
//		
//		PoolingLayer pool1 = new PoolingLayer(network, 4, 2, 2);
//		
//		ConvolutionalLayer conv3 = new ConvolutionalLayer(network, 5, 1, 1, 64, 3, 32);
//		ActivationLayer activation3 = new ActivationLayer(network, 6, new ReluFunction());
//		
//		ConvolutionalLayer conv4 = new ConvolutionalLayer(network, 7, 1, 1, 64, 3, 64);
//		ActivationLayer activation4 = new ActivationLayer(network, 8, new ReluFunction());
//		
//		PoolingLayer pool2 = new PoolingLayer(network, 9, 2, 2);
//		
//		FullyConnectedLayer connected = new FullyConnectedLayer(network, 10, new int[] {3136, 256, 10});
////		FullyConnectedLayer connected = new FullyConnectedLayer(network, 0, new int[] {784, 32, 32, 10});
//		
//		network.layers[0] = conv1;
//		network.layers[1] = activation1;
//		network.layers[2] = conv2;
//		network.layers[3] = activation2;
//		network.layers[4] = pool1;
//		network.layers[5] = conv3;
//		network.layers[6] = activation3;
//		network.layers[7] = conv4;
//		network.layers[8] = activation4;
//		network.layers[9] = pool2;
//		network.layers[10] = connected;
//		
//		conv1.init();
//		activation1.init();
//		conv2.init();
//		activation2.init();
//		pool1.init();
//		conv3.init();
//		activation3.init();
//		conv4.init();
//		activation4.init();
//		pool2.init();
//		connected.init();
		
//		network = new FullyConnected(file);
		
//		System.out.println(greatest(network.feedForward(data).getColumn(0)));
		
		System.out.println("Initial, " + (evaluate() * 100.0) + "%");
		
//		Matrix[][] initialKernals = new Matrix[conv1.getKernals().length][conv1.getKernals()[0].length];
//		
//		for(int i = 0; i < initialKernals.length; i++) {
//		for(int j = 0; j < initialKernals[0].length; j++) {
//			initialKernals[i][j] = new Matrix(conv1.getKernals()[i][j]);
//		}}
		
		for(int j = 0; j < 5; j++) {
			long startTime = System.currentTimeMillis();
			
			for(int i = 0; i < 10000; i++) {
				int label = trainLabels[i];
				network.feedForward(new Matrix[] { normalize(trainImages.get(i)) });
				
				double[] answers = new double[10];
				answers[label] = 1;
				
				network.backpropogate(new Matrix[] { new Matrix(10, 1, answers) } );
				
//				network.batchedNesterovBackpropogate(answers);
//				network.nesterovBackpropogate(answers);
//				network.resilientBackpropogation(answers);
			}
			
			System.out.println(((System.currentTimeMillis() - startTime) / 1000.0) + "s, " + (evaluate() * 100.0) + "%");
			
//			for(int i = 0; i < conv.getKernals().length; i++) {
//				for(int k = 0; k < conv.getKernals()[0].length; k++) {
//					System.out.println(conv.getKernals()[i][k]);
//				}}
		}
		
		
	}
}
