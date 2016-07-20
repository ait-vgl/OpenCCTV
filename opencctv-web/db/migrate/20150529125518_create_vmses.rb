class CreateVmses < ActiveRecord::Migration
  def change
    create_table :vmses do |t|
      t.string :name
      t.text :description
      t.string :server_ip
      t.integer :server_port
      t.string :username
      t.string :password
      t.boolean :verified, :null => false, :default => false
      t.references :vms_connector, index: true

      t.timestamps
    end
  end
end
