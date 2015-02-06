class AddVerifiedToVmses < ActiveRecord::Migration
  def change
    add_column :vmses, :verified, :boolean
  end
end
