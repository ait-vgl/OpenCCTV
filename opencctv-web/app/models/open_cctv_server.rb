class OpenCctvServer < ActiveRecord::Base

  validates :name, presence: true
  validates :host, presence: true
  validates :port, presence: true

  def send_to_server(message_type)

    msg_details = nil
    reply = nil
    error_reply = reply = "<?xml version=\"1.0\" encoding=\"utf-8\"?><opencctvmsg><opencctvmsg><type>Error</type><content>Connecting to OpenCCTV Sever on #{self.host}:#{self.port} Failed</content><serverstatus>Unknown</serverstatus><serverpid>0</serverpid></opencctvmsg>"

    context = ZMQ::Context.new(1)
    requester = nil

    if context
      #Create a request type socket
      requester = context.socket(ZMQ::REQ)

      #Set the socket options
      requester.setsockopt(ZMQ::SNDTIMEO, 10000) # A 10 second timeout is set for send
      requester.setsockopt(ZMQ::RCVTIMEO, 10000) # A 10 second timeout is set for receive
      requester.setsockopt(ZMQ::LINGER,0)

      #Connects to the socket
      rc = requester.connect("tcp://#{self.host}:#{self.port}")
      message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><opencctvmsg><type>#{message_type}</type></opencctvmsg>"

      #Send a request
      rc = requester.send_string(message) unless zmq_error_check(rc)

      #Receive reply
      rc = requester.recv_string(reply) unless zmq_error_check(rc)
    end

    #Close the socket and terminate the ZMQ context
    requester.close unless requester.nil?
    context.terminate

    reply.nil? ? msg_details = parse_reply(error_reply) : msg_details = parse_reply(reply)
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

  def zmq_error_check(rc)
    if ZMQ::Util.resultcode_ok?(rc)
      false
    else
      STDERR.puts "Operation failed, errno [#{ZMQ::Util.errno}] description [#{ZMQ::Util.error_string}]"
      true
    end
  end

end