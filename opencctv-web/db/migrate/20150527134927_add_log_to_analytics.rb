class AddLogToAnalytics < ActiveRecord::Migration
  def change
    add_column :analytics, :log, :text
  end
end
