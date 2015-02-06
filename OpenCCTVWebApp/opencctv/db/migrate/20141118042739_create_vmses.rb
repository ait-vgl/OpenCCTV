class CreateVmses < ActiveRecord::Migration
  def change
    create_table :vmses do |t|
      t.string :server_name
      t.integer :server_port
      t.string :vms_type
      t.text :description
      t.string :username
      t.string :password

      t.timestamps
    end
  end
end
