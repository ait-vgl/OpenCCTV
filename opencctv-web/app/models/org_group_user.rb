class OrgGroupUser < ActiveRecord::Base
  belongs_to :group_user
  belongs_to :org_user
end
