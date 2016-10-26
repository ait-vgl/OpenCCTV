class GoogleToken < ActiveRecord::Base
  belongs_to :user
  belongs_to :analytic

  validates_uniqueness_of :reg_token
end
