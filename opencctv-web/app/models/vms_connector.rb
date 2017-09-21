class VmsConnector < ActiveRecord::Base
  has_many :vmses, dependent: :destroy

  validates :name, presence: true
  validates_presence_of :filename, message: 'No VMS connector plugin archive selected to upload.'

  def validate_plugin_archive

    verified = false
    html_content = 'Error occured during VMS connector plugin validation'

    if(!self.filename.empty?)
      cmd = "#{Rails.root}/app/assets/programs/PluginArchiveValidator/Release/PluginArchiveValidator " +
          "#{Rails.root}/app/uploads/vms_connectors " + "#{self.filename}.zip " + "#{self.filename} " + 'vms'
      puts cmd

      begin
        stdin, stdout, stderr = Open3.popen3(cmd)
        output = stdout.readline
      rescue
        output = nil
      end

      #puts "PluginArchiveValidator output fot VMS - #{output}"

      if(!output.nil? && (output.start_with?("<")))
        xml = Nokogiri::XML(output)
        begin
          if(xml.xpath('//pluginvalidatorreply/verified')[0].content == '1')
            verified = true
          end
          html_content = xml.xpath('//pluginvalidatorreply/html')[0].inner_html
        rescue
          html_content = 'Invalid message format received from the Plugin Validator' if html_content.nil?
        end
      end
    end

    self.verified = verified
    self.log = html_content

    return verified
  end
end
