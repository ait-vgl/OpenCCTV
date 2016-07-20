package ait.vgl.opencctv;

import javax.xml.transform.OutputKeys;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class OutputXml extends Xml
{
	public OutputXml() {
		super();
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
	}
	
	public String getOutput(boolean hasError, String message, int width, int height)
	{
		Document doc = createNewDoc();
		String output = null;
		if(doc != null)
		{
			Element rootEle = doc.createElement("output");
			doc.appendChild(rootEle);
			if(hasError)
				rootEle.appendChild(createNewElement(doc, "error", "yes"));
			else
				rootEle.appendChild(createNewElement(doc, "error", "no"));
			rootEle.appendChild(createNewElement(doc, "message", message));
			rootEle.appendChild(createNewElement(doc, "width", String.valueOf(width)));
			rootEle.appendChild(createNewElement(doc, "height", String.valueOf(height)));
			output = addOutputEnding(toXmlString(doc));
		}
		return output;
	}
	
	private String addOutputEnding(String msg)
	{
		StringBuilder str = new StringBuilder(msg.trim());
		str.append("\n");
		return str.toString();
	}
}
