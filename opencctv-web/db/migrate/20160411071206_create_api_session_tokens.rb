class CreateApiSessionTokens < ActiveRecord::Migration

  def change
    create_table :api_session_tokens do |t|
      t.string :token
      t.references :user, index: true

      t.timestamps
    end
  end
end
