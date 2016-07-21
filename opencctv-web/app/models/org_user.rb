class OrgUser < ActiveRecord::Base
  has_many :org_group_users, dependent: :destroy
  has_many :group_users, through: :org_group_users

  belongs_to :org
  belongs_to :user
end
