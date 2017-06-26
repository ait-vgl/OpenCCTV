class ResultsAppConnector < ActiveRecord::Base

  has_many :results_app_connector_files,  dependent: :destroy
  has_many :results_apps, dependent: :destroy

  validates :name, presence: true
  validates_presence_of :filename, message: 'No results application connector plugin archive selected to upload.'

  def validate_plugin_archive
    validation_result = {}
    verified = false
    html_content = nil
    input_files = Array.new

    if(!self.filename.empty?)
      cmd = "#{Rails.root}/app/assets/programs/PluginArchiveValidator/Release/PluginArchiveValidator " +
          "#{Rails.root}/app/uploads/results_app_connectors " + "#{self.filename}.zip " + "#{self.filename} " + 'results_app'
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

        #Input file details
        xml.xpath('//pluginvalidatorreply/result/inputfiles/inputfile').each do |in_file|
          input_file = ResultsAppConnectorFile.new
          input_file.name = in_file.xpath("name")[0].content
          input_file.required = in_file.xpath("required")[0].content
          input_file.editable = false
          input_file.description = in_file.xpath("description")[0].content
          input_files.push(input_file)
        end
      end
    end

    validation_result[:verified] = verified
    validation_result[:html_content] = html_content
    validation_result[:input_files] = input_files

    return validation_result
  end

end
