package MNIST;

import java.util.ArrayList;

import Layers.FullyConnected;

public class Mnist {
	public static FullyConnected network;
	
	public static int[] trainLabels = MnistReader.getLabels("Mnist Dataset/train-labels.idx1-ubyte");
	public static ArrayList<int[][]> trainImages = (ArrayList<int[][]>) MnistReader.getImages("Mnist Dataset/train-images.idx3-ubyte");
	
	public static int[] testLabels = MnistReader.getLabels("Mnist Dataset/t10k-labels.idx1-ubyte");
	public static ArrayList<int[][]> testImages = (ArrayList<int[][]>) MnistReader.getImages("Mnist Dataset/t10k-images.idx3-ubyte");
	
	public static double[] convert(int[][] image) {
		double[] toRet = new double[image.length * image[0].length];
		int index = 0;
		
		for(int row = 0; row < image.length; row++) {
		for(int col = 0; col < image[0].length; col++) {
			toRet[index] = image[row][col];
			
			index++;
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
			double result = greatest(network.calculate(convert(testImages.get(i))).getColumn(0));
			
			if(label == result)
				correct++;
		}
		
		return correct / 10000.0;
	}
	
	public static void main(String[] args) {
		network = new FullyConnected(new int[] { 784, 32, 32, 10 });

		for(int j = 0; j < 100; j++) {
			System.out.println(evaluate() * 100.0);
			for(int i = 0; i < 55000; i++) {
				int label = trainLabels[i];
				greatest(network.calculate(convert(trainImages.get(i))).getColumn(0));
				
				double[] answers = new double[10];
				answers[label] = 1;
				
				network.batchedNesterovBackpropogate(answers);
			}
		}
	}
}
