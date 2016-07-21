class AddCreatedByToOrg < ActiveRecord::Migration
  def change
    add_column :orgs, :created_by, :integer
  end
end
