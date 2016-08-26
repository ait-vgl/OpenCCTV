module OpenCctvServersHelper

  def sendToServerWithData(openCctvServer,message_type,data)

    puts openCctvServer.port
    puts openCctvServer.host

    msg_details = nil
    reply = nil
    error_reply = reply = "<?xml version=\"1.0\" encoding=\"utf-8\"?><opencctvmsg><opencctvmsg><type>Error</type><content>Connecting to OpenCCTV Sever on #{openCctvServer.host}:#{openCctvServer.port} Failed</content><serverstatus>Unknown</serverstatus><serverpid>0</serverpid></opencctvmsg>"

    context = ZMQ::Context.new(1)
    requester = nil

    if context
      #Create a request type socket
      requester = context.socket(ZMQ::REQ)

      #Set the socket optionst
      requester.setsockopt(ZMQ::SNDTIMEO, 10000) # A 10 second timeout is set for send
      requester.setsockopt(ZMQ::RCVTIMEO, 10000) # A 10 second timeout is set for receive
      requester.setsockopt(ZMQ::LINGER,0)

      #Connects to the socket
      rc = requester.connect("tcp://#{openCctvServer.host}:#{openCctvServer.port}")
      message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><opencctvmsg><type>#{message_type}</type><data>#{data}</data></opencctvmsg>"

      #Send a request
      rc = requester.send_string(message) unless zmq_error_check(rc)

      #Receive reply
      rc = requester.recv_string(reply) unless zmq_error_check(rc)
    end

    #Close the socket and terminate the ZMQ context
    requester.close unless requester.nil?
    context.terminate

    puts reply

    #reply.nil? ? msg_details = parse_reply(error_reply) : msg_details = parse_reply(reply)
    #return msg_details

  end


  private
  def zmq_error_check(rc)
    if ZMQ::Util.resultcode_ok?(rc)
      false
    else
      STDERR.puts "Operation failed, errno [#{ZMQ::Util.errno}] description [#{ZMQ::Util.error_string}]"
      true
    end
  end


end
