class AddisStreamToVms < ActiveRecord::Migration
  def change
    add_column :vms_connector, :is_stream, :boolean, default: false
  end
end
