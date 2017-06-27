class ResultsApp < ActiveRecord::Base
  belongs_to :results_app_connector
  belongs_to :user
  belongs_to :group_user

  has_many :results_app_input_files, dependent: :destroy

  has_many :results_app_parameters, dependent: :destroy

  validates :name, presence: true
  validates :results_app_connector_id, presence: true

  def set_usable
    input_file_ids = self.results_app_input_files.select(:results_app_connector_file_id)
    required_files = self.results_app_connector.results_app_connector_files.where(required: true)
    remaining_files = required_files.where.not(id: input_file_ids)

=begin
    puts 'remaining_files'
    remaining_files.each  do |file|
      puts file.id
    end
=end

    # TODO Similarly check all the required parameters are set

    self.usable =  remaining_files.empty?
    self.save
  end

end
