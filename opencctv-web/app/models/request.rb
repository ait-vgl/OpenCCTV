class Request < ActiveRecord::Base
  belongs_to :status
  belongs_to :org
  belongs_to :user
end
