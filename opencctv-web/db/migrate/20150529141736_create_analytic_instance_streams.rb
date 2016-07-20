class CreateAnalyticInstanceStreams < ActiveRecord::Migration
  def change
    create_table :analytic_instance_streams do |t|
      t.references :analytic_instance, index: true
      t.references :analytic_input_stream, index: true
      t.references :stream, index: true

      t.timestamps
    end
  end
end
