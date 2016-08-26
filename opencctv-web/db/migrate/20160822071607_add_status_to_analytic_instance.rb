class AddStatusToAnalyticInstance < ActiveRecord::Migration
  def change
    add_column :analytic_instances, :status, :boolean, default: false
  end
end
