class VmsConnector < ActiveRecord::Base
  has_many :vmses, dependent: :destroy

  validates :name, presence: true
  validates_presence_of :filename, message: 'No VMS connector plugin archive selected to upload.'

  def validate_plugin_archive
    if(!self.filename.empty?)
      cmd = "#{Rails.root}/app/assets/programs/PluginArchiveValidator/Release/PluginArchiveValidator " +
          "#{Rails.root}/app/uploads/vms_connectors " + "#{self.filename}.zip " + "#{self.filename} " + 'vms'
          
      puts cmd
      stdin, stdout, stderr = Open3.popen3(cmd)
      output = stdout.readline
      if(!output.nil? && (output.start_with?("<")))
        xml = Nokogiri::XML(output)
        if(xml.xpath('//pluginvalidatorreply/verified')[0].content == '1')
          self.verified = true
        end
        self.log = xml.xpath('//pluginvalidatorreply/html')[0].inner_html
      end
    end
    return self.verified?
  end
end
