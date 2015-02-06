class Server < ActiveRecord::Base
  validates :name, presence: true
  validates :host, presence: true
  validates :port, presence: true
  validates :username, presence: true
  validates :password, presence: true


  def send_to_server(message_type)
    context = ZMQ::Context.new(1)
    requester = context.socket(ZMQ::REQ)
    requester.connect("tcp://#{self.host}:#{self.port}")

    message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><opencctvmsg><type>#{message_type}</type></opencctvmsg>"

    requester.send_string message

    reply = ''
    requester.recv_string(reply)
    requester.close

    msg_details = parse_reply(reply);

    return msg_details

  end

  private
  def parse_reply (xml_string)
    #Example reply messages :
    #successfull : <?xml version="1.0" encoding="utf-8"?><opencctvmsg><type>StartReply</type><content>OpenCCTV Server Started</content><serverstatus>Running</serverstatus><serverpid>17287</serverpid></opencctvmsg>
    #failed : <?xml version="1.0" encoding="utf-8"?> <opencctvmsg><type>Error</type><content>Unknown message type received</content><serverstatus>Unknown</serverstatus><serverpid>0</serverpid></opencctvmsg>

    type = nil
    content = nil
    server_status = nil
    server_pid = nil

    msg_details = {}


    if(!xml_string.nil? && xml_string.start_with?("<"))
      doc = Nokogiri::XML(xml_string)
      type = doc.xpath('//opencctvmsg//type')[0].content.to_s.strip
      content = doc.xpath('//opencctvmsg//content')[0].content.to_s.strip
      server_status = doc.xpath('//opencctvmsg//serverstatus')[0].content.to_s.strip
      server_pid = doc.xpath('//opencctvmsg//serverpid')[0].content.to_s.strip

      msg_details[:type] = type
      msg_details[:content] = content
      msg_details[:server_status] = server_status
      msg_details[:server_pid] = server_pid
    end

    return msg_details

  end

end
