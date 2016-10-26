class AddCodecToStream < ActiveRecord::Migration
  def change
    add_column :streams, :codec, :string
    add_column :streams, :protocol, :string
    add_column :streams, :url, :string
  end
end
