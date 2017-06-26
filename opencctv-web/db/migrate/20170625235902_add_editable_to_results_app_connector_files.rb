class AddEditableToResultsAppConnectorFiles < ActiveRecord::Migration
  def change
    add_column :results_app_connector_files, :editable, :boolean
  end
end
