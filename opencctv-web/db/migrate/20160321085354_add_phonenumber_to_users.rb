class AddPhonenumberToUsers < ActiveRecord::Migration
  def change
    add_column :users, :phone_number, :string
  end
end
