class Analytic < ActiveRecord::Base
  has_many :analytic_input_streams, dependent: :destroy
  has_many :analytic_instances, dependent: :destroy

  validates :name, presence: true
  validates :description, presence: true
  validates_presence_of :location, message: 'No analytic files selected to upload'

  # read the names of the input streams if they are specified in the analytic library
  # return an array of AnalyticInputStream objects if stream details can be obtained from the library
  def read_analytic_input_stream_names
    input_streams = Array.new
    if !location.empty?
      cmd = "#{Rails.root}/app/assets/programs/AnalyticStreamNameRead #{self.location}"
      stdin, stdout, stderr = Open3.popen3(cmd)
      if stderr.readlines.empty?
        output = stdout.readlines.join('');
        if(!output.nil? && (output.start_with?("<")))
          doc = Nokogiri::XML(output)
          doc.xpath('//inputstreams/input').each do |in_stream|
            stream = AnalyticInputStream.new
            stream.name = in_stream.xpath("name")[0].content
            stream.description = in_stream.xpath("description")[0].content
            input_streams.push(stream);
          end
        end
      end
      return input_streams
    end
  end

end
