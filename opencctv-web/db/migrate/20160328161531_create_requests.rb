class CreateRequests < ActiveRecord::Migration
  def change
    create_table :requests do |t|
      t.references :status, index: true
      t.references :org, index: true
      t.references :user, index: true
      t.timestamps
    end
  end
end
