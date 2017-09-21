class AnalyticInstanceResultsApp < ActiveRecord::Base
  belongs_to :analytic_instance
  belongs_to :results_app

  validates :analytic_instance_id, presence: true
  validates :results_app_id, presence: true
end
