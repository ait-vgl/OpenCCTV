class AddPidToServers < ActiveRecord::Migration
  def change
    add_column :servers, :pid, :integer
  end
end
