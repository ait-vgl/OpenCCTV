class AddVerifiedToStreams < ActiveRecord::Migration
  def change
    add_column :streams, :verified, :boolean
  end
end
