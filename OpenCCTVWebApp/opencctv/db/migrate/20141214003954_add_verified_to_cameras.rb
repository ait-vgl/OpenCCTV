class AddVerifiedToCameras < ActiveRecord::Migration
  def change
    add_column :cameras, :verified, :boolean
  end
end
