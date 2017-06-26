class ResultsAppConnectorFile < ActiveRecord::Base
  belongs_to :results_app_connector
  has_many :results_app_input_files

  validates :name, presence: true
end
