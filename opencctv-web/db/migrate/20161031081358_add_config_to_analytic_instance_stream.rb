class AddConfigToAnalyticInstanceStream < ActiveRecord::Migration
  def change
    add_column :analytic_instance_streams, :config, :text
  end
end
