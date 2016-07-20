class CreateResults < ActiveRecord::Migration
  def change
    create_table :results do |t|
      t.integer :analytic_instance_id
      t.string :timestamp
      t.text :result_text
      t.datetime :time
      t.integer :count

      t.timestamps null: false
    end
  end
end
