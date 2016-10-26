class CreateStreams < ActiveRecord::Migration
  def change
    create_table :streams do |t|
      t.string :name
      t.text :description
      t.integer :width
      t.integer :height
      t.boolean :keep_aspect_ratio
      t.boolean :allow_upsizing
      t.integer :compression_rate
      t.boolean :verified, :null => false, :default => false
      t.references :camera, index: true

      t.timestamps
    end
  end
end
