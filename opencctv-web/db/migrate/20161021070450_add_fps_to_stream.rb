class AddFpsToStream < ActiveRecord::Migration
  def change
    add_column :streams, :fps, :integer
  end
end
