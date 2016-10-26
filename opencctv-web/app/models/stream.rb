class Stream < ActiveRecord::Base
  belongs_to :camera
  has_many :analytic_instance_streams, dependent: :destroy
  has_many :analytic_instances, through: :analytic_instance_streams

  validates :name, presence: true
  validates_numericality_of :compression_rate, :greater_than => 0, :less_than_or_equal_to => 100
  validates_numericality_of :width, :greater_than => 0
  validates_numericality_of :height, :greater_than => 0

  before_destroy :delete_frame

  def set_verification(is_verified)
    update(:verified => is_verified)
  end

  def milestone_validate_stream
    xml_builder = Nokogiri::XML::Builder.new do |xml|
      xml.send(:'methodcall') {
        xml.send(:'requestid', '1')
        xml.send(:'methodname', 'connect')
        xml.send(:'username', self.camera.vms.username)
        xml.send(:'password', self.camera.vms.password)
        xml.send(:'cameraid', self.camera.camera_id)
        xml.send(:'alwaysstdjpeg', 'yes')
        xml.send(:'transcode') {
          xml.send(:'width', self.width)
          xml.send(:'height', self.height)
          if self.keep_aspect_ratio
            xml.send(:'keepaspectratio', 'yes')
          else
            xml.send(:'keepaspectratio', 'no')
          end
          if self.allow_upsizing
            xml.send(:'allowupsizing', 'yes')
          else
            xml.send(:'allowupsizing', 'no')
          end
        }
      }
    end
    msg = xml_builder.to_xml.to_s + "\r\n\r\n"
    sock = TCPSocket.new( self.camera.vms.server_ip, self.camera.vms.server_port )
    sock.write( msg )
    reply = sock.recv( 2048 )
    puts reply
    if(reply.start_with?("<"))
      doc = Nokogiri::XML(reply)
      connected = doc.xpath('//methodresponse//connected')[0].content
      if(connected == "yes")
        return true
      end
    end
    return false
  end

  def milestone_grab_stream_frame
    aspect = "no"
    upsizing = "no"
    if(self.keep_aspect_ratio)
      aspect = "yes"
    end
    if(self.allow_upsizing)
      upsizing = "yes"
    end
    img_path = "#{Rails.root}/app/assets/images/#{self.camera.vms.id}_#{self.camera.id}_#{self.id}.jpeg"
    cmd = "java -jar #{Rails.root}/app/assets/programs/MilestoneFrameGrabber/mx_grab_stream_frame.jar #{self.camera.vms.server_ip} #{self.camera.vms.server_port} #{self.camera.vms.username} #{self.camera.vms.password} #{self.camera.camera_id} #{self.width} #{self.height} #{aspect} #{upsizing} #{self.compression_rate} #{img_path}"
    stdin, stdout, stderr = Open3.popen3(cmd)
    output = stdout.readline
    if(!output.nil? && output.start_with?("<"))
      doc = Nokogiri::XML(output)
      error = doc.xpath('//output//error')[0].content
      if(error == "no")
        return true
      else
        raise 'Failed to grab a frame of the stream.'
      end
    else
      raise 'Failed to grab a frame of the stream.'
    end
    return false
  end

  def zoneminder_grab_frame(streamUrl)
    complete = false
    width = -1
    height = -1
    img_path = "#{Rails.root}/app/assets/images/#{self.camera.vms.id}_#{self.camera.id}.jpeg"
    cmd = "java -jar #{Rails.root}/app/assets/programs/ZoneminderFrameGrabber/zm_grab_stream_frame.jar #{streamUrl} #{img_path}"
    stdin, stdout, stderr = Open3.popen3(cmd)
    output = stdout.readline
    if(!output.nil? && output.start_with?("<"))
      doc = Nokogiri::XML(output)
      error = doc.xpath('//output//error')[0].content
      if(!error.nil? && error == "no")
        width = doc.xpath('//output//width')[0].content.to_i
        height = doc.xpath('//output//height')[0].content.to_i
        complete = true
      end
    end
    return complete, width, height
  end

  def delete_frame
    path_to_file = "#{Rails.root}/app/assets/images/#{self.camera.vms.id}_#{self.camera.id}_#{self.id}.jpeg"
    File.delete(path_to_file) if File.exist?(path_to_file)
  end

end