class CreateAnalyticConfigs < ActiveRecord::Migration
  def change
    create_table :analytic_configs do |t|
      t.string :name
      t.string :fileName
      t.text :data, :limit => 16777215
      t.references :analytic, index: true

      t.timestamps
    end
  end
end
