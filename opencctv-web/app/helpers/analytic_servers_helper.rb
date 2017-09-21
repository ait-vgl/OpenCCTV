module AnalyticServersHelper

  def update_analytic_server_status(analytic_server)
    message = "<?xml version=\"1.0\" encoding=\"utf-8\"?>
               <analyticrequest>
               <operation>AnalyticServerStatus</operation>
               </analyticrequest>"
    reply = parse_status_reply(send_to_analytic_server(analytic_server, message))
    analytic_server.update(status: reply[:server_status], pid: reply[:server_pid])
    return reply
  end

  def update_analytic_inst_status(analytic_server)
    message = "<?xml version=\"1.0\" encoding=\"utf-8\"?>
               <analyticrequest>
               <operation>AnalyticInstStatus</operation>
               </analyticrequest>"
    reply = parse_status_reply(send_to_analytic_server(analytic_server, message))
    analytic_server.update(status: reply[:server_status], pid: reply[:server_pid])
    return reply
  end

  def start_analytic_inst(analytic_server, message)
    reply = parse_reply(send_to_analytic_server(analytic_server, message))
    return reply
  end

  def stop_analytic_inst(analytic_server, message)
    reply = parse_reply(send_to_analytic_server(analytic_server, message))
    return reply
  end



  private
  def send_to_analytic_server(analytic_server, message)
    msg_details = nil
    reply = ""
    error_reply = "<?xml version=\"1.0\" encoding=\"utf-8\"?><analyticreply><type>Error</type>
                   <content>Communication failure with analytic server at #{analytic_server.ip}:#{analytic_server.port}</content>
                   <serverstatus>Unknown</serverstatus>
                   <serverpid>0</serverpid></analyticreply>"
    context = nil
    requester = nil

    begin
      context = ZMQ::Context.new(1)

      if context
        #Create a request type socket
        requester = context.socket(ZMQ::REQ)

        #Set the socket options
        requester.setsockopt(ZMQ::SNDTIMEO, 10000) # A 10 second timeout is set for send
        requester.setsockopt(ZMQ::RCVTIMEO, 10000) # A 10 second timeout is set for receive
        requester.setsockopt(ZMQ::LINGER,0)

        #Connects to the socket
        rc = requester.connect("tcp://#{analytic_server.ip}:#{analytic_server.port}")

        #Send a request
        rc = requester.send_string(message) unless zmq_error_check(rc)

        #Receive reply
        rc = requester.recv_string(reply) unless zmq_error_check(rc)
      end
    rescue StandardError=>e
      reply = error_reply
    ensure
      #Close the socket and terminate the ZMQ context
      requester.close unless requester.nil?
      context.terminate
    end

    if reply.blank?
      reply = error_reply;
    end

    #reply.blank? ? msg_details = parse_reply(error_reply) : msg_details = parse_reply(reply)
    #return msg_details

    return reply;
  end

  def parse_status_reply (xml_string)
    #Example reply messages :
    #successfull : <?xml version="1.0" encoding="utf-8"?><analyticreply><type>StartAnalytic</type><content>Analytic instance 4 started</content><serverstatus>Running</serverstatus><serverpid>17287</serverpid></analyticreply>
    #failed : <?xml version="1.0" encoding="utf-8"?><analyticreply><type>Error</type><content>Failed to start analytic instance 4</content><serverstatus>Running</serverstatus><serverpid>17287</serverpid></analyticreply>
    msg_type = nil
    msg_content = nil
    server_status = nil
    server_pid = nil

    msg_details = {}
    if(!xml_string.nil? && xml_string.start_with?("<"))
      begin # XML parsing
        doc = Nokogiri::XML(xml_string)
        msg_type = doc.xpath('//analyticreply//type')[0].content.to_s.strip
        msg_content = doc.xpath('//analyticreply//content')[0].content.to_s.strip
        server_status = doc.xpath('//analyticreply//serverstatus')[0].content.to_s.strip
        server_pid = doc.xpath('//analyticreply//serverpid')[0].content.to_s.strip
      rescue # Handle XML parsing errors
        msg_type = 'Error' if msg_type.nil?
        msg_content = 'Error occured parsing the analytic server reply!' if msg_content.nil?
        server_status = 'Unknown' if server_status.nil?
        server_pid = '0' if server_pid.nil?
      end # End XML parsing
    end

    msg_details[:msg_type] = msg_type
    msg_details[:msg_content] = msg_content
    msg_details[:server_status] = server_status
    msg_details[:server_pid] = server_pid

    return msg_details
  end

  # USED BY START ANALYTIC INSTANCE
  def parse_reply (xml_string)
    #Example reply messages :
    #successfull : <?xml version="1.0" encoding="utf-8"?><analyticreply><type>StartAnalytic</type><content>Analytic instance 4 started</content><serverstatus>Running</serverstatus><serverpid>17287</serverpid></analyticreply>
    #failed : <?xml version="1.0" encoding="utf-8"?><analyticreply><type>Error</type><content>Failed to start analytic instance 4</content><serverstatus>Running</serverstatus><serverpid>17287</serverpid></analyticreply>
    msg_type = nil
    msg_content = nil
    server_status = nil
    server_pid = nil

    puts "=============Parsing message #{xml_string}=================="

    msg_details = {}
    if(!xml_string.nil? && xml_string.start_with?("<"))
      begin # XML parsing
        doc = Nokogiri::XML(xml_string)
        msg_type = doc.xpath('//analyticreply//type')[0].content.to_s.strip
        msg_content = doc.xpath('//analyticreply//content')[0].content.to_s.strip
        server_status = doc.xpath('//analyticreply//serverstatus')[0].content.to_s.strip
        server_pid = doc.xpath('//analyticreply//serverpid')[0].content.to_s.strip
      rescue # Handle XML parsing errors
        msg_type = 'Error' if msg_type.nil?
        msg_content = 'Error occured parsing the analytic server reply!' if msg_content.nil?
        server_status = 'Unknown' if server_status.nil?
        server_pid = '0' if server_pid.nil?
      end # End XML parsing
    end

    msg_details[:msg_type] = msg_type
    msg_details[:msg_content] = msg_content
    msg_details[:server_status] = server_status
    msg_details[:server_pid] = server_pid

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
