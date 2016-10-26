class CreateNotifications < ActiveRecord::Migration
  def change
    create_table :notifications do |t|
      t.string :title
      t.text :email
      t.text :to_user

      t.references :user, index: true
      t.references :group_user, index: true

      t.timestamps
    end
  end
end
