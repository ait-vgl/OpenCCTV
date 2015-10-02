class AddVmsTypeToVmses < ActiveRecord::Migration
  def change
    add_column :vmses, :vms_type, :string
  end
end
