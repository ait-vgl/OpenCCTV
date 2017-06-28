class ResultsAppInputParameter < ActiveRecord::Base
  belongs_to :results_app
  belongs_to :results_app_connector_parameter

  validates :results_app_connector_parameter, presence: true
  validates :value, presence: true
end
