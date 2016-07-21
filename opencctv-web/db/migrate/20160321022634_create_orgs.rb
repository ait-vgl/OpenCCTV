class CreateOrgs < ActiveRecord::Migration
  def change
    create_table :orgs do |t|
      t.string :title
      t.text :detail
      t.string :password

      t.timestamps
    end
  end
end
