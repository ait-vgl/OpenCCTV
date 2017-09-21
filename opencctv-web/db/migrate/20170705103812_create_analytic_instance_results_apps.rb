class CreateAnalyticInstanceResultsApps < ActiveRecord::Migration
  def change
    create_table :analytic_instance_results_apps do |t|
      t.references :analytic_instance, index: true
      t.references :results_app, index: true

      t.timestamps
    end
  end
end
