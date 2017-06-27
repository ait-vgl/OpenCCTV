class AddUsableToResultsApp < ActiveRecord::Migration
  def change
    add_column :results_apps, :usable, :boolean
  end
end
