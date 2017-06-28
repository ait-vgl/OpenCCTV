class CreateResultsAppInputParameters < ActiveRecord::Migration
  def change
    create_table :results_app_input_parameters do |t|
      t.references :results_app, index: true
      t.references :results_app_connector_parameter, index: { name: 'index_r_app_input_params_on_r_app_connector_param_id'}
      t.string :value

      t.timestamps
    end
  end
end
