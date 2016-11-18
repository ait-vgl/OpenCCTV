class AddReferencesToVms < ActiveRecord::Migration
  def change
    add_reference :vmses, :open_cctv_server, index: true
  end
end
