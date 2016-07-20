package ait.vgl.opencctv;

import java.io.StringWriter;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class Xml
{
	protected DocumentBuilderFactory docFactory;
	protected Transformer transformer;
	
	public Xml() {
		docFactory = DocumentBuilderFactory.newInstance();
		transformer = null;
		try {
			transformer = TransformerFactory.newInstance().newTransformer();
		} catch (TransformerConfigurationException e) {
		}
	}
	
	public Document createNewDoc() {
		Document doc = null;
		try {
			DocumentBuilder db = docFactory.newDocumentBuilder();
			doc = db.newDocument();
		} catch (ParserConfigurationException pce) {
		}
		return doc;
	}
	
	public Element createNewElement(Document doc, String name, String value)
	{
		Element ele = doc.createElement(name);
		ele.appendChild(doc.createTextNode(value));
		return ele;
	}
	
	public String toXmlString(Document doc) {
		if (transformer != null) {
			StringWriter writer = new StringWriter();
			try {
				transformer.transform(new DOMSource(doc), new StreamResult(
						writer));
			} catch (TransformerException e) {
				return null;
			}
			String output = writer.getBuffer().toString();
			return output;
		}
		return null;
	}
}
