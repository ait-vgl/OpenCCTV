class AnalyticInputStream < ActiveRecord::Base
  belongs_to :analytic

  validates :name, presence: true
  validates :description, presence: true
end
