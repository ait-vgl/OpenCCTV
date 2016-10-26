class AddHttpPortToCamera < ActiveRecord::Migration
  def change
    add_column :cameras, :http_port, :integer
    add_column :cameras, :video_port, :integer
  end
end
