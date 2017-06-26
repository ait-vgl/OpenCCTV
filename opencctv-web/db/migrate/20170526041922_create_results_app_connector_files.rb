class CreateResultsAppConnectorFiles < ActiveRecord::Migration
  def change
    create_table :results_app_connector_files do |t|
      t.string :name
      t.boolean :required
      t.text :description
      t.references :results_app_connector, index: true

      t.timestamps
    end
  end
end
