class AddConectionTypeToVmses < ActiveRecord::Migration
  def change
    add_column :vmses, :connection_type, :string
    add_column :vmses, :isFrameGrabber, :boolean
  end
end
