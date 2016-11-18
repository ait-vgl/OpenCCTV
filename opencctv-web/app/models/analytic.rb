class Analytic < ActiveRecord::Base
  has_many :analytic_input_streams, dependent: :destroy
  has_many :analytic_instances, dependent: :destroy

  has_many :analytic_configs, dependent: :destroy

  validates :name, presence: true
  validates_presence_of :filename, message: 'No analytic plugin archive selected to upload.'

  def validate_plugin_archive

    validation_result = {}
    verified = false
    html_content = nil
    input_streams = Array.new
    analytic_configs = Array.new

    if(!self.filename.empty?)
      cmd = "#{Rails.root}/app/assets/programs/PluginArchiveValidator/Release/PluginArchiveValidator " +
            "#{Rails.root}/app/uploads/analytics " + "#{self.filename}.zip " + "#{self.filename} " + 'analytic'
      stdin, stdout, stderr = Open3.popen3(cmd)
      output = stdout.readline
      puts cmd
      if(!output.nil? && (output.start_with?("<")))
        xml = Nokogiri::XML(output)

        #Verification results
        if(xml.xpath('//pluginvalidatorreply/verified')[0].content == '1')
          verified = true
        end

        #HTML content
        html_content = xml.xpath('//pluginvalidatorreply/html')[0].inner_html

        puts output

        #Analytic config file details
        xml.xpath('//pluginvalidatorreply/analytic/configfiles/configfile').each do |in_analytic_config|

          puts in_analytic_config.xpath("filename")[0].content
          puts in_analytic_config.xpath("name")[0].content

          analyticConfig = AnalyticConfig.new
          analyticConfig.name = in_analytic_config.xpath("name")[0].content
          analyticConfig.fileName = in_analytic_config.xpath("filename")[0].content
          analyticConfig.data = File.read("/usr/local/opencctv/analytics/#{self.filename}/#{in_analytic_config.xpath("filename")[0].content}")
          analytic_configs.push(analyticConfig);
        end

        #Input stream details
        xml.xpath('//pluginvalidatorreply/analytic/inputstreams/input').each do |in_stream|
          stream = AnalyticInputStream.new
          stream.name = in_stream.xpath("name")[0].content
          stream.description = in_stream.xpath("description")[0].content
          input_streams.push(stream);
        end
      end
    end

    validation_result[:verified] = verified
    validation_result[:html_content] = html_content
    validation_result[:input_streams] = input_streams
    validation_result[:analytic_configs] = analytic_configs

    return validation_result


  end

end
