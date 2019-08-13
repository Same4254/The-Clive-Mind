package Layers;

import Utilities.Matrix;

public class PoolingLayer {
	private int size, stride;
	
	public PoolingLayer(int size, int stride) {
		this.size = size;
		this.stride = stride;
	}
	
	public Matrix[] maxPool(Matrix[] input) {
		Matrix[] toRet = new Matrix[input.length];
		
		for(int i = 0; i < toRet.length; i++) {
			double outputWidth = ((input[0].getNCols() - size) / ((double) stride)) + 1.0;
			double outputHeight = ((input[0].getNRows() - size) / ((double) stride)) + 1.0;
			
			if((int) outputWidth != outputWidth) {
				System.err.println("Invalid Pooling Width!");
				return null;
			}
			
			if((int) outputHeight != outputHeight) {
				System.err.println("Invalid Pooling Height!");
				return null;
			}
			
			toRet[i] = new Matrix((int) outputHeight, (int) outputWidth);
			
			for(int y = 0; y <= input[0].getNRows() - size; y += stride) {
			for(int x = 0; x <= input[0].getNCols() - size; x += stride) {
				toRet[i].set(y / stride, x / stride, input[i].subMatrix(x, y, size, size).mostPositive());
			}}
		}
		
		return toRet;
	}
	
	public static void main(String[] args) {
		PoolingLayer pool = new PoolingLayer(2, 2);
		System.out.println(pool.maxPool(new Matrix[] {
			new Matrix(new double[][] {
				{1, 1, 2, 4},
				{5, 6, 7, 8},
				{3, 2, 1, 0},
				{1, 2, 3, 4}
			})
		})[0]);
	}
}
