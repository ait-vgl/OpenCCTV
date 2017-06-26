class AddUserToResultsApp < ActiveRecord::Migration
  def change
    add_reference :results_apps, :user, index: true
  end
end
