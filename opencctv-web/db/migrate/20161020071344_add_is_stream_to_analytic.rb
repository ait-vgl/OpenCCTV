class AddisStreamToAnalytic < ActiveRecord::Migration
  def change
    add_column :analytics, :is_stream, :boolean, default: false
  end
end
