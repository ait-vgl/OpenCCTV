package ait.vgl.opencctv;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;

public class MjpegDecoder {
	private InputStream s_in;
	private ByteArrayOutputStream jpgOut;
	private byte[] curFrame;
	private int prev, cur;
	private int width, height;

	private void init() {
		jpgOut = null;
		curFrame = new byte[0];
		prev = -1;
		cur = -1;
		width = -1;
		height = -1;
	}

	public MjpegDecoder(InputStream tcpSocketInputStream) {
		s_in = tcpSocketInputStream;
		init();
	}

	public MjpegDecoder() {
		s_in = null;
		init();
	}

	public void setTcpSocketReader(InputStream tcpSocketInputStream) {
		s_in = tcpSocketInputStream;
	}

	public boolean decodeFrame() {
		try {
			while ((cur = s_in.read()) >= 0) {
				if (prev == 0xFF && cur == 0xD8) { // JPEG starts with 0xFF,
													// 0xD8 sequence
					jpgOut = new ByteArrayOutputStream();
					jpgOut.write((byte) prev);
					jpgOut.write((byte) cur);
				} else if (jpgOut != null) {
					jpgOut.write((byte) cur);
					if (prev == 0xFF && cur == 0xD9) { // JPEG ends with 0xFF,
														// 0xD9 sequence
						synchronized (curFrame) { // don't write to curFrame
													// now!
							curFrame = jpgOut.toByteArray();
						}
						jpgOut.close();
						jpgOut = null;
						ByteArrayInputStream jpgIn = new ByteArrayInputStream(curFrame);
						BufferedImage bufImg = null;
						try {
							bufImg = ImageIO.read(jpgIn);
						} catch (IOException e) {
						}
						try {
							jpgIn.close();
						} catch (IOException e) {
						}
						if(bufImg != null)
						{
							width = bufImg.getWidth();
							height = bufImg.getHeight();
						}
						return true;
					}
				}
				prev = cur;
			}
		} catch (IOException e) {
		}
		return false;
	}

	public void saveImage(String path) {
		FileOutputStream stream = null;
		try {
			stream = new FileOutputStream(path);
		} catch (FileNotFoundException e) {
		}
		try {
			stream.write(curFrame);
			stream.close();
		} catch (IOException e) {
		}
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}
}
