class AddReferencesToAnalyticInstance < ActiveRecord::Migration
  def change
    add_reference :analytic_instances, :analytic_server, index: true
  end
end
