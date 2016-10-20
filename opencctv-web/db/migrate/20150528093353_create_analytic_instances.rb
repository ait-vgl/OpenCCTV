class CreateAnalyticInstances < ActiveRecord::Migration
  def change
    create_table :analytic_instances do |t|
      t.string :name
      t.text :description

      t.references :analytic, index: true
      t.references :notification, index: true
      t.references :user, index: true
      t.references :group_user, index: true

      t.timestamps
    end
  end
end
