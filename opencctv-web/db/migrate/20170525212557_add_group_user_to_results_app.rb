class AddGroupUserToResultsApp < ActiveRecord::Migration
  def change
    add_reference :results_apps, :group_user, index: true
  end
end
