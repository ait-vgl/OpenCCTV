class ResultsAppConnectorParameter < ActiveRecord::Base
  belongs_to :results_app_connector
  has_many :results_app_input_parameters, dependent: :destroy

  validates :name, presence: true
  #editable is always true if the parameter is manually added, false otherwise
  #validates :editable, presence: true
  #validates :required, presence: true
end
