class CreateVmsConnectors < ActiveRecord::Migration
  def change
    create_table :vms_connectors do |t|
      t.string :name
      t.text :description
      t.string :filename
      t.boolean :verified, :null => false, :default => false
      t.text :log
      t.timestamps
    end
  end
end
