class AddLogToResultsAppConnectors < ActiveRecord::Migration
  def change
    add_column :results_app_connectors, :log, :text
  end
end
