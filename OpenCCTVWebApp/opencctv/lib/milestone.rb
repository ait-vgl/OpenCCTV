require 'net/http'
require 'uri'
require 'nokogiri'

class Milestone

  # validates a Milestone VMS
  # returns (true, systeminfo_xml_string) if valid or (false, error_msg) if invalid
  def self.validate_vms(vms)
    response = connect_to_milestone_enterprise(vms.server_name, vms.server_port, vms.username, vms.password)
    if response.nil? || response.code == "404" # if a enterprise server cannot be found
      response = connect_to_milestone_corporate(vms.server_name, vms.server_port, vms.username, vms.password)
    end
    error_msg = "Error."
    if !response.nil?
      if response.code == "200" # HTTP OK
        return true, response.body
      elsif response.code == "404" # HTTP not found
        error_msg = "Invalid Server name or Server port"
      elsif response.code == "401" # HTTP unauthorized
        error_msg = "Invalid Username or Password"
      else
        error_msg = "We could not connect to the Milestone VMS. HTTP error: #{response.message}"
      end
    end
    return false, error_msg
  end

  # Gets an array of Cameras when systeminfo.xml is given
  # returns an array of Cameras
  def self.get_cameras(systeminfo_xml_string)
    cameras = Array.new
    doc = Nokogiri::XML(systeminfo_xml_string)
    doc.xpath("//camera").each do |cam_ele|
      camera = Camera.new
      camera.name = cam_ele.attribute("cameraid").to_s
      camera.camera_id = cam_ele.xpath("guid")[0].content
      cameras.push(camera)
    end
    return cameras
  end

  # validates a given Camera
  # returns true if valid or false if invalid
  def self.validate_camera(camera)
    xml_builder = Nokogiri::XML::Builder.new do |xml|
      xml.send(:'methodcall') {
        xml.send(:'requestid', '1')
        xml.send(:'methodname', 'connect')
        xml.send(:'username', camera.vms.username)
        xml.send(:'password', camera.vms.password)
        xml.send(:'cameraid', camera.camera_id)
        xml.send(:'alwaysstdjpeg', 'yes')
      }
    end
    return send_receive_validate_connection(camera.vms, xml_builder)
  end

  # validates a given Stream
  # returns true if valid or false if invalid
  def self.validate_stream(stream)
    xml_builder = Nokogiri::XML::Builder.new do |xml|
      xml.send(:'methodcall') {
        xml.send(:'requestid', '1')
        xml.send(:'methodname', 'connect')
        xml.send(:'username', stream.camera.vms.username)
        xml.send(:'password', stream.camera.vms.password)
        xml.send(:'cameraid', stream.camera.camera_id)
        xml.send(:'alwaysstdjpeg', 'yes')
        xml.send(:'transcode') {
          xml.send(:'width', stream.width)
          xml.send(:'height', stream.height)
          if stream.keep_aspect_ratio
            xml.send(:'keepaspectratio', 'yes')
          else
            xml.send(:'keepaspectratio', 'no')
          end
          if stream.allow_upsizing
            xml.send(:'allowupsizing', 'yes')
          else
            xml.send(:'allowupsizing', 'no')
          end
        }
      }
    end
    return send_receive_validate_connection(stream.camera.vms, xml_builder)
  end

  # grabs an image frame from a given Stream and save it in assets/images dir with a given filename
  # returns true if successful or false if could not grab a frame
  def self.grab_stream_frame(stream, img_filename)
    if(stream.errors.any?)
      return false
    end
    complete = false
    aspect = "no"
    upsizing = "no"
    if(stream.keep_aspect_ratio)
      aspect = "yes"
    end
    if(stream.allow_upsizing)
      upsizing = "yes"
    end
    img_path = "#{Rails.root}/app/assets/images/#{img_filename}"
    cmd = "java -jar #{Rails.root}/app/assets/programs/grab_stream_frame.jar #{stream.camera.vms.server_name} #{stream.camera.vms.server_port} #{stream.camera.vms.username} #{stream.camera.vms.password} #{stream.camera.camera_id} #{stream.width} #{stream.height} #{aspect} #{upsizing} #{stream.compression_rate} #{img_path}"
    stdin, stdout, stderr = Open3.popen3(cmd)
    output = stdout.gets
    if(output.start_with?("<"))
      doc = Nokogiri::XML(output)
      error = doc.xpath('//output//error')[0].content
      if(error == "no")
        complete = true
      end
    end
    return complete
  end

  private

  def self.connect_to_milestone_enterprise(server_name, server_port, username, password)
    url = "http://#{server_name}/systeminfo.xml"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  def self.connect_to_milestone_corporate(server_name, server_port, username, password)
    url = "http://#{server_name}/rcserver/systeminfo.xml"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  def self.connect_with_basic_auth(url, port_number, username, password)
    uri = URI.parse(url)
    http = Net::HTTP.new(uri.host, port_number)
    http.open_timeout = 5
    http.read_timeout = 5
    request = Net::HTTP::Get.new(uri.request_uri)
    request.basic_auth(username, password)
    begin
      return true, http.request(request)
    rescue
      return false, "Something went wrong. It could be an error in your inputs."
    end

    def self.send_receive_validate_connection(vms, connect_xml_builder)
      if(vms.errors.any?)
        return false
      end
      msg = connect_xml_builder.to_xml.to_s + "\r\n\r\n"
      sock = TCPSocket.new( vms.server_name, vms.server_port )
      sock.write( msg )
      reply = sock.recv( 2048 )
      if(reply.start_with?("<"))
        doc = Nokogiri::XML(reply)
        connected = doc.xpath('//methodresponse//connected')[0].content
        if(connected == "yes")
          return true
        end
      end
      return false
    end

  end
end