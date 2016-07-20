package ait.vgl.opencctv.zoneminder;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

import ait.vgl.opencctv.MjpegDecoder;
import ait.vgl.opencctv.OutputXml;

public class GrabStreamFrame {
	public static void main(String[] args) {
		boolean hasError = false;
		String message = "";
		int width = -1;
		int height = -1;
		if (args.length == 2) {
			final int STREAM_URL_INDEX = 0;
			final int SAVE_PATH_INDEX = 1;
			try
			{
				String rawData = "";
				String type = "application/x-www-form-urlencoded";
				String encodedData = URLEncoder.encode(rawData);
				URL u = new URL(args[STREAM_URL_INDEX]);
				HttpURLConnection conn = (HttpURLConnection) u.openConnection();
				conn.setDoOutput(true);
				conn.setRequestMethod("POST");
				conn.setRequestProperty("Content-Type", type);
				conn.setRequestProperty("Content-Length", String.valueOf(encodedData.length()));
				OutputStream os = conn.getOutputStream();
				InputStream is = conn.getInputStream();
				os.write(encodedData.getBytes());
				MjpegDecoder decoder = new MjpegDecoder(is);
				if (decoder.decodeFrame()) {
					decoder.saveImage(args[SAVE_PATH_INDEX]);
					width = decoder.getWidth();
					height = decoder.getHeight();
					message = "Successful";
				}
			}
			catch(Exception e)
			{
				hasError = true;
				message = "Cannot connect to the Zoneminder VMS";
				System.exit(-1);
			}
		}
		else {
			hasError = true;
			message = "Missing one or more input arguments";
		}
		OutputXml out = new OutputXml();
		System.out.println(out.getOutput(hasError, message, width, height));
	}
}
