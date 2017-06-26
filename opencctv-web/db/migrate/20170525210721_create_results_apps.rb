class CreateResultsApps < ActiveRecord::Migration
  def change
    create_table :results_apps do |t|
      t.string :name
      t.text :description
      t.references :results_app_connector, index: true

      t.timestamps
    end
  end
end
