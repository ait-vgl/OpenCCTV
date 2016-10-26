class CreateMobileNotifications < ActiveRecord::Migration
  def change
    create_table :mobile_notifications do |t|
      t.references :analytic, index: true
      t.references :google_project, index: true

      t.timestamps
    end
  end
end
