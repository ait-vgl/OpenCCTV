class CreateOrgGroupUsers < ActiveRecord::Migration
  def change
    create_table :org_group_users do |t|

      t.references :org_user, index: true
      t.references :group_user, index: true

      t.timestamps
    end
  end
end
