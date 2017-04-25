class Result < ActiveRecord::Base

  @@context = nil
  @@requester = nil

  def init_zmq
    @@context = ZMQ::Context.new(1)
    @@requester = @@context.socket(ZMQ::PUSH)
    @@requester.setsockopt(ZMQ::SNDTIMEO, 5000) # A 5 second timeout is set for send
    @@requester.setsockopt(ZMQ::RCVTIMEO, 5000) # A 5 second timeout is set for receive
    @@requester.setsockopt(ZMQ::LINGER,0)
    @@requester.connect("tcp://#{face_images_grabber_ip}:#{face_images_grabber_port}")
  end

  def face_images_grabber_ip
    return '127.0.0.1'
  end

  def face_images_grabber_port
    return 10000
  end

  def hikvision_ip
    return '203.159.18.213'
  end

  # camera_ids Hash[<analytic instance id> => <camera id>]
  def camera_ids
    return Hash[1 => '101', 2 => '201', 3 => '201']
  end

  def username
    return 'csim'
  end

  def password
    return 'csimcctv2015'
  end

  def get_rtsp_uri
    if(!self.time.nil?)
      start_time = (self.time - 3.seconds).strftime("%Y%m%dT%H%M%SZ")
      end_time = (self.time + 3.seconds).strftime("%Y%m%dT%H%M%SZ")
      return "rtsp://#{username}:#{password}@#{hikvision_ip}:554/ISAPI/Streaming/tracks/#{camera_ids[self.analytic_instance_id]}?starttime=#{start_time}\\&endtime=#{end_time}"
    end
  end

  def save_images
    cmd = "#{Rails.root}/app/assets/programs/FaceDetectAnalyticResults/Release/FaceDetectAnalyticResults " +
        "#{self.id} #{Rails.root}/app/assets/images/"
    stdin, stdout, stderr = Open3.popen3(cmd)
    stdout.readline
  end

  def send_save_images_request
    if(@@context.nil?)
      init_zmq
    end
      #Connects to the socket
      # rc = @@requester.connect("tcp://#{face_images_grabber_ip}:#{face_images_grabber_port}")
      message = "<request><id>#{self.id}</id><cameraid>#{self.camera_ids[self.analytic_instance_id]}</cameraid><dstdirpath>#{Rails.root}/app/assets/images/#{self.timestamp}/</dstdirpath></request>"
      #Send a request
      @@requester.send_string(message)
    puts "Save images request for #{self.id} sent."

    # end
    #Close the socket and terminate the ZMQ context
    # requester.close unless requester.nil?
    # context.terminate
  end

  # def zmq_error_check(rc)
  #   if ZMQ::Util.resultcode_ok?(rc)
  #     false
  #   else
  #     STDERR.puts "Operation failed, errno [#{ZMQ::Util.errno}] description [#{ZMQ::Util.error_string}]"
  #     true
  #   end
  # end

end
