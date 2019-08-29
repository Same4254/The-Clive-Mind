package MNIST;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;

import Layers.FullyConnected;

public class MnistTraining {
	public static FullyConnected network;
	
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
			double result = greatest(network.feedForward(convert(testImages.get(i))).getColumn(0));
			
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
		
		network = new FullyConnected(new int[] { 784, 32, 32, 10 });

//		network = new FullyConnected(file);
		
//		System.out.println(greatest(network.feedForward(data).getColumn(0)));
		
//		
		System.out.println("Initial, " + (evaluate() * 100.0) + "%");
		
		for(int j = 0; j < 100; j++) {
			long startTime = System.currentTimeMillis();
			
			for(int i = 0; i < 55000; i++) {
				int label = trainLabels[i];
				greatest(network.feedForward(convert(trainImages.get(i))).getColumn(0));
				
				double[] answers = new double[10];
				answers[label] = 1;
				
//				network.batchedNesterovBackpropogate(answers);
				network.nesterovBackpropogate(answers);
//				network.resilientBackpropogation(answers);
			}
			
			System.out.println((System.currentTimeMillis() - startTime) + "ms, " + (evaluate() * 100.0) + "%");
			
//			network.save(file);
		}
	}
}
