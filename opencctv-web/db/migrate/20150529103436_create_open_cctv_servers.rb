class CreateOpenCctvServers < ActiveRecord::Migration
  def change
    create_table :open_cctv_servers do |t|
      t.string :name
      t.string :host
      t.integer :port
      t.string :status
      t.integer :pid

      t.timestamps
    end
  end
end
