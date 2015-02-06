class CreateAnalyticInstances < ActiveRecord::Migration
  def change
    create_table :analytic_instances do |t|
      t.references :analytic, index: true
      t.string :name
      t.text :description

      t.timestamps
    end
  end
end
