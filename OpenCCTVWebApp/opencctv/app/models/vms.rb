class Vms < ActiveRecord::Base
  has_many :cameras, dependent: :destroy
  validates :server_name, presence: true
  validates :server_port, presence: true
  validates :vms_type, presence: true

  def set_verification(is_verified)
    if(!is_verified)
      self.cameras.each do |camera|
        camera.set_verification(false)
      end
    end
    update(:verified => is_verified)
  end

  # validates a Milestone VMS
  # returns (true, systeminfo_xml_string) if valid or (false, error_msg) if invalid
  def milestone_validate_vms
    response = connect_to_milestone_enterprise(self.server_name, self.server_port, self.username, self.password)
    if response.nil? || response.code == "404" # if a enterprise server cannot be found
      response = connect_to_milestone_corporate(self.server_name, self.server_port, self.username, self.password)
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

  private
  def connect_to_milestone_enterprise(server_name, server_port, username, password)
    url = "http://#{server_name}/systeminfo.xml"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  def connect_to_milestone_corporate(server_name, server_port, username, password)
    url = "http://#{server_name}/rcserver/systeminfo.xml"
    return connect_with_basic_auth(url, server_port.to_i, username, password)
  end

  def connect_with_basic_auth(url, port_number, username, password)
    uri = URI.parse(url)
    http = Net::HTTP.new(uri.host, port_number)
    http.open_timeout = 5
    http.read_timeout = 5
    request = Net::HTTP::Get.new(uri.request_uri)
    request.basic_auth(username, password)
    begin
      return http.request(request)
    rescue
      return nil
    end
  end
end
