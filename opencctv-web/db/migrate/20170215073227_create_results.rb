class CreateResults < ActiveRecord::Migration
  def change
    create_table :results do |t|
      t.string :result_text
      t.string :location
      t.time :timestamp
      t.references :analytic_instance, index: true
      t.references :analytic, index: true
      t.references :vms, index: true
      t.references :camera, index: true

      t.timestamps
    end
  end
end
