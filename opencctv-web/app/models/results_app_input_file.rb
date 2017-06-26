class ResultsAppInputFile < ActiveRecord::Base
  belongs_to :results_app
  belongs_to :results_app_connector_file, dependent: :destroy
end
