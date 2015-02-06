class AnalyticInputStream < ActiveRecord::Base
  belongs_to :analytic
  has_many :analytic_instance_streams, dependent: :destroy

  validates :name, presence: true
  validates :description, presence: true
end
