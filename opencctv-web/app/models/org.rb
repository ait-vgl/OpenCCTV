class Org < ActiveRecord::Base

 # belongs_to :user

  has_many :group_users, dependent: :destroy
  has_many :requests, dependent: :destroy

  has_many :org_users, dependent: :destroy
  has_many :users, through: :org_users
end
