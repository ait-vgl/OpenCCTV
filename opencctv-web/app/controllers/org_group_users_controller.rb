class OrgGroupUsersController < ApplicationController
  before_action :authenticate_user!
  before_action :isOrganizationPageAdmin?
  before_action :setGroupUser
  respond_to :html

  def create

    begin

      @group_user.org_group_users.create(:org_user_id => params[:org_user_id])

      flash[:notice] = 'Successfully added a user to group.'
    rescue
      flash[:error] = 'Cannot added user to group.'
    end

    redirect_to group_user_path(@group_user)
  end


  def destroy
    @group_user.org_group_users.where(:org_group_users => {:org_user_id => params[:org_user_id]}).first().destroy
    flash[:notice] = 'Added user was successfully destroyed.'
    redirect_to group_user_path(@group_user)
  end


  private
  def setGroupUser
    @group_user = GroupUser.where(:id => params[:group_user_id], :org_id => session[:org_id]).first()
  end

end