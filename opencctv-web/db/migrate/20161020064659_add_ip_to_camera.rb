class AddIpToCamera < ActiveRecord::Migration
  def change
    add_column :cameras, :ip, :string
    add_column :cameras, :port, :integer
    add_column :cameras, :username, :string
    add_column :cameras, :password, :string
  end
end
