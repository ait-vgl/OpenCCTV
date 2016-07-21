class CreateGoogleTokens < ActiveRecord::Migration
  def change
    create_table :google_tokens do |t|
      t.string :reg_token
      t.integer :analytic_id
      t.integer :user_id
      t.references :user, index: true

      t.timestamps
    end
  end
end
