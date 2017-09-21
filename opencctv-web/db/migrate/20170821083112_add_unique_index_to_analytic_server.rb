class AddUniqueIndexToAnalyticServer < ActiveRecord::Migration
  def change
    add_index :analytic_servers, [:name], unique: true
    add_index :analytic_servers, [:ip, :port], unique: true
  end
end
