class Analytic < ActiveRecord::Base
  has_many :analytic_input_streams, dependent: :destroy
  has_many :analytic_instances, dependent: :destroy

  validates :name, presence: true
  validates_presence_of :filename, message: 'No analytic plugin archive selected to upload.'

  def validate_plugin_archive

    validation_result = {}
    verified = false
    html_content = nil
    input_streams = Array.new

    if(!self.filename.empty?)
      cmd = "#{Rails.root}/app/assets/programs/PluginArchiveValidator/Release/PluginArchiveValidator " +
            "#{Rails.root}/app/uploads/analytics " + "#{self.filename}.zip " + "#{self.filename} " + 'analytic'
      stdin, stdout, stderr = Open3.popen3(cmd)
      output = stdout.readline
      if(!output.nil? && (output.start_with?("<")))
        xml = Nokogiri::XML(output)

        #Verification results
        if(xml.xpath('//pluginvalidatorreply/verified')[0].content == '1')
          verified = true
        end

        #HTML content
        html_content = xml.xpath('//pluginvalidatorreply/html')[0].inner_html

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

    return validation_result
  end

end
