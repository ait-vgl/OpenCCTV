class AnalyticInstance < ActiveRecord::Base
  belongs_to :analytic
  has_many :analytic_instance_streams, dependent: :destroy
  has_many :streams, through: :analytic_instance_streams

  belongs_to :user
  belongs_to :group_user

  #has_many :group_cans, dependent: :destroy
  #has_many :group_users, through: :group_cans

  validates :analytic_id, presence: true
  validates :name, presence: true
end
