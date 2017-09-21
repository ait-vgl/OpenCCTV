class ResultsApp < ActiveRecord::Base
  belongs_to :results_app_connector
  belongs_to :user
  belongs_to :group_user

  has_many :results_app_input_files, dependent: :destroy
  has_many :results_app_input_parameters, dependent: :destroy
  has_many :analytic_instance_results_apps, dependent: :destroy

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

    input_param_ids = self.results_app_input_parameters.select(:results_app_connector_parameter_id)
    required_params = self.results_app_connector.results_app_connector_parameters.where(required: true)
    remaining_params = required_params.where.not(id: input_param_ids)

    self.usable =  remaining_files.empty? && remaining_params.empty?
    self.save
  end

end
