class ResultsAppInputFile < ActiveRecord::Base
  belongs_to :results_app
  belongs_to :results_app_connector_file

  #validates :results_app, presence: true
  validates :results_app_connector_file, presence: true
  validates_presence_of :filename, message: 'No input file is selected to upload.'
end
