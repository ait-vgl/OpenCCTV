class GroupUsersController < ApplicationController
  before_action :authenticate_user!
  before_action :isOrganizationPageAdmin?

  before_action :set_group_user, only: [:show, :edit, :update, :destroy]

  respond_to :html

  def index

    @group_users = GroupUser.getGroupListPerOrg(session[:org_id])
    respond_with(@group_users)
  end

  def show

    @addedUserLists = User.joins(org_users: :group_users).where(:org_users => {:org_id => session[:org_id]}, :group_users =>{:id => @group_user.id}).select('org_users.id','users.email')

    @userLists = User.joins(:orgs).where(:orgs => {:id => session[:org_id]}).where.not(:users => {:id => @addedUserLists.ids}).select('org_users.id','users.email')

    @org_group_user = OrgGroupUser.new
    respond_with(@group_user)
  end

  def new
    @group_user = GroupUser.new
    respond_with(@group_user)
  end

  def edit
  end

  def create
    @group_user = GroupUser.new(:title => params[:group_user][:title], :detail => params[:group_user][:detail],
                                :org_id => session[:org_id], :role_id => ApplicationController::ROLE_RESOURCE_ADMIN)
    @group_user.save
    respond_with(@group_user)
  end

  def update
    @group_user.update(group_user_params)
    respond_with(@group_user)
  end

  def destroy
    @group_user.destroy
    respond_with(@group_user)
  end

  def addUserGroup

  end

  private
    def set_group_user
      @group_user = GroupUser.where(:id => params[:id]).first()
    end

    def group_user_params
      params.require(:group_user).permit(:title, :detail)
    end
end
