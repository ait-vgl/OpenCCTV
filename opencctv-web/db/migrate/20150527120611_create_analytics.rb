class CreateAnalytics < ActiveRecord::Migration
  def change
    create_table :analytics do |t|
      t.string :name
      t.text :description
      t.string :filename
      t.boolean :verified, :null => false, :default => false

      t.timestamps
    end
  end
end
