class ResultsApp < ActiveRecord::Base
  belongs_to :results_app_connector
  belongs_to :user
  belongs_to :group_user

  has_many :results_app_input_files, dependent: :destroy

  has_many :results_app_parameters, dependent: :destroy

  #validates :name, presence: true
end
