class CreateAuthTokens < ActiveRecord::Migration
  def change
    create_table :auth_tokens do |t|

      t.timestamps
    end
  end
end
