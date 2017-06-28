class CreateResultsAppConnectorParameters < ActiveRecord::Migration
  def change
    create_table :results_app_connector_parameters do |t|
      t.string :name
      t.boolean :required
      t.boolean :editable
      t.text :description
      #t.references :results_app_connector, index: true
      t.references :results_app_connector, index: { name: 'index_r_app_connector_params_on_r_app_connector_id' }

      t.timestamps
    end
  end
end
