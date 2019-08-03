package Utilities;

import java.util.Arrays;
import java.util.function.Function;

public class Matrix {
	private double[][] values;
	private int nCols, nRows;

	public static Matrix random(int rows, int columns, double min, double max) {
		Matrix toRet = new Matrix(rows, columns);
		
		for(int x = 0; x < rows; x++) {
		for(int y = 0; y < columns; y++) {
			toRet.values[x][y] = (Math.random() * (max - min)) + min;
		}}
		
		return toRet;
	}
	
	public Matrix(double[][] values) {
		this.values = values;
		this.nRows = values.length;
		this.nCols = values[0].length;
	}
	
	public Matrix(int rows, int columns) {
		this.nRows = rows;
		this.nCols = columns;
		this.values = new double[nRows][nCols];
	}
	
	public Matrix(int rows, int columns, double[] values) {
		this(rows, columns);
		
		int index = 0;
		
		for(int row = 0; row < rows; row++) {
		for(int col = 0; col < columns; col++) {
			this.values[row][col] = values[index];
			
			index++;
		}}
	}
	
	public Matrix(Matrix copy) {
		this.nCols = copy.nCols;
		this.nRows = copy.nRows; 
		
		values = new double[copy.nRows][copy.nCols];
		
		for(int row = 0; row < copy.nRows; row++) {
		for(int col = 0; col < copy.nCols; col++) {
			values[row][col] = copy.values[row][col];
		}}
	}
	
	public Matrix transpose() {
		Matrix toRet = new Matrix(nCols, nRows);
		
		for(int row = 0; row < this.nRows; row++) {
		for(int col = 0; col < this.nCols; col++) {
			toRet.values[col][row] = this.values[row][col];
		}}
		
		return toRet;
	}
	
	public Matrix multiply(Matrix matrix) {
		Matrix toRet = new Matrix(this.nRows, matrix.nCols);
		
		for(int row = 0; row < this.nRows; row++) {
		for(int col = 0; col < matrix.nCols; col++) {
			double sum = 0.0;
			
			for(int i = 0; i < this.nCols; i++) {
				sum += this.values[row][i] * matrix.values[i][col];
			}
			
			toRet.values[row][col] = sum;
		}}
		
		return toRet;
	}
	
	public Matrix add(Matrix matrix) {
		Matrix toRet = new Matrix(this.nRows, this.nCols);
		
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			toRet.values[row][col] = this.values[row][col] + matrix.values[row][col];
		}}
		
		return toRet;
	}
	
	public Matrix subtract(Matrix matrix) {
		Matrix toRet = new Matrix(this.nRows, this.nCols);
		
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			toRet.values[row][col] = this.values[row][col] - matrix.values[row][col];
		}}
		
		return toRet;
	}
	
	public double elementWiseProduct(Matrix smaller, int x, int y) {
		double runningSum = 0.0;
		
		for(int row = y; row < y + smaller.nRows; row++) {
		for(int col = x; col < x + smaller.nCols; col++) {
			runningSum += values[row][col] * smaller.values[row - y][col - x]; 
		}}
		
		return runningSum;
	}
	
	public Matrix hadamardProduct(Matrix matrix) {
		Matrix toRet = new Matrix(nRows, nCols);
		
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			toRet.values[row][col] = values[row][col] * matrix.values[row][col];
		}}
		
		return toRet;
	}
	
	public Matrix pad(int amount) {
		Matrix matrix = new Matrix(this.nRows + (2 * amount), this.nCols + (2 * amount));
		
		for(int row = amount; row < this.nRows + amount; row++) {
		for(int col = amount; col < this.nCols + amount; col++) {
			matrix.values[row][col] = this.values[row - amount][col - amount]; 
		}}
		
		return matrix;
	}

	public Matrix subMatrix(int x, int y, int width, int height) {
		Matrix toRet = new Matrix(height, width);
		
		for(int row = y; row < y + height; row++) {
		for(int col = x; col < x + width; col++) {
			toRet.values[row - y][col - x] = values[row][col];
		}}
		
		return toRet;
	}
	
	public Matrix forEach(Function<Double, Double> function) {
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			values[row][col] = function.apply(values[row][col]);
		}}
		
		return this;
	}
	
	public Matrix immutableForEach(Function<Double, Double> function) {
		Matrix toRet = new Matrix(nRows, nCols);
		
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			toRet.values[row][col] = function.apply(values[row][col]);
		}}
		
		return toRet;
	}
	
	public Matrix scale(double scalar) {
		return forEach(x -> scalar * x);
	}
	
	public Matrix immutableScale(double scalar) {
		return immutableForEach(x -> scalar * x);
	}

	public Matrix clear() {
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			values[row][col] = 0;
		}}
		
		return this;
	}
	
	public double mostPositive() {
		double value = 0.0;
		
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(values[row][col] > value)
				value = values[row][col];
		}}
		
		return value;
	}
	
	public double mostNegative() {
		double value = 0.0;
		
		for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(values[row][col] < value)
				value = values[row][col];
		}}
		
		return value;
	}
	
	public String toString() {
		StringBuilder stringBuilder = new StringBuilder();
		for(int i = 0; i < values.length; i++)
			stringBuilder.append(Arrays.toString(values[i]) + "\n");
		
		return stringBuilder.toString();
	}
	
	public double get(int row, int col) { return values[row][col]; }
	
	public double[] getRow(int row) {
		return values[row];
	}
	
	public double[] getColumn(int column) {
		double[] toRet = new double[values.length];
		
		for(int i = 0; i < toRet.length; i++) {
			toRet[i] = values[i][column];
		}
		
		return toRet;
	}
	
	public double[][] getValues() { return values; }
	
	public int getNRows() { return nRows; }
	public int getNCols() { return nCols; }
	
	public static void main(String[] args) {
		Matrix matrix = new Matrix(new double[][] {
			{5,4,3},
			{4,0,4},
			{7,10,3}
		});
		
		System.out.println(matrix.transpose());
		
//		Matrix matrix1 = new Matrix(new double[][] {
//			{-1, 2, 3},
//			{4, 0, 5}
//		});
//		
//		Matrix matrix2 = new Matrix(new double[][] {
//			{5, -1},
//			{-4, 0},
//			{2, 3}
//		});
//		
//		System.out.println(matrix2.multiply(matrix1));
		
		
		
//		double sum = 0.0;
//		
//		Matrix matrix1 = new Matrix(new double[][] {
//			{0,0,0,0,0,0,0},
//			{0,1,1,1,2,1,0},
//			{0,0,2,2,2,2,0},
//			{0,2,0,1,2,0,0},
//			{0,2,0,2,0,0,0},
//			{0,1,0,1,0,0,0},
//			{0,0,0,0,0,0,0}
//		});
//		
//		Matrix filter1 = new Matrix(new double[][] {
//			{-1, 0, 1},
//			{-1, 1, -1},
//			{-1, -1, 0}
//		});
//		
//		sum += matrix1.elementWiseProduct(filter1, 2, 0);
//		
//		Matrix matrix2 = new Matrix(new double[][] {
//			{0,0,0,0,0,0,0},
//			{0,1,1,2,2,0,0},
//			{0,2,2,1,0,2,0},
//			{0,2,0,2,1,2,0},
//			{0,2,1,1,1,0,0},
//			{0,1,1,1,0,1,0},
//			{0,0,0,0,0,0,0}
//		});
//		
//		Matrix filter2 = new Matrix(new double[][] {
//			{0, -1, -1},
//			{-1, 0, 0},
//			{-1, 1, 0}
//		});
//		
//		sum += matrix2.elementWiseProduct(filter2, 2, 0);
//		
//		Matrix matrix3 = new Matrix(new double[][] {
//			{0,0,0,0,0,0,0},
//			{0,0,2,1,1,1,0},
//			{0,2,0,2,2,1,0},
//			{0,0,2,2,1,1,0},
//			{0,0,1,2,1,1,0},
//			{0,1,2,2,2,2,0},
//			{0,0,0,0,0,0,0}
//		});
//		
//		Matrix filter3 = new Matrix(new double[][] {
//			{1, -1, 0},
//			{0, -1, 1},
//			{-1, -1, 1}
//		});
//		
//		sum += matrix3.elementWiseProduct(filter3, 2, 0);
//		
//		System.out.println(sum);
	}
}
