package ait.vgl.opencctv.milestone;

import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;

import ait.vgl.opencctv.MjpegDecoder;
import ait.vgl.opencctv.OutputXml;

public class GrabStreamFrame {
	public static void main(String[] args) {
		boolean hasError = false;
		String message = "";
		int width = -1;
		int height = -1;
		if (args.length == 11 || args.length == 6) {
			final int SERVER_INDEX = 0;
			final int PORT_INDEX = 1;
			final int USER_INDEX = 2;
			final int PASS_INDEX = 3;
			final int CAM_INDEX = 4;
			final int WIDTH_INDEX = 5;
			final int HEIGHT_INDEX = 6;
			final int ASPECT_RATIO_INDEX = 7;
			final int UPSIZING_INDEX = 8;
			final int COMPRESS_INDEX = 9;
			final int PATH_INDEX = 10;
			final int DEFAULT_PATH_INDEX = 5;
			
			PrintWriter s_out = null;
			InputStream s_in = null;
			Socket s = new Socket();
			try {
				s.connect(new InetSocketAddress(args[SERVER_INDEX], Integer.parseInt(args[PORT_INDEX])));
				// writer for socket
				s_out = new PrintWriter(s.getOutputStream(), true);
				// reader for socket
				s_in = s.getInputStream();
			} catch (Exception e) {
				hasError = true;
				message = "Cannot connect to the Milestone VMS";
				System.exit(-1);
			}
			MilestoneXml xml = new MilestoneXml();
			if (args.length == 11) {
				String msg = xml.getConnectMessage(1,
						args[USER_INDEX], args[PASS_INDEX], args[CAM_INDEX], 
						args[WIDTH_INDEX], args[HEIGHT_INDEX], args[ASPECT_RATIO_INDEX], args[UPSIZING_INDEX]);
				s_out.printf(msg);
				s_out.printf(xml.getLiveMessage(2, args[COMPRESS_INDEX]));
			} else if (args.length == 6) {
				String msg = xml.getConnectMessage(1,
						args[USER_INDEX], args[PASS_INDEX], args[CAM_INDEX]);
				s_out.printf(msg);
				s_out.printf(xml.getLiveMessage(2));
			}
			MjpegDecoder decoder = new MjpegDecoder(s_in);
			if(decoder.decodeFrame())
			{
				if(args.length == 6)
				{
					decoder.saveImage(args[DEFAULT_PATH_INDEX]);
				}
				else
				{
					decoder.saveImage(args[PATH_INDEX]);
				}
				width = decoder.getWidth();
				height = decoder.getHeight();
				message = "Successful";
				try {
					s.close(); // close socket
				} catch (IOException e) {
				}
			}
		} else {
			hasError = true;
			message = "Missing one or more input arguments";
		}
		OutputXml out = new OutputXml();
		System.out.println(out.getOutput(hasError, message, width, height));
	}
}
