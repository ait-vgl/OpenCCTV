class CreateResultsAppInputFiles < ActiveRecord::Migration
  def change
    create_table :results_app_input_files do |t|
      t.references :results_app, index: true
      t.references :results_app_connector_file, index: true
      t.string :filename

      t.timestamps
    end
  end
end
