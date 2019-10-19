package NeuralNetworks;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Stroke;

import javax.swing.JFrame;
import javax.swing.JPanel;

import Utilities.Matrix;

public class FullyConnectedRenderer {
	private static int colGap = 120, rowGap = 20;
	private static int activationSize = 80;
	private static float largestStroke = 7;
	
	private FullyConnected network;
	private JPanel panel;
 	
	private String[] inputNames, outputNames;
	
	private int width, height;
	private int[] layerSizes;
	
	public FullyConnectedRenderer(FullyConnected network) {
		this.network = network;
		
		layerSizes = network.getLayerSizes();
		int largestColumn = 0;

		for(int i = 0; i < layerSizes.length; i++)
			if(layerSizes[i] > largestColumn) largestColumn = layerSizes[i];
		
		height = calculateColHeight(largestColumn);
		width = activationSize * layerSizes.length + (layerSizes.length - 1) * colGap + 30;
		
		inputNames = new String[] { "In 1", "In 2","In 3","In 4","In 5","In 6",};
		outputNames = new String[] { "Out 1","Out 2","Out 3", };
	}
	
	public void render(Graphics2D g2d, int xOffset, int yOffset) {
		g2d.translate(xOffset, yOffset);
		
		Color startColor = g2d.getColor();
		Stroke startStroke = g2d.getStroke();
		
		Matrix[] biasMatricies = network.getBiases();
		double[][] biases = new double[biasMatricies.length][];
		for(int i = 0; i < biasMatricies.length; i++) 
			biases[i] = biasMatricies[i].getColumn(0);
		
		Matrix[] weightMatricies = network.getWeights();
		double mostNegativeWeight = 0, mostPositiveWeight = 0;
		
		for(Matrix m : weightMatricies) {
			double positive = m.mostPositive();
			double negative = m.mostNegative();
			
			if(negative < mostNegativeWeight)
				mostNegativeWeight = negative;
			if(positive > mostPositiveWeight)
				mostPositiveWeight = positive;
		}
		
		int x = 10, y = 0;
		for(int i = 0; i < layerSizes.length; i++) {
			int height = calculateColHeight(layerSizes[i]);
			y = (int) ((this.height / 2.0) - (height / 2.0)) + rowGap;
			
			for(int j = 0; j < layerSizes[i]; j++) {
				if(i < layerSizes.length - 1) { 
					Matrix weights = weightMatricies[i];
					
					for(int w = 0; w < weights.getNRows(); w++) {
						double weightToDraw = weights.get(w, j);
						int x1 = x + activationSize, y1 = (int) (y + (activationSize / 2.0));
						int x2 = x + activationSize + colGap, y2 = getMiddleOfIndex(w, layerSizes[i + 1], this.height);
						
						Color color = null;
						BasicStroke stroke = null;
						
						if(weightToDraw < 0) {
							int r = (int) (255.0 * (weightToDraw / mostNegativeWeight));
							if(r > 255)
								r = 255;
							if(r < 0)
								r = 0;
							
							color = new Color(r, 0, 0);
							
							stroke = new BasicStroke(Math.max((float) (largestStroke * (weightToDraw / mostNegativeWeight)), 1));
						} else {
							int b = (int) (255.0 * (weightToDraw / mostPositiveWeight));
							if(b > 255)
								b = 255;
							if(b < 0)
								b = 0;
							
							color = new Color(0, 0, b);
							stroke = new BasicStroke(Math.max((float) (largestStroke * (weightToDraw / mostPositiveWeight)), 1));
						}
						
						g2d.setStroke(stroke);
						g2d.setColor(color);
						g2d.drawLine(x1, y1, x2, y2);
					}
				}
				
				g2d.setStroke(startStroke);
				g2d.setColor(new Color(50, 50, 50));
				g2d.fillRect(x, y, activationSize, activationSize);
				
				if(i == 0 && network.getInput().getValues().length > 0) {
					g2d.setStroke(startStroke);
					g2d.setColor(Color.WHITE);
					
					if(inputNames.length > 0) 
						g2d.drawString(inputNames[j], x + 5, y + 20);
					g2d.drawString("In: " + Double.toString(((int) (network.getInput().getValues()[j][0] * 1000.0)) / 1000.0), x + 5, y + 40);
				}
				
				if(i > 0) {
					g2d.setStroke(startStroke);
					g2d.setColor(Color.WHITE);
					g2d.drawString("B: " + Double.toString( ((int) (biases[i - 1][j] * 1000.0)) / 1000.0), x + 5, y + 40);
					
					if(i != layerSizes.length - 1)
						g2d.drawString("Hidden", x + 5, y + 20);
				}
				
				if(i == layerSizes.length - 1 && network.getOutput().getValues().length > 0) {
					g2d.setStroke(startStroke);
					g2d.setColor(Color.WHITE);
					
					if(outputNames.length > 0)
						g2d.drawString(outputNames[j], x + 5, y + 20);
					g2d.drawString("Out: " + Double.toString(((int) (network.getOutput().getValues()[j][0] * 1000.0)) / 1000.0), x + 5, y + 60);
				} else if(i != 0 && network.getActivations()[0] != null) {
					g2d.setStroke(startStroke);
					g2d.setColor(Color.WHITE);
					g2d.drawString("Act: " + Double.toString( ((int) (network.getActivations()[i]
							.getValues()[j][0] * 10000.0)) / 10000.0), x + 5, y + 60);
				}
				
				y += activationSize + rowGap;
			}
			
			x += activationSize + colGap;
		}
		
		g2d.setColor(startColor);
		g2d.setStroke(startStroke);
		
		g2d.translate(-xOffset, -yOffset);
	}
	
	private int getMiddleOfIndex(int index, int size, int greatestHeight) {
		int height = calculateColHeight(size);
		int y = (int) ((greatestHeight / 2.0) - (height / 2.0)) + rowGap;
		return (int) (y + (activationSize + rowGap) * index + (activationSize / 2.0));
	}
	
	private int calculateColHeight(int size) {
		return (size + 1) * rowGap + (size + 1) * activationSize;
	}
	
	public void notifyRepaint() {
		if(panel != null)
			panel.repaint();
	}
	
	public void assignPanel(JPanel panel) {	this.panel = panel; }
	public int getWidth() { return width; }
	public int getHeight() { return height; }
	
	public static void main(String[] args) throws InterruptedException {
		JFrame frame = new JFrame();
		frame.setSize(800, 700);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		FullyConnected network = new FullyConnected(new int[] { 6, 3, 4, 3 });
		TempPanel panel = new TempPanel(network);
		
		frame.add(panel);
		
		frame.setVisible(true);
		
		while(true) {
			network.feedForward(new double[] {.4, .3, .7, .4, .2, .1});
			network.nesterovBackpropogate(new double[] { .5, .3, .8});
			
			panel.repaint();
			
			Thread.sleep(1);
		}
	}
	
	static class TempPanel extends JPanel {
		private static final long serialVersionUID = 1L;
		
		private FullyConnected network;
		private FullyConnectedRenderer render;
		
		public TempPanel(FullyConnected network) {
			this.network = network;
			this.render = new FullyConnectedRenderer(network);
		}
		
		@Override
		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			
			Graphics2D g2d = (Graphics2D) g;
			render.render(g2d, 0, 0);
		}
	}
}
