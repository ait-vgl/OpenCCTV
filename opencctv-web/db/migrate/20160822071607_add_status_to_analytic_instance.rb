class AddStatusToAnalyticInstance < ActiveRecord::Migration
  def change
    add_column :analytic_instances, :status, :integer, default: 0
  end
end
