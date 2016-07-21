class MobileNotification < ActiveRecord::Base
  belongs_to :analytic
  belongs_to :google_project

  validates :analytic_id, :google_project_id, presence: true
end
