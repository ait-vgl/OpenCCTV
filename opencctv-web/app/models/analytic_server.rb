class AnalyticServer < ActiveRecord::Base
  validates :name, presence: true
  validates :ip, presence: true
  validates :port, presence: true
end
