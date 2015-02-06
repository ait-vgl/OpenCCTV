class AddNameToStreams < ActiveRecord::Migration
  def change
    add_column :streams, :name, :string
  end
end
