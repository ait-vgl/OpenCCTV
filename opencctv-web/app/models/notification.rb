class Notification < ActiveRecord::Base

  belongs_to :user
  belongs_to :group_user

  #has_many :group_cans, dependent: :destroy
  #has_many :group_users, through: :group_cans
end
