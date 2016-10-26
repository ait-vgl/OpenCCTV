class GroupUser < ActiveRecord::Base
  #has_many :group_cans, dependent: :destroy
  #has_many :roles, through: :group_cans

  belongs_to :org
  belongs_to :role

  has_many :org_group_users, dependent: :destroy
  has_many :org_users, through: :org_group_users

  #has_many :cameras, through: :org_group_users
  has_many :vmses, dependent: :destroy
  has_many :analytic_instances, dependent: :destroy
  has_many :notifications, dependent: :destroy



  def self.getGroupUserList(org_id, user_id)
    return self.joins(:org_users).where(:org_id => org_id, :org_users => {:user_id => user_id}).pluck(:group_user_id)
  end

  def self.getGroupUserListSelection(org_id, user_id)
    return self.joins(:org_users).where(:org_id => org_id, :org_users => {:user_id => user_id})
        .select('group_users.id', 'group_users.title')
  end


  def self.getGroupListPerOrg(org_id)
    return self.where(:org_id => org_id)
  end

end
