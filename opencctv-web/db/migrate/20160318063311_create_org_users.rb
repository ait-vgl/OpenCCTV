class CreateOrgUsers < ActiveRecord::Migration
  def change
    create_table :org_users do |t|
      t.references :user, index: true
      t.references :org, index: true

      t.timestamps
    end
  end
end
