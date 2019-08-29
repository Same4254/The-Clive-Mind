package ColorImages;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class ColorTrainingTest {
	public static BufferedImage loadImage(String path) {
		BufferedImage toRet = null;
		
		try {
			toRet = ImageIO.read(new File(path));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return toRet;
	}
	
	public static void main(String[] args) {
		BufferedImage color = loadImage(System.getProperty("user.dir") + "/Color.jpg");
		BufferedImage gray = loadImage(System.getProperty("user.dir") + "/BlackAndWhite.jpg");
		
		Color colorTest = new Color(color.getRGB(200, 200)), grayTest = new Color(gray.getRGB(200, 200));
	}
}
