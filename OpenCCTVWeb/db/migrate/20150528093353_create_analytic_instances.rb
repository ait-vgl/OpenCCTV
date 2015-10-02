class CreateAnalyticInstances < ActiveRecord::Migration
  def change
    create_table :analytic_instances do |t|
      t.string :name
      t.text :description
      t.references :analytic, index: true

      t.timestamps
    end
  end
end
