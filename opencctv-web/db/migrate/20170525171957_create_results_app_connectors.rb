class CreateResultsAppConnectors < ActiveRecord::Migration
  def change
    create_table :results_app_connectors do |t|
      t.string :name
      t.text :description
      t.string :filename
      t.boolean :verified

      t.timestamps
    end
  end
end
