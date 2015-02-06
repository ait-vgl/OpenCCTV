class AnalyticInstance < ActiveRecord::Base
  belongs_to :analytic
  has_many :analytic_instance_streams, dependent: :destroy
  has_many :streams, through: :analytic_instance_streams

  validates :analytic_id, presence: true
  validates :name, presence: true
end
