class CreateAnalyticServers < ActiveRecord::Migration
  def change
    create_table :analytic_servers do |t|
      t.string :name
      t.string :ip
      t.integer :port
      t.string  :tag
      t.string :status
      t.integer :pid

      t.timestamps
    end
  end
end
