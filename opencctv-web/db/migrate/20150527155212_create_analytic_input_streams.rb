class CreateAnalyticInputStreams < ActiveRecord::Migration
  def change
    create_table :analytic_input_streams do |t|
      t.string :name
      t.text :description
      t.references :analytic, index: true

      t.timestamps
    end
  end
end
