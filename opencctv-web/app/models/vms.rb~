class Vms < ActiveRecord::Base
  belongs_to :vms_connector
  has_many :cameras, dependent: :destroy

  belongs_to :user
  belongs_to :group_user

  #has_many :group_cans, dependent: :destroy
  #has_many :group_users, through: :group_cans

  validates :server_ip, presence: true
  validates :server_port, presence: true
  validates :vms_connector_id, presence: true
  validates :vms_type, presence: true

  ## Public methods start

  # validates all VMS types and update verified column of table if VMS is valid
  def validate_connection
    valid = false
    if (self.vms_type == 'milestone')
      valid, xml_response = milestone_validate_vms
    elsif (self.vms_type == 'zoneminder')
      valid = zoneminder_validate_vms
    elsif (self.vms_type == 'hikvision')
      valid = hikvision_validate_device
    elsif (self.vms_type =='lilin')
      valid = lilin_validate_vms
    else # anything else
      valid = true
    end
    if (valid)
      self.update(:verified => true)
    end
    return valid
  end

  # adds cameras and streams to the vms
  def add_cameras
    if (self.vms_type == 'milestone')
      is_vms_valid, xml_response = milestone_validate_vms
      if (is_vms_valid)
        return milestone_add_cameras(xml_response)
      end
    elsif (self.vms_type == 'zoneminder')
      url = create_zoneminder_url(self.server_ip, self.server_port)
      cookie = zoneminder_login(url, self.username, self.password)
      if (!cookie.nil?)
        zoneminder_add_monitors(url, cookie)
        zoneminder_add_streams(url, cookie, self.cameras)
      end
    elsif (self.vms_type == 'hikvision')
      hikvision_add_monitors
    elsif (self.vms_type == 'lilin')

      response = connect_to_lilin_vms(self.server_ip, self.server_port, self.username, self.password)

      if (validate_http_response(response))
        lilin_add_monitors(response)
      else
        raise 'Failed to connect to Lilin server.'
      end

    else
    end
  end

  def set_verification(verification)
    self.update(:verified => verification)
    self.cameras.each do |camera|
      camera.set_verification(verification)
    end
  end

  ## Public methods end

  # Private methods start

  private
  # validates a Milestone VMS
  # returns (true, systeminfo_xml_string) if valid or (false, error_msg) if invalid
  def milestone_validate_vms
    response = connect_to_milestone_enterprise(self.server_ip, self.server_port, self.username, self.password)
    if response.nil? || response.code == '404' # if a enterprise server cannot be found
      response = connect_to_milestone_corporate(self.server_ip, self.server_port, self.username, self.password)
    end
    error_msg = nil
    if !response.nil?
      if response.code == '200' # HTTP OK
        return true, response.body
      elsif response.code == '404' # HTTP not found
        error_msg = 'Failed to connect to Milestone XProtect server. Invalid Server IP or Port.'
      elsif response.code == '401' # HTTP unauthorized
        error_msg = 'Failed to connect to Milestone XProtect server. Invalid Username or Password.'
      else
        error_msg = "We could not connect to the Milestone VMS. HTTP error: #{response.message}"
      end
    else
      error_msg = 'Failed to connect to Milestone XProtect server.'
    end
    if (!error_msg.nil?)
      raise error_msg
    end
    return false, error_msg
  end

  # validates a ZoneMinder VMS
  def zoneminder_validate_vms
    url = create_zoneminder_url(self.server_ip, self.server_port)
    cookie = zoneminder_login(url, self.username, self.password)
    error_msg = nil
    if (!cookie.nil?)
      monitors = zoneminder_get_monitors(url, cookie)
      if (!monitors.nil?)
        return true
      else
        error_msg = 'Failed to retrieve Monitor information from the ZoneMinder server.'
      end
    else
      error_msg = 'Failed to connect to ZoneMinder server.'
    end
    if (!error_msg.nil?)
      raise error_msg
    end
    return false
  end

  # validates HikVision devices (DVRs, NVRs, Cameras)
  def hikvision_validate_device
    response = connect_to_hikvision_device(self.server_ip, self.server_port, self.username, self.password)
    if (validate_http_response(response))
      return true
    else
      raise 'Failed to connect to HikVision device.'
    end
  end


  def lilin_validate_vms
    response = connect_to_lilin_vms(self.server_ip, self.server_port, self.username, self.password)
    if (validate_http_response(response))
      return true
    else
      raise 'Failed to connect to Lilin server.'
    end

  end


  #### Connect to VMS or camera ####
  def connect_to_milestone_enterprise(server_name, server_port, username, password)
    url = "http://#{server_name}/systeminfo.xml"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  def connect_to_milestone_corporate(server_name, server_port, username, password)
    url = "http://#{server_name}/rcserver/systeminfo.xml"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  def connect_to_hikvision_device(server_name, server_port, username, password)
    url = "http://#{server_name}/ISAPI/Streaming/channels"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  # zoneminder login returns cookie (as an Array) after successful auth or nil if login failed
  def zoneminder_login(url, username, password)
    cookie = nil # return val
    response = Unirest.post(url, parameters: {:action => "login", :view => "postlogin", :username => username, :password => password})
    error_msg = nil
    if response.code == 200
      if response.body.include? "username" and response.body.include? "password" #again login form
      elsif (response.body.include? 'Logging In') #got auth correct
        cookie = response.headers[:set_cookie]
      else #unknown response
        error_msg = 'Failed to connect to the ZoneMinder server. Unknown response from ZoneMinder server.'
      end
    else
      error_msg = 'Failed to connect to the ZoneMinder server.'
    end
    if (!error_msg.nil?)
      raise error_msg
    end
    return cookie
  end

  def connect_to_lilin_vms(server_name, server_port, username, password)
    url = "http://#{server_name}/cmd=getgroupinfo"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end


  def connect_to_rtsp_device(server_name, server_port, username, password)
    #url = "http://#{server_name}/ISAPI/Streaming/channels"
    #return connect_with_basic_auth(url, server_port.to_i, username, password)
  end


  # HTTP with basic auth. returns HTTP respose
  def connect_with_basic_auth(url, port_number, username, password)
    uri = URI.parse(url)
    http = Net::HTTP.new(uri.host, port_number)
    http.open_timeout = 20
    http.read_timeout = 20
    request = Net::HTTP::Get.new(uri.request_uri)
    request.basic_auth(username, password)
    begin
      return http.request(request)
    rescue
      raise 'Failed to connect to the Server using provided IP and Port.'
    end
  end

  # validate HTTP response
  def validate_http_response(response)
    if (!response.nil?)
      if (response.code == '404') # host could not found
        raise 'Invalid Server IP or Server Port.'
      elsif (response.code == '200')
        return true
      elsif (response.code == '401')
        raise 'Invalid Username or Password.'
      else
        raise "Could not connect to the Server. HTTP error: #{response.message}."
      end
    end
    return false
  end

  def validate_camera_http(response)
    if (!response.nil?)
      if (response.code == '200')
        return true
      else
        return false
      end
    end
    return false
  end

  def connect_camera_with_basic_auth(url, port_number, username, password)
    uri = URI.parse(url)
    http = Net::HTTP.new(uri.host, port_number)
    http.open_timeout = 5
    http.read_timeout = 5
    request = Net::HTTP::Get.new(uri.request_uri)
    request.basic_auth(username, password)
    begin
      return http.request(request)
    rescue
      return false
    end
  end


  ### Add Camera after testing by trying to connect with the device

  # extract milestone camera info from milestone system info xml
  def milestone_add_cameras(xml_string)
    doc = Nokogiri::XML(xml_string)
    doc.xpath("//camera").each do |cam_ele|
      camera = Camera.new
      camera.name = cam_ele.attribute("cameraid").to_s
      camera.camera_id = cam_ele.xpath("guid")[0].content
      self.cameras.push(camera)
      is_default_stream_valid, default_width, default_height = camera.milestone_grab_default_stream_frame
      if (is_default_stream_valid)
        default_stream = Stream.new(:name => "Default - #{camera.name}", :width => default_width, :height => default_height, :keep_aspect_ratio => false, :allow_upsizing => false, :compression_rate => 90, :description => "Original Stream from Camera")
        camera.update(:verified => true)
        camera.streams.push(default_stream)
        if (default_stream.milestone_grab_stream_frame)
          default_stream.update(:verified => true)
        end
      end
    end
    return cameras
  end

  # zoneminder get monitors returns a HashMap<monitorName, monitorWatchPath>,
  # nil if an error occurred or empty HashMap if no monitors
  def zoneminder_add_monitors(url, cookie)
    response = Unirest.get(url, headers: {"Cookie" => cookie.join(" ")})
    if (response.code == 200)
      page = Nokogiri::HTML(response.body)
      page.css('tbody//tr//td[class="colName"]').each do |monitorInfo|
        camera = Camera.new
        camera.name = monitorInfo.inner_text
        params = monitorInfo.children[0].attribute("href").inner_text
        camera.camera_id = params.split('=')[-1] # get string after last '='
        if (!camera.name.nil? && !camera.camera_id.nil?)
          camera.verified = true
        end
        self.cameras.push(camera)
      end
    elsif (response.code == 404)
      raise 'Failed to connect to the ZoneMinder server. Invalid Server URL or Port.'
    elsif (response.code == 401)
      raise 'Failed to connect to the ZoneMinder server. Invalid Username or Password.'
    else
      raise 'Failed to connect to the ZoneMinder server.'
    end
  end

  def hikvision_add_monitors
    response = connect_to_hikvision_device(self.server_ip, self.server_port, self.username, self.password)
    doc = Nokogiri::XML(response.body) # response is an xml
    doc.remove_namespaces!
    doc.xpath('//StreamingChannelList/StreamingChannel').each do |channel|
      if (channel.xpath('enabled')[0].content.to_s == 'true' &&
          channel.xpath('//ControlProtocol/streamingTransport')[0].inner_text == 'RTSP' &&
          channel.xpath('//Video/enabled')[0].inner_text == 'true')
        camera = Camera.new
        camera.camera_id = channel.xpath('id')[0].inner_text
        camera.description = channel.xpath('channelName')[0].inner_text
        camera.name = camera.camera_id
        if (!camera.name.nil? && !camera.camera_id.nil?)
          camera.verified = true
        end
        self.cameras.push(camera)

        camera.hikvision_grab_default_frame
        stream = Stream.new
        stream.name = "Default - #{camera.name}"
        stream.width = channel.xpath('//Video/videoResolutionWidth')[0].inner_text.to_i
        stream.height = channel.xpath('//Video/videoResolutionHeight')[0].inner_text.to_i
        stream.compression_rate = channel.xpath('//Video/fixedQuality')[0].inner_text.to_i
        stream.keep_aspect_ratio = false
        stream.allow_upsizing = false
        stream.camera = camera
        stream.save
        stream.update(:verified => true)
      end
    end
  end

  def zoneminder_add_streams(url, cookie, monitors)
    monitors.each do |camera|
      monitorWatchUrl = "#{url}?view=watch&mid=#{camera.camera_id}"
      response = Unirest.post(monitorWatchUrl, headers: {"Cookie" => cookie.join(" ")})
      if (response.code == 200)
        page = Nokogiri::HTML(response.body)
        streamInfo = page.css('div#imageFeed//img#liveStream')[0]
        if (!streamInfo.nil?)
          stream = Stream.new
          stream.name = "Default - #{camera.name}"
          streamUrl = streamInfo.attribute("src").inner_text
          width = streamInfo.attribute("width").inner_text
          height = streamInfo.attribute("height").inner_text
          if (!streamUrl.nil?)
            stream.width = width.to_i
            stream.height = height.to_i
            stream.compression_rate = 100
            stream.keep_aspect_ratio = false
            stream.allow_upsizing = false
            stream.camera = camera
            stream.save
            done, new_width, new_height = stream.zoneminder_grab_frame(streamUrl)
            if (done)
              stream.update(:verified => true)
              stream.update(:width => new_width.to_i)
              stream.update(:height => new_height.to_i)
            else
              stream.update(:verified => false)
            end
          end
        else
          raise 'Failed to retrieve Monitor Streams from the Zoneminder server.'
        end
      else
        raise 'Failed to connect to the ZoneMinder server.'
      end
    end
  end

  def create_zoneminder_url(ip, port)
    if (port == 80)
      return "http://#{ip}/index.php"
    end
    return "http://#{ip}:#{port}/index.php"
  end

  def lilin_add_monitors(response)

    puts("kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk")

    result = response.body

    if !result.to_s.empty?

      #puts "Startttttttttttt"
      groupAllowMax = findValueResult(result, "groupallowmax").to_s.to_i
      puts groupAllowMax

      groupMax = findValueResult(result, "groupmax").to_s.to_i
      groupMax -= 1

      chkMaxGroup = 0
      for i in 0..groupMax

       # puts "iii  #{i}"
        #puts chkMaxGroup

        if chkMaxGroup == groupAllowMax
          break
        end

        if findValueResult(result, "group#{i}allow").to_s.to_i == 1 ## is allowed
          #puts findValueResult(result, "group#{i}chused")
          channels = findValueResult(result, "group#{i}chused").to_s
          count = 0
          channels.to_s.each_char do |channel|
            if channel == '1'

              url = "http://#{self.server_ip}/cmd=getchannelinfo&ch=#{count}"
              response = connect_with_basic_auth(url, 8080, self.username, self.password)

              if (validate_http_response(response))

                cameraResult = response.body

                puts "Camera result xxxxxxxxxxxxxxxxxxxxxxx"
                puts cameraResult

                #puts "http://192.41.170.243:8080/cmd=getchannelinfo&ch=#{count}"

                camera = Camera.new
                camera.camera_id = "getstream#{count}"
                camera.description =  "The camera channel is #{count}"
                camera.name = findValueResult(cameraResult, "name").to_s
                camera.ip = findValueResult(cameraResult, "ip").to_s
                camera.username = findValueResult(cameraResult, "user").to_s.strip
                camera.password = findValueResult(cameraResult, "pass").to_s.strip
                camera.http_port = findValueResult(cameraResult, "httpport")
                camera.video_port = findValueResult(cameraResult, "videoport")


                camera.verified = true

                self.cameras.push(camera)

                camera.lilin_grab_default_frame

                urlProfile = "http://#{camera.ip}/getprofile"
                responseProfile = connect_camera_with_basic_auth(urlProfile, camera.http_port, camera.username, camera.password)

                #puts camera.username
                #puts camera.password

                #puts urlProfile
                #puts responseProfile
                # http://192.41.170.252/getprofile

                if (responseProfile && validate_camera_http(responseProfile))
                    profileResult = responseProfile.body

                    puts "profile result xxxxxxxxxxxxxxxxxxxxxxx"
                    puts profileResult

                    #puts "jjjjjjjjjjjjjjjjjjjjjjjjj"
                    #puts  findValueResult(profileResult, "profileno_range").to_s
                  #loop with all profile
                  #

                    range = findValueResult(profileResult, "profileno_range").to_s.split(",")
                   # puts range.first
                    #puts range.last
                    #puts range.count
                    #TODO please fix when VMS have empty camera, what to do then
                    for j in 0..(range.count-1)

                      puts "Loop in profile #{j}"

                      #out of profile range
                      if(findValueResult(profileResult, "profile_0#{j}_name").to_s == "")
                        break
                      end


                      stream = Stream.new
                      stream.name = findValueResult(profileResult, "profile_0#{j}_name").to_s
                      stream.width = findValueResult(profileResult, "profile_0#{j}_res").to_s.split("x")[0]
                      stream.height = findValueResult(profileResult, "profile_0#{j}_res").to_s.split("x")[1]
                      stream.fps = findValueResult(profileResult, "profile_0#{j}_fps")


                      # 0 = H.264, 1 = JPEG
                      #TODO profile type mismatch
                      #puts "profile type"
                      #puts findValueResult(profileResult, "profile_0#{j}_type")
                      #puts (findValueResult(profileResult, "profile_0#{j}_type") == "0")

                      stream.codec = (findValueResult(profileResult, "profile_0#{j}_type") == "0") ? "H.264" : "JPEG"
                      stream.protocol = "rtsp"


                      stream.url = "rtsp://#{camera.ip}:#{camera.video_port}/rtsp#{(stream.name).downcase}"

                      stream.compression_rate = 100
                      stream.keep_aspect_ratio = false
                      stream.allow_upsizing = false
                      stream.camera = camera

                      stream.save
                      stream.update(:verified => true)
                  end
                else
                    puts "Failed to connect to camera ip #{camera.ip}, named #{camera.name}."
                end
              else
                raise 'Failed to connect to Lilin server.'
              end
            end
            count += 1

          end

          chkMaxGroup += 1
        end
      end
    end
  end

  def findValueResult(result, search)

    result.each_line do |line|
      # puts line.split('=')[0]
      #puts line.split('=')[1]
      if line.split('=')[0] == search
        return (line.split('=')[1]).split("\n")[0]
      end
    end

    return ""

  end



  # Private methods end

end
