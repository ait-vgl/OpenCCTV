class CreateResultsAppParameters < ActiveRecord::Migration
  def change
    create_table :results_app_parameters do |t|
      t.string :name
      t.string :value
      t.references :results_app, index: true

      t.timestamps
    end
  end
end
