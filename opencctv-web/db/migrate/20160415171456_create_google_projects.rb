class CreateGoogleProjects < ActiveRecord::Migration
  def change
    create_table :google_projects do |t|
      t.string :application_name
      t.string :project_number
      t.string :api_key

      t.timestamps
    end
  end
end
