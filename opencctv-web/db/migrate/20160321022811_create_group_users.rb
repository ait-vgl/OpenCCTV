class CreateGroupUsers < ActiveRecord::Migration
  def change
    create_table :group_users do |t|
      t.string :title
      t.text :detail


      t.references :org, index: true
      t.references :role, index: true

      t.timestamps
    end
  end
end
