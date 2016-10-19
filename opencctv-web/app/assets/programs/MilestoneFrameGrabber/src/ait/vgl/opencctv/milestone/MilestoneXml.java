package ait.vgl.opencctv.milestone;

import javax.xml.transform.OutputKeys;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import ait.vgl.opencctv.Xml;

public class MilestoneXml extends Xml {

	public MilestoneXml() {
		super();
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "no");
	}

	public String getConnectMessage(int requestId, String username,
			String password, String cameraId, String width, String height,
			String keepAspectRatio, String allowUpsizing) {
		Document doc = createNewDoc();
		String msg = null;
		if (doc != null) {
			Element rootEle = doc.createElement("methodcall");
			doc.appendChild(rootEle);
			rootEle.appendChild(createNewElement(doc, "requestid",
					String.valueOf(requestId)));
			rootEle.appendChild(createNewElement(doc, "methodname", "connect"));
			rootEle.appendChild(createNewElement(doc, "username", username));
			rootEle.appendChild(createNewElement(doc, "password", password));
			rootEle.appendChild(createNewElement(doc, "cameraid", cameraId));
			rootEle.appendChild(createNewElement(doc, "alwaysstdjpeg", "yes"));

			Element transcodeEle = doc.createElement("transcode");
			rootEle.appendChild(transcodeEle);
			transcodeEle.appendChild(createNewElement(doc, "width", width));
			transcodeEle.appendChild(createNewElement(doc, "height", height));
			transcodeEle.appendChild(createNewElement(doc, "keepaspectratio",
					keepAspectRatio));
			transcodeEle.appendChild(createNewElement(doc, "allowupsizing",
					allowUpsizing));
			msg = addMilestoneMessageEnding(toXmlString(doc));
		}
		return msg;
	}

	public String getConnectMessage(int requestId, String username,
			String password, String cameraId) {
		Document doc = createNewDoc();
		String msg = null;
		if (doc != null) {
			Element rootEle = doc.createElement("methodcall");
			doc.appendChild(rootEle);
			rootEle.appendChild(createNewElement(doc, "requestid",
					String.valueOf(requestId)));
			rootEle.appendChild(createNewElement(doc, "methodname", "connect"));
			rootEle.appendChild(createNewElement(doc, "username", username));
			rootEle.appendChild(createNewElement(doc, "password", password));
			rootEle.appendChild(createNewElement(doc, "cameraid", cameraId));
			rootEle.appendChild(createNewElement(doc, "alwaysstdjpeg", "yes"));
			msg = addMilestoneMessageEnding(toXmlString(doc));
		}
		return msg;
	}

	public String getLiveMessage(int requestId, String compressionRate) {
		Document doc = createNewDoc();
		String msg = null;
		if (doc != null) {
			Element rootEle = doc.createElement("methodcall");
			doc.appendChild(rootEle);
			rootEle.appendChild(createNewElement(doc, "requestid",
					String.valueOf(requestId)));
			rootEle.appendChild(createNewElement(doc, "methodname", "live"));
			rootEle.appendChild(createNewElement(doc, "compressionrate",
					compressionRate));
			msg = addMilestoneMessageEnding(toXmlString(doc));
		}
		return msg;
	}
	
	public String getLiveMessage(int requestId) {
		Document doc = createNewDoc();
		String msg = null;
		if (doc != null) {
			Element rootEle = doc.createElement("methodcall");
			doc.appendChild(rootEle);
			rootEle.appendChild(createNewElement(doc, "requestid",
					String.valueOf(requestId)));
			rootEle.appendChild(createNewElement(doc, "methodname", "live"));
			msg = addMilestoneMessageEnding(toXmlString(doc));
		}
		return msg;
	}

	private String addMilestoneMessageEnding(String msg) {
		StringBuilder str = new StringBuilder(msg.trim());
		str.append("\r\n\r\n");
		return str.toString();
	}
}
